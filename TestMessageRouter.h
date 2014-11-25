#ifndef PRIMITIVES2_TESTMESSAGEROUTER_H
#define PRIMITIVES2_TESTMESSAGEROUTER_H
/////////////////////////////////////////////////////////////////////
//
// TestMessageRouter.h
//
/////////////////////////////////////////////////////////////////////

#include "Message.h"

/////////////////////////////////////////////////////////////////////

class TestMessageRouter {
public:

	TestMessageRouter();

	void runClient();

	void runServer();

private:
	bool			messageReceivedClient( EventData & data );
	bool			messageReceivedServer( EventData & data );

private:
	EventRelay<TestMessageRouter>
				m_messageReceivedClient;

	EventRelay<TestMessageRouter>
				m_messageReceivedServer;

	MessageRouter		m_messageRouter;
};

/////////////////////////////////////////////////////////////////////

#endif
