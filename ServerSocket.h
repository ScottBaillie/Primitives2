#ifndef PRIMITIVES2_SERVERSOCKET_H
#define PRIMITIVES2_SERVERSOCKET_H
/////////////////////////////////////////////////////////////////////
//
// ServerSocket.h
//
/////////////////////////////////////////////////////////////////////

#include "Socket.h"

/////////////////////////////////////////////////////////////////////

class ServerSocket : public SocketBase {
public:
	ServerSocket();
	ServerSocket( ServerSocket && s );
	ServerSocket( IpAddress::Type adrType, Type sockType );

	void			bind( const IpAddress & adr );

	std::tuple<bool, std::shared_ptr<Socket> >
				accept();

	ServerSocket &		operator = ( ServerSocket && s );

	const IpAddress &	getBoundAddress() const;

private:
	ServerSocket( const ServerSocket & s ) {}

	ServerSocket &		operator = ( const ServerSocket & s ) { return(*this); }

	void			listen();
};

/////////////////////////////////////////////////////////////////////

#endif
