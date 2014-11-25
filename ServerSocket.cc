/////////////////////////////////////////////////////////////////////
//
// ServerSocket.cc
//
/////////////////////////////////////////////////////////////////////

#include "ServerSocket.h"
#include "Exception.h"

using namespace std;

/////////////////////////////////////////////////////////////////////

ServerSocket::ServerSocket()
{
}

ServerSocket::ServerSocket( ServerSocket && s )
{
	*this = s;
}

ServerSocket::ServerSocket( IpAddress::Type adrType, Type sockType )
{
	open( adrType, sockType );
}

//
// Use address INADDR_ANY to automatically set the host IP address.
// name.sin_addr.s_addr = htonl (INADDR_ANY);
//
void ServerSocket::bind( const IpAddress & adr )
{
	if ( m_state != eOpen ) throw Primitives2Exception(
		"ServerSocket::bind() : Wrong state");

	if ( adr.getType() != getAdrType() ) throw Primitives2Exception(
		"ServerSocket::bind() : Wrong address type");

	const Ipv4Address & adr4 = dynamic_cast<const Ipv4Address&>(adr);

	struct sockaddr_in sadr = adr4.get();

	int ret = ::bind( m_fd, (struct sockaddr *)&sadr, sizeof(struct sockaddr_in) );

	if ( ret < 0 ) throw Primitives2Exception(
		"ServerSocket::bind() : Error from bind");

	listen();

	m_boundAddress.reset( new Ipv4Address( adr4 ) );
	m_state = eReady;
}

tuple<bool, shared_ptr<Socket> > ServerSocket::accept()
{
	if ( m_state != eReady ) throw Primitives2Exception(
		"ServerSocket::accept() : Wrong state");

	int fd;
	int err;
	struct sockaddr_in adr;
	socklen_t length;
	int numFdReady;

	while ( true ) {

		length = sizeof( struct sockaddr_in );

		fd = ::accept( m_fd, (sockaddr*)&adr, &length );
		err = errno;

		if ( fd >= 0 ) break;

		if ( err != EWOULDBLOCK ) throw Primitives2Exception(
			"ServerSocket::accept() : Error from accept");

		while ( true ) {

			struct timeval timeout;
			timeout.tv_sec = 0; 
			timeout.tv_usec = 500000;

			fd_set set_rd;
			fd_set set_wr;
			fd_set set_ex;
		
			FD_ZERO( &set_rd );
			FD_SET( m_fd, &set_rd );

			FD_ZERO( &set_wr );
			FD_SET( m_fd, &set_wr );

			FD_ZERO( &set_ex );
			FD_SET( m_fd, &set_ex );

			numFdReady = ::select( FD_SETSIZE, &set_rd, &set_wr, &set_ex, &timeout );
			err = errno;

			if ( m_stopped ) {
				tuple<bool, shared_ptr<Socket> > ret( false, shared_ptr<Socket>() );
				return( ret );
			}
			
			if ( numFdReady < 0 ) {
				if ( err == EINTR ) {
					tuple<bool, shared_ptr<Socket> > ret( false, shared_ptr<Socket>() );
					return( ret );
				}
				throw Primitives2Exception(
					"ServerSocket::accept : Error from select()");
			}

			if ( numFdReady > 0 ) break;
		}
	}

	shared_ptr<IpAddress> acceptedAddress( new Ipv4Address( adr ) );

	shared_ptr<Socket> s( new Socket( fd, acceptedAddress ) );

	tuple<bool, shared_ptr<Socket> > ret( true, s );

	return( ret );
}

void ServerSocket::listen()
{
	const int SERVER_SOCKET_QUEUE_LENGTH = 1024;
	int queLength = SERVER_SOCKET_QUEUE_LENGTH;

	int ret = ::listen( m_fd, queLength );

	if ( ret < 0 ) throw Primitives2Exception(
		"ServerSocket::listen() : Error from listen()");
}

ServerSocket & ServerSocket::operator = ( ServerSocket && s )
{
	move((SocketBase &&)s);
	return(*this);
}

const IpAddress & ServerSocket::getBoundAddress() const
{
	if (!m_boundAddress) throw Primitives2Exception("No bound Address");
	return(*m_boundAddress);
}

/////////////////////////////////////////////////////////////////////


