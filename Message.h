#ifndef PRIMITIVES2_MESSAGE_H
#define PRIMITIVES2_MESSAGE_H
/////////////////////////////////////////////////////////////////////
//
// Message.h
//
/////////////////////////////////////////////////////////////////////

#include "ServerSocketThread.h"
#include "DataMap.h"
#include "Event.h"
#include "Queue.h"

/////////////////////////////////////////////////////////////////////


class Message {

public:
	Message();
	Message( const Message & msg );
	Message( Message && msg );

	Message &		operator = ( const Message & msg );
	Message &		operator = ( Message && msg );

	void			clear();

	static std::string	buildUrl( const std::string & address, const std::string & name );
	static std::string	getAdrPortFromUrl( const std::string & url );
	static std::string	getNameFromUrl( const std::string & url );

	static uint64_t		getLength( const Message & msg );
	static uint64_t		toBinary( const Message & msg, Buffer & buf );
	static void		fromBinary( const Buffer & buf, Message & msg );

private:
	static uint64_t		toBinary( const Message & msg, Buffer & buf, bool write );


public:
	// type 0 means close the tcp connection.
	uint64_t		m_type;

	// Used to pair up request and response.
	uint64_t		m_messageId;

	// There can be multiple messages in a message exchange.
	// For example , 2 messages each having the same type and message id
	// but have a sequence number of 0 and 1.
	uint64_t		m_sequenceNum;

	// Gets set automatically by the router when a message is received by a host
	std::string		m_srcAddress;

	// Set by message sender.
	//
	// msg://hostname:port/MsgRxName
	//
	// The port number may be the listen port of a server or it
	// may be the source port of a client connection.
	//
	std::string		m_dstAddress;

	DataMap			m_dataMap;
};


/////////////////////////////////////////////////////////////////////

class MessageData : public EventData {
public:
	std::shared_ptr<Message>	m_msg;
};

/////////////////////////////////////////////////////////////////////

class MessageClientSocket {
public:
	MessageClientSocket( EventHandler & hlr );

	bool			start( const IpAddress & adr );
	void			stop();
	void			join();

	bool			isStopped();

	static uint64_t		getMessage( Socket & s, Buffer & buf, bool & retval, bool allowResize );
	static bool		getMessage( Socket & s, Message & msg );
	static bool		getMessage( EventHandler & hlr, Socket & s, const std::string & srcAddress );

	bool			write( const Message & msg );

private:
	static void		threadFunctionStatic( MessageClientSocket * self );

private:
	bool			m_stopped;
	bool			m_terminated;
	EventHandler &		m_hlr;
	Socket			m_s;
	std::shared_ptr<std::thread>
				m_thread;
};

/////////////////////////////////////////////////////////////////////

class MessageServerSocket {
public:
	MessageServerSocket( EventHandler & hlr );

	void			start( const IpAddress & adr );
	void			stop();
	void			join();

	void			lock();
	void			unlock();

	bool			find_unlocked( const IpAddress & adr );
	bool			write_unlocked( const IpAddress & adr, const Message & msg );
	void			stop_unlocked( const IpAddress & adr );

private:
	bool			acceptConnection( EventData & data );

private:
	EventRelay<MessageServerSocket>
				m_acceptConnection;
	EventHandler &		m_hlr;
	ServerSocketThread	m_serverSocketThread;
};

/////////////////////////////////////////////////////////////////////

class MessageProcessor {
public:
	MessageProcessor( EventHandler & hlr );

	void			start();
	void			stop();
	void			join();

	void			addMessage( std::shared_ptr<Message> msg );

private:
	static void		threadFunctionStatic( MessageProcessor * self );

private:
	EventHandler &		m_hlr;
	std::shared_ptr<std::thread>
				m_thread;
	Queue<Message>		m_messageQueue;
};

/////////////////////////////////////////////////////////////////////

class MessageRouter {

public:
	MessageRouter();

	std::vector<std::string>
				getMessageProcessorNames();

	void			createMessageProcessor( const std::string & name,
					EventHandler & hlr );

	void			removeMessageProcessor( const std::string & name );

	bool			sendMessage( std::shared_ptr<Message> msg,
					bool createClientConnection = false );

	bool			closeClientConnection( const IpAddress & adr );

	bool			closeServerConnection( const IpAddress & servAdr,
					const IpAddress & connAdr );

	std::vector<std::shared_ptr<IpAddress> >
				getServerSocketAddressList();

	void			createServerSocket( const IpAddress & servAdr );
	void			removeServerSocket( const IpAddress & servAdr );

	void			start();
	void			stop();
	void			join();

private:
	bool			messageReceived( EventData & data );

	static void		cleanupClientConnections( MessageRouter * self );

	void			cleanup();

private:
	EventRelay<MessageRouter>
				m_messageReceived;

	std::unordered_map<std::string,std::shared_ptr<MessageProcessor> >
				m_messageProcessorMap;

	std::unordered_map<std::string,std::shared_ptr<MessageClientSocket> >
				m_messageClientSocketMap;

	std::unordered_map<std::string,std::shared_ptr<MessageServerSocket> >
				m_messageServerSocketMap;

	std::mutex		m_mutex;

	bool			m_stopped;

	std::shared_ptr<std::thread>
				m_cleanupThread;
};

/////////////////////////////////////////////////////////////////////

#endif
