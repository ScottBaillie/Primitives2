#ifndef PRIMITIVES2_TESTSERVERSOCKET_H
#define PRIMITIVES2_TESTSERVERSOCKET_H
/////////////////////////////////////////////////////////////////////
//
// TestServerSocket.h
//
/////////////////////////////////////////////////////////////////////

#include <tuple>
#include <memory>
#include <thread>
#include <forward_list>
#include "ServerSocket.h"
#include "ServerSocketThread.h"

/////////////////////////////////////////////////////////////////////

class TestServerSocket : public EventHandler {
public:

	TestServerSocket();

	void runTest();

private:
	virtual bool event( EventData & data );

private:

	ServerSocketThread m_serverSocketThread;
};

/////////////////////////////////////////////////////////////////////

#endif
