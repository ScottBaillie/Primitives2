/////////////////////////////////////////////////////////////////////
//
// TestServerSocket2.cc
//
/////////////////////////////////////////////////////////////////////

#include <unistd.h>

#include <stdexcept>
#include <iostream>

#include "TestServerSocket2.h"
#include "Exception.h"

using namespace std;

/////////////////////////////////////////////////////////////////////

const uint64_t BUFF_SIZE = 16384;

/////////////////////////////////////////////////////////////////////

TestServerSocket2::TestServerSocket2() : m_serverSocketThread(*this)
{
}

void TestServerSocket2::runTest()
{
	Ipv4Address ip4("127.0.0.1",50001);

	m_serverSocketThread.start( ip4, SocketBase::eStream );

	Socket s( ip4.getType(), Socket::eStream );

	bool ret = s.connect( ip4 );
	if (!ret) {
		exitTest("TestServerSocket2::runTest() : connect() has been stopped");
		return;
	}

////////////////////

	Buffer wr_buf(BUFF_SIZE);
	wr_buf.fill( 0x55 );

	ret = s.write( wr_buf );
	if (!ret) {
		exitTest("TestServerSocket2::runTest() : write() has been stopped");
		return;
	}

/////////////////////

	Buffer rd_buf(BUFF_SIZE);
	rd_buf.fill( 0 );

	ret = s.readWait( rd_buf );
	if (!ret) {
		exitTest("TestServerSocket2::runTest() : readWait() has been stopped");
		return;
	}

	Buffer rd_buf2(BUFF_SIZE);
	rd_buf2.fill( 0xaa );

	if ( rd_buf != rd_buf2 ) {
		exitTest("TestServerSocket2::runTest() : Data mismatch");
		return;
	}

/////////////////////

	m_serverSocketThread.stop();
	m_serverSocketThread.join();
}

/////////////////////////////////////////////////////////////////////

bool TestServerSocket2::event( EventData & data )
{
	SocketEventData & sdata = dynamic_cast<SocketEventData &>(data);

/////////////////////

	Buffer rd_buf(BUFF_SIZE);
	rd_buf.fill( 0 );

	bool ret = sdata.m_s->readWait( rd_buf );
	if (!ret) {
		cout << "TestServerSocket2::event() : readWait() has been stopped\n";
		return(false);
	}

	Buffer rd_buf2(BUFF_SIZE);
	rd_buf2.fill( 0x55 );

	if ( rd_buf != rd_buf2 ) {
		cout << "TestServerSocket2::event() : Data mismatch\n";
		return(false);
	}

/////////////////////

	Buffer wr_buf(BUFF_SIZE);
	wr_buf.fill( 0xaa );

	ret = sdata.m_s->write( wr_buf );
	if (!ret) {
		cout << "TestServerSocket2::event() : write() has been stopped\n";
		return(false);
	}
	return(true);
}

void TestServerSocket2::exitTest( const string & msg )
{
	cout << msg << "\n";
	m_serverSocketThread.stop();
	m_serverSocketThread.join();
}

