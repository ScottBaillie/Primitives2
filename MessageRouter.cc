/////////////////////////////////////////////////////////////////////
//
// MessageRouter.cc
//
/////////////////////////////////////////////////////////////////////

#include <iostream>

#include "Logger.h"
#include "Message.h"
#include "ConvertFromBinary.h"
#include "ConvertToBinary.h"
#include "ConvertBinary.h"
#include "Exception.h"

using namespace std;

/////////////////////////////////////////////////////////////////////

MessageRouter::MessageRouter() :
	m_messageReceived(*this,&MessageRouter::messageReceived)
{
	m_stopped = false;
}

vector<string> MessageRouter::getMessageProcessorNames()
{
	unique_lock<mutex> lock(m_mutex);

	vector<string> names;
	for ( auto i : m_messageProcessorMap )
		names.push_back( i.first );
	return( names );
}

void MessageRouter::createMessageProcessor( const string & name, EventHandler & hlr )
{
	unique_lock<mutex> lock(m_mutex);

	if (m_stopped) return;

	shared_ptr<MessageProcessor> s;
	s.reset( new MessageProcessor(hlr) );
	s->start();
	m_messageProcessorMap[name] = s;
}

void MessageRouter::removeMessageProcessor( const string & name )
{
	unique_lock<mutex> lock(m_mutex);

	if (m_stopped) return;

	auto i = m_messageProcessorMap.find( name );
	if ( i != m_messageProcessorMap.end() ) {
		i->second->stop();
		i->second->join();
		m_messageProcessorMap.erase(i);
	} else {
		throw Primitives2Exception("Name not found");
	}
}

bool MessageRouter::sendMessage( shared_ptr<Message> msg, bool createClientConnection )
{
	unique_lock<mutex> lock(m_mutex);

	if (m_stopped) return(false);

	string adrPortStr = Message::getAdrPortFromUrl( msg->m_dstAddress );
	shared_ptr<IpAddress> adr = IpAddress::create(adrPortStr);
	auto i = m_messageClientSocketMap.find(adrPortStr);

	if (createClientConnection) {
		if ( i != m_messageClientSocketMap.end() )
			throw Primitives2Exception("Client connection exists");
		shared_ptr<MessageClientSocket> s;
		s.reset( new MessageClientSocket(m_messageReceived) );
		s->start( *adr );
		m_messageClientSocketMap[adrPortStr] = s;
		return( s->write( *msg ) );
	}

	if ( i != m_messageClientSocketMap.end() ) {
		return( i->second->write( *msg ) );
	}

	for ( auto j : m_messageServerSocketMap ) {

		unique_lock<MessageServerSocket> mss_lock( *j.second );

		if ( j.second->find_unlocked(*adr) ) {
			return(j.second->write_unlocked( *adr, *msg ));
		}
	}

	Logger::logInfo("MessageRouter::sendMessage() : Unable to send message");

	return( false );
}

bool MessageRouter::closeClientConnection( const IpAddress & adr )
{
	unique_lock<mutex> lock(m_mutex);

	string adrPortStr = adr.getAddressPortString();
	auto i = m_messageClientSocketMap.find(adrPortStr);
	if ( i == m_messageClientSocketMap.end() )
		return( false );

	i->second->stop();
	i->second->join();
	m_messageClientSocketMap.erase(i);

	return( true );
}

bool MessageRouter::closeServerConnection( const IpAddress & servAdr,
						const IpAddress & connAdr )
{
	unique_lock<mutex> lock(m_mutex);

	string adrPortStr = servAdr.getAddressPortString();
	auto i = m_messageServerSocketMap.find(adrPortStr);
	if ( i == m_messageServerSocketMap.end() )
		throw Primitives2Exception("Address not found");

	unique_lock<MessageServerSocket> mss_lock( *i->second );

	bool found = i->second->find_unlocked(connAdr);

	if (found) i->second->stop_unlocked(connAdr);

	return(found);
}

vector<shared_ptr<IpAddress> >
MessageRouter::getServerSocketAddressList()
{
	unique_lock<mutex> lock(m_mutex);

	vector<shared_ptr<IpAddress> > list;

	for ( auto & i : m_messageServerSocketMap ) {
		list.push_back( IpAddress::create( i.first ) );
	}

	return( list );
}

void MessageRouter::createServerSocket( const IpAddress & servAdr )
{
	unique_lock<mutex> lock(m_mutex);

	if (m_stopped) return;

	shared_ptr<MessageServerSocket> s;
	s.reset( new MessageServerSocket(m_messageReceived) );
	s->start( servAdr );
	string adrPortStr = servAdr.getAddressPortString();
	m_messageServerSocketMap[adrPortStr] = s;
}

void MessageRouter::removeServerSocket( const IpAddress & servAdr )
{
	unique_lock<mutex> lock(m_mutex);

	if (m_stopped) return;

	string adrPortStr = servAdr.getAddressPortString();
	auto i = m_messageServerSocketMap.find( adrPortStr );
	if ( i != m_messageServerSocketMap.end() ) {
		i->second->stop();
		i->second->join();
		m_messageServerSocketMap.erase(i);
	} else {
		throw Primitives2Exception("Address not found");
	}
}

void MessageRouter::start()
{
	unique_lock<mutex> lock(m_mutex);

	m_cleanupThread.reset( new thread(cleanupClientConnections,this) );
}

void MessageRouter::stop()
{
	unique_lock<mutex> lock(m_mutex);

	m_stopped = true;

	for ( auto i : m_messageProcessorMap ) i.second->stop();
	for ( auto i : m_messageClientSocketMap ) i.second->stop();
	for ( auto i : m_messageServerSocketMap ) i.second->stop();
}

void MessageRouter::join()
{
	for ( auto i : m_messageProcessorMap ) i.second->join();
	for ( auto i : m_messageServerSocketMap ) i.second->join();

	if (m_cleanupThread) m_cleanupThread->join();
}

bool MessageRouter::messageReceived( EventData & data )
{
	MessageData & d = dynamic_cast<MessageData &>(data);

	string name = Message::getNameFromUrl( d.m_msg->m_dstAddress );

	auto i = m_messageProcessorMap.find(name);
	if ( i != m_messageProcessorMap.end() ) {
		i->second->addMessage(d.m_msg);
	} else {
		Logger::logInfo("MessageRouter::messageReceived() : Name not found");
	}

	return( true );
}

void MessageRouter::cleanupClientConnections( MessageRouter * self )
{
	try {
		uint64_t count = 0;
		while ( true ) {
			this_thread::sleep_for( chrono::milliseconds(1000) );
			{
				unique_lock<mutex> lock(self->m_mutex);
				if ( self->m_stopped ) break;
			}
			if ( ( count % 8 ) == 0 ) self->cleanup();
			count++;
		}
		while ( true ) {
			self->cleanup();
			{
				unique_lock<mutex> lock(self->m_mutex);
				if ( self->m_messageClientSocketMap.size() == 0 ) break;
			}
			this_thread::sleep_for( chrono::milliseconds(1000) );
		}
	}
	catch ( const exception e ) {
		cout << "MessageRouter::cleanupClientConnections() : exception\n";
	}
	catch ( ... ) {
		cout << "MessageRouter::cleanupClientConnections() : caught in ...\n";
	}
}

void MessageRouter::cleanup()
{
	unique_lock<mutex> lock(m_mutex);

	auto i = m_messageClientSocketMap.begin();
	auto i0 = i;

	while ( i != m_messageClientSocketMap.end() ) {
		i0 = i;
		i++;
		if ( i0->second->isStopped() ) {
			i0->second->stop();
			i0->second->join();
			m_messageClientSocketMap.erase(i0);
		}
	}
}

/////////////////////////////////////////////////////////////////////

