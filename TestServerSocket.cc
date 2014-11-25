/////////////////////////////////////////////////////////////////////
//
// TestServerSocket.cc
//
/////////////////////////////////////////////////////////////////////

#include <unistd.h>

#include <stdexcept>
#include <iostream>

#include "TestServerSocket.h"
#include "Exception.h"

using namespace std;

/////////////////////////////////////////////////////////////////////

TestServerSocket::TestServerSocket() : m_serverSocketThread(*this)
{
}

const char * g_message_wr =
		"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ"
		"0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZ";

const char * g_message_rd =
		"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
		"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
		"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
		"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
		"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
		"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
		"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
		"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
		"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
		"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
		"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
		"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
		"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
		"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
		"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
		"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
		"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
		"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
		"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
		"abcdefghijklmnopqrstuvwxyzabcdefghijklmnopqrstuvwxyz"
		"#";


uint64_t g_acceptCounter = 0;
mutex g_acceptCounterMutex;


void TestServerSocket::runTest()
{
	Ipv4Address ips4("127.0.0.1",50000);
	m_serverSocketThread.start( ips4, SocketBase::eStream );

	Buffer wr_buf = g_message_wr;
	Buffer rd_buf( string(g_message_rd).size() );
	Ipv4Address ip4("127.0.0.1",50000);
	uint32_t numSockets = 200;
	vector<shared_ptr<Socket> > vs(numSockets);
	bool ret;

	for ( auto & i : vs ) i.reset( new Socket( ip4.getType(), Socket::eStream ) );
	for ( auto i   : vs ) {
		ret = i->connect( ip4 );
		if (!ret) {
			cout << "TestServerSocket::runTest() : connect() has been stopped\n";
			return;
		}
	}
	for ( auto i   : vs ) {
		ret = i->write( wr_buf );
		if (!ret) {
			cout << "TestServerSocket::runTest() : write() has been stopped\n";
			return;
		}
	}

	uint64_t index;
	uint64_t length;
	uint64_t numbytes;

	for ( auto i : vs ) {
		index = 0;
		length = rd_buf.size();
		rd_buf.fill(0);

		ret = i->readWait( rd_buf );
		if (!ret) {
			cout << "TestServerSocket::runTest() : readWait() has been stopped\n";
			return;
		}
		if ( rd_buf != Buffer(g_message_rd) ) {
			cout << "TestServerSocket::runTest() : data has wrong value\n";
			return;
		}
	}

	m_serverSocketThread.stop();
	m_serverSocketThread.join();

	if ( g_acceptCounter == numSockets ) {
		cout << "Test Passed\n";
	} else {
		cout << "Test Failed : g_acceptCounter=" << g_acceptCounter << "\n";
	}
}

/////////////////////////////////////////////////////////////////////

bool TestServerSocket::event( EventData & data )
{
	SocketEventData & sdata = dynamic_cast<SocketEventData &>(data);
	Buffer wr_buf = g_message_rd;
	Buffer rd_buf( string(g_message_wr).size() );
	rd_buf.fill(0);

	uint64_t index = 0;
	uint64_t length = rd_buf.size();
	uint64_t numbytes;
	bool ret;

	ret = sdata.m_s->readWait( rd_buf );
	if (!ret) {
		cout << "TestServerSocket::event() : readWait() has been stopped\n";
		return(false);
	}

	if ( rd_buf != Buffer(g_message_wr) ) {
		cout << "TestServerSocket::event() : read() data has wrong value\n";
		return(false);
	}

	ret = sdata.m_s->write( wr_buf );
	if (!ret) {
		cout << "TestServerSocket::event() : write() has been stopped\n";
		return(false);
	}

	{
	unique_lock<mutex> lock(g_acceptCounterMutex);
	g_acceptCounter++;
	}
	return(true);
}

