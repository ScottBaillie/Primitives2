#ifndef PRIMITIVES2_SOCKET_H
#define PRIMITIVES2_SOCKET_H
/////////////////////////////////////////////////////////////////////
//
// Socket.h
//
/////////////////////////////////////////////////////////////////////

#include <memory>

#include "Buffer.h"
#include "IpAddress.h"

/////////////////////////////////////////////////////////////////////

class SocketBase {
public:

	enum Type {
		eStream,
		eDatagram
	};

	virtual ~SocketBase();

	void			open( IpAddress::Type adrType, Type sockType );

	IpAddress::Type		getAdrType() const;
	Type			getSockType() const;

	void			close();

	bool			isClosed() const;

	void			stop();

protected:

	SocketBase();
	void			init();
	void			move( SocketBase && s );

private:

	SocketBase( const SocketBase & s ) {}
	SocketBase &		operator = ( const SocketBase & s ) { return(*this); }

protected:

	enum State {
		eInitial,
		eOpen,
		eReady
	};

	int			m_fd;
	bool			m_stopped;
	State			m_state;
	std::shared_ptr<IpAddress>
				m_connectedAddress;
	std::shared_ptr<IpAddress>
				m_acceptedAddress;
	std::shared_ptr<IpAddress>
				m_boundAddress;

private:
	IpAddress::Type		m_adrType;
	Type			m_sockType;
};

/////////////////////////////////////////////////////////////////////

class Socket : public SocketBase {
public:

  friend class ServerSocket;

  Socket();
  Socket( Socket && s );
  Socket( IpAddress::Type adrType, Type sockType );

  bool				connect( const IpAddress & adr );

  bool				read( Buffer & buf, uint64_t & numbytes );
  bool				readWait( Buffer & buf );

  bool				write( const Buffer & buf );

  Socket &			operator = ( Socket && s );

  const IpAddress &		getConnectedAddress() const;
  const IpAddress &		getAcceptedAddress() const;

private:

  Socket( const Socket & s ) {}
  Socket &			operator = ( const Socket & s ) { return(*this); }

  Socket( int fd, std::shared_ptr<IpAddress> acceptedAddress );

};


/////////////////////////////////////////////////////////////////////

#endif
