/////////////////////////////////////////////////////////////////////
//
// TestMessageRouter.cc
//
/////////////////////////////////////////////////////////////////////

#include <unistd.h>

#include <stdexcept>
#include <thread>
#include <chrono>
#include <iostream>

#include "Message.h"
#include "Logger.h"
#include "Exception.h"
#include "TestMessageRouter.h"

using namespace std;

/////////////////////////////////////////////////////////////////////

TestMessageRouter::TestMessageRouter() :
	m_messageReceivedClient(*this,&TestMessageRouter::messageReceivedClient),
	m_messageReceivedServer(*this,&TestMessageRouter::messageReceivedServer)
{
}

const string g_serverAddressPort = "127.0.0.1:50000";

void TestMessageRouter::runClient()
{
	m_messageRouter.start();

	m_messageRouter.createMessageProcessor( "ClientMsgProc", m_messageReceivedClient );

	std::shared_ptr<Message> msg( new Message );

	msg->m_type = 1234;
	msg->m_messageId = 2468;
	msg->m_sequenceNum = 7777;
	msg->m_srcAddress = Message::buildUrl( "0.0.0.0:0", "ClientMsgProc" );
	msg->m_dstAddress = Message::buildUrl( g_serverAddressPort, "ServerMsgProc" );

	msg->m_dataMap.createUint8( "uint8_value", 12 );

	m_messageRouter.sendMessage( msg, true );

	this_thread::sleep_for( chrono::milliseconds(16*1000) );

	m_messageRouter.stop();
	m_messageRouter.join();
}

void TestMessageRouter::runServer()
{
	m_messageRouter.start();

	m_messageRouter.createMessageProcessor( "ServerMsgProc", m_messageReceivedServer );
	m_messageRouter.createServerSocket( *IpAddress::create(g_serverAddressPort) );

	this_thread::sleep_for( chrono::milliseconds(16*1000) );

	m_messageRouter.stop();
	m_messageRouter.join();
}

bool TestMessageRouter::messageReceivedClient( EventData & data )
{
	MessageData & d = dynamic_cast<MessageData &>(data);

	uint8_t val = d.m_msg->m_dataMap.getUint8("uint8_value");

	cout <<
		"Client : m_type=" <<
		d.m_msg->m_type <<
		" : m_messageId=" <<
		d.m_msg->m_messageId <<
		" : m_sequenceNum=" <<
		d.m_msg->m_sequenceNum <<
		" : m_srcAddress=" <<
		d.m_msg->m_srcAddress <<
		" : m_dstAddress=" <<
		d.m_msg->m_dstAddress <<
		" : val=" <<
		(uint32_t)val <<
		"\n";

}

bool TestMessageRouter::messageReceivedServer( EventData & data )
{
	MessageData & d = dynamic_cast<MessageData &>(data);

	uint8_t val = d.m_msg->m_dataMap.getUint8("uint8_value");

	cout <<
		"Server : m_type=" <<
		d.m_msg->m_type <<
		" : m_messageId=" <<
		d.m_msg->m_messageId <<
		" : m_sequenceNum=" <<
		d.m_msg->m_sequenceNum <<
		" : m_srcAddress=" <<
		d.m_msg->m_srcAddress <<
		" : m_dstAddress=" <<
		d.m_msg->m_dstAddress <<
		" : val=" <<
		(uint32_t)val <<
		"\n";

	string s = d.m_msg->m_dstAddress;
	d.m_msg->m_dstAddress = d.m_msg->m_srcAddress;
	d.m_msg->m_srcAddress = s;

	m_messageRouter.sendMessage( d.m_msg );

	return( true );
}

/////////////////////////////////////////////////////////////////////

