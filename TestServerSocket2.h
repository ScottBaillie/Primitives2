#ifndef PRIMITIVES2_TESTSERVERSOCKET2_H
#define PRIMITIVES2_TESTSERVERSOCKET2_H
/////////////////////////////////////////////////////////////////////
//
// TestServerSocket2.h
//
/////////////////////////////////////////////////////////////////////

#include <tuple>
#include <memory>
#include <thread>
#include <forward_list>
#include "ServerSocket.h"
#include "ServerSocketThread.h"

/////////////////////////////////////////////////////////////////////

class TestServerSocket2 : public EventHandler {
public:

	TestServerSocket2();

	void runTest();

private:
	virtual bool event( EventData & data );

	void exitTest( const std::string & msg );

private:

	ServerSocketThread m_serverSocketThread;
};

/////////////////////////////////////////////////////////////////////

#endif
