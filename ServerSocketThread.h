#ifndef PRIMITIVES2_SERVERSOCKETTHREAD_H
#define PRIMITIVES2_SERVERSOCKETTHREAD_H
/////////////////////////////////////////////////////////////////////
//
// ServerSocketThread.h
//
/////////////////////////////////////////////////////////////////////

#include <thread>
#include <mutex>
#include <unordered_map>
#include "ServerSocket.h"
#include "Event.h"

/////////////////////////////////////////////////////////////////////

class SocketEventData : public EventData {
public:
	SocketEventData( std::shared_ptr<Socket> s ) : m_s(s) {}

	std::shared_ptr<Socket> m_s;
};

/////////////////////////////////////////////////////////////////////

class ServerSocketThread {
public:
	ServerSocketThread( EventHandler & hlr );

	void			start( const IpAddress & adr, SocketBase::Type type );

	void			stop();
	void			join();

	void			lock();
	void			unlock();

	bool			find_unlocked( const IpAddress & adr );
	bool			write_unlocked( const IpAddress & adr, const Buffer & buf );
	void			close_unlocked( const IpAddress & adr );

	std::shared_ptr<Socket>	getSocket_unlocked( const IpAddress & adr );

	std::vector<std::shared_ptr<IpAddress> >
				getAddressList_unlocked();

private:
	void			listenThreadFunction();
	void			acceptThreadFunction( EventData & data );

	static void		listenThreadFunctionStatic( ServerSocketThread * self );
	static void		acceptThreadFunctionStatic( ServerSocketThread * self, std::shared_ptr<Socket> s, const std::string & adrPortStr );
	static void		cleanupThreadFunctionStatic( ServerSocketThread * self );

	void			cleanup();

private:

	struct ConnectionInfo {
		std::shared_ptr<std::thread>	m_thread;
		std::shared_ptr<Socket>		m_socket;
		bool				m_terminated = false;
	};

	bool			m_stopped;

	EventHandler &		m_hlr;

	ServerSocket 		m_serverSocket;

	std::shared_ptr<std::thread>
				m_listenThread;

	std::shared_ptr<std::thread>
				m_cleanupThread;

	std::mutex		m_connectionMapMutex;

	std::unordered_map<std::string,ConnectionInfo>
				m_connectionMap;
};

/////////////////////////////////////////////////////////////////////

#endif
