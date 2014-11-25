/////////////////////////////////////////////////////////////////////
//
// ServerSocketThread.cc
//
/////////////////////////////////////////////////////////////////////

#include <iostream>

#include "ServerSocketThread.h"
#include "Exception.h"

using namespace std;

/////////////////////////////////////////////////////////////////////

void ServerSocketThread::listenThreadFunctionStatic( ServerSocketThread * self )
{
	try {
		self->listenThreadFunction();
	}
	catch ( const exception & e ) {
		cout << "ServerSocketThread::listenThreadFunctionStatic() : exception : " << e.what() << "\n";
	}
	catch ( ... ) {
		cout << "ServerSocketThread::listenThreadFunctionStatic() : caught in ...\n";
	}
}

void ServerSocketThread::acceptThreadFunctionStatic( ServerSocketThread * self, shared_ptr<Socket> s, const string & adrPortStr )
{
	try {
		SocketEventData data(s);
		self->acceptThreadFunction(data);
	}
	catch ( const exception & e ) {
		cout << "ServerSocketThread::acceptThreadFunctionStatic() : exception : " << e.what() << "\n";
	}
	catch ( ... ) {
		cout << "ServerSocketThread::acceptThreadFunctionStatic() : caught in ...\n";
	}

	try {
		unique_lock<mutex> lock(self->m_connectionMapMutex);

		ConnectionInfo & ci = self->m_connectionMap[adrPortStr];
		ci.m_terminated = true;
		ci.m_socket->close();
	}
	catch ( const exception e ) {
		cout << "ServerSocketThread::acceptThreadFunctionStatic() : exception\n";
	}
	catch ( ... ) {
		cout << "ServerSocketThread::acceptThreadFunctionStatic() : caught in ...\n";
	}
}

void ServerSocketThread::cleanupThreadFunctionStatic( ServerSocketThread * self )
{
	try {
		uint64_t count = 0;

		while ( true ) {
			this_thread::sleep_for( chrono::milliseconds(1000) );
			{
				unique_lock<mutex> lock(self->m_connectionMapMutex);
				if ( self->m_stopped ) break;
			}
			if ( ( count % 8 ) == 0 ) self->cleanup();
			count++;
		}
		while ( true ) {
			self->cleanup();
			{
				unique_lock<mutex> lock(self->m_connectionMapMutex);
				if ( self->m_connectionMap.size() == 0 ) break;
			}
			this_thread::sleep_for( chrono::milliseconds(1000) );
		}
	}
	catch ( const exception e ) {
		cout << "ServerSocketThread::cleanupThreadFunctionStatic() : exception\n";
	}
	catch ( ... ) {
		cout << "ServerSocketThread::cleanupThreadFunctionStatic() : caught in ...\n";
	}
}

/////////////////////////////////////////////////////////////////////

ServerSocketThread::ServerSocketThread( EventHandler & hlr ) : m_hlr(hlr)
{
	m_stopped = false;
}

void ServerSocketThread::start( const IpAddress & adr, SocketBase::Type type )
{
	unique_lock<mutex> lock(m_connectionMapMutex);

	m_serverSocket.open(adr.getType(),type);

	m_serverSocket.bind( adr );

	m_listenThread.reset( new thread(listenThreadFunctionStatic,this) );
	m_cleanupThread.reset( new thread(cleanupThreadFunctionStatic,this) );
}

void ServerSocketThread::stop()
{
	unique_lock<mutex> lock(m_connectionMapMutex);
	if (m_stopped) return;
	m_stopped = true;
	m_serverSocket.stop();
	for ( auto i : m_connectionMap ) i.second.m_socket->stop();
}

void ServerSocketThread::join()
{
	m_listenThread->join();
	m_cleanupThread->join();
}

void ServerSocketThread::lock()
{
	m_connectionMapMutex.lock();
}

void ServerSocketThread::unlock()
{
	m_connectionMapMutex.unlock();
}

bool ServerSocketThread::find_unlocked( const IpAddress & adr )
{
	string adrPortStr = adr.getAddressPortString();
	if ( m_connectionMap.find(adrPortStr) == m_connectionMap.end() )
		return( false );
	return( true );
}

bool ServerSocketThread::write_unlocked( const IpAddress & adr, const Buffer & buf )
{
	return(getSocket_unlocked(adr)->write(buf));
}

void ServerSocketThread::close_unlocked( const IpAddress & adr )
{
	getSocket_unlocked(adr)->close();
}

shared_ptr<Socket> ServerSocketThread::getSocket_unlocked( const IpAddress & adr )
{
	string adrPortStr = adr.getAddressPortString();
	auto i = m_connectionMap.find(adrPortStr);
	if ( i == m_connectionMap.end() )
		throw Primitives2Exception("Not found");
	return( i->second.m_socket );
}

vector<shared_ptr<IpAddress> > ServerSocketThread::getAddressList_unlocked()
{
	vector<shared_ptr<IpAddress> > ret;
	for ( auto i : m_connectionMap )
		ret.push_back( IpAddress::create(i.first) );
	return( ret );
}

void ServerSocketThread::listenThreadFunction()
{
	while ( true ) {
		tuple<bool, shared_ptr<Socket> > res = m_serverSocket.accept();

		bool run = get<0>(res);
		shared_ptr<Socket> s = get<1>(res);

		if ( ! run ) return;

		string adrPortStr = s->getAcceptedAddress().getAddressPortString();

		unique_lock<mutex> lock(m_connectionMapMutex);
		if ( m_stopped ) return;

		ConnectionInfo & ci = m_connectionMap[adrPortStr];
		ci.m_socket = s;
		ci.m_thread.reset( new thread(acceptThreadFunctionStatic,this,s,adrPortStr) );
	}
}

void ServerSocketThread::acceptThreadFunction( EventData & data )
{
	m_hlr.event( data );
}

void ServerSocketThread::cleanup()
{
	unique_lock<mutex> lock(m_connectionMapMutex);

	auto i = m_connectionMap.begin();
	auto i1 = i;

	while ( i != m_connectionMap.end() ) {
		i++;

		if ( i1->second.m_terminated ) {
			i1->second.m_thread->join();
			m_connectionMap.erase(i1);
		}

		i1 = i;
	}
}


