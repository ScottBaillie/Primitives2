/////////////////////////////////////////////////////////////////////
//
// Socket.cc
//
/////////////////////////////////////////////////////////////////////

#include <unistd.h>
#include <fcntl.h>

#include "Socket.h"
#include "String.h"
#include "Exception.h"

using namespace std;

/////////////////////////////////////////////////////////////////////

SocketBase::SocketBase()
{
	init();
}

SocketBase::~SocketBase()
{
	close();
}

void SocketBase::open( IpAddress::Type adrType, Type sockType )
{
	if ( m_state != eInitial ) throw Primitives2Exception(
		"SocketBase::open() : Wrong state");

	int domain = PF_INET;
	int type = SOCK_STREAM;
	int protocol = 0;

	if ( adrType == IpAddress::eIPv6 )
		domain = PF_INET6;

	if ( sockType == eDatagram ) type = SOCK_DGRAM;

	int fd = ::socket( domain, type, protocol );

	if ( fd < 0 ) throw Primitives2Exception(
		"SocketBase::open() : Error from socket()");

	int oldflags = ::fcntl( fd, F_GETFL, 0 );

	if ( oldflags < 0 ) throw Primitives2Exception(
		"SocketBase::open() : Error from fcntl()");

	oldflags |= O_NONBLOCK ;

	int ret = ::fcntl( fd, F_SETFL, oldflags );

	if ( ret < 0 ) throw Primitives2Exception(
		"SocketBase::open() : Error from fcntl()");

	m_adrType = adrType;
	m_sockType = sockType;
	m_fd = fd;
	m_state = eOpen;
}

IpAddress::Type SocketBase::getAdrType() const
{
	if ( m_state == eInitial ) throw Primitives2Exception(
		"SocketBase::getAdrType() : Wrong state");

	return(m_adrType);
}

SocketBase::Type SocketBase::getSockType() const
{
	if ( m_state == eInitial ) throw Primitives2Exception(
		"SocketBase::getSockType() : Wrong state");

	return(m_sockType);
}

void SocketBase::close()
{
	if ( m_fd == -1 ) return;
	int res = ::close( m_fd );
	init();
	if ( res < 0 ) throw Primitives2Exception(
		"SocketBase::close() : Error from close()");
}

bool SocketBase::isClosed() const
{
	if ( m_fd == -1 ) return( true );
	return( false );
}

void SocketBase::stop()
{
	m_stopped = true;
}

void SocketBase::init()
{
	m_adrType = IpAddress::eIPv4;
	m_sockType = eStream;
	m_fd = -1;
	m_stopped = false;
	m_state = eInitial;
	m_connectedAddress.reset();
	m_acceptedAddress.reset();
	m_boundAddress.reset();
}

void SocketBase::move( SocketBase && s )
{
	close();

	m_fd = std::move( s.m_fd );
	m_stopped = std::move( s.m_stopped );
	m_state = std::move( s.m_state );
	m_connectedAddress = std::move( s.m_connectedAddress );
	m_acceptedAddress = std::move( s.m_acceptedAddress );
	m_boundAddress = std::move( s.m_boundAddress );

	s.init();
}

/////////////////////////////////////////////////////////////////////

Socket::Socket()
{
}

Socket::Socket( int fd, shared_ptr<IpAddress> acceptedAddress )
{
	if ( fd < 0 ) throw Primitives2Exception(
		"Socket::Socket() : Invalid file descriptor");

	m_fd = fd;
	m_state = eReady;
	m_acceptedAddress = acceptedAddress;
}

Socket::Socket( Socket && s )
{
	*this = s;
}

Socket::Socket( IpAddress::Type adrType, Type sockType )
{
	open( adrType, sockType );
}

bool Socket::connect( const IpAddress & adr )
{
	if ( m_stopped ) return( false );

	if ( adr.getType() != getAdrType() ) throw Primitives2Exception(
		"Socket::connect() : Wrong address type");

	if ( m_state != eOpen ) throw Primitives2Exception(
		"Socket::connect() : Wrong state");

	const Ipv4Address & adr4 = dynamic_cast<const Ipv4Address&>(adr);
	struct sockaddr_in in = adr4.get();

	int res = ::connect( m_fd, (const sockaddr*)&in, sizeof(struct sockaddr_in) );
	int err = errno;

	if ( m_stopped ) return( false );

	if ( res == 0 ) {
		m_state = eReady;
		return( true );
	}
	
	if ( res < 0 ) {
		if ( err != EINPROGRESS ) throw Primitives2Exception(
			"Socket::connect() : Error from connect()");
	}

	int numFdReady;

	while ( true ) {

		struct timeval timeout;
		timeout.tv_sec = 0;
		timeout.tv_usec = 500000;

		fd_set set;
	
		FD_ZERO( &set );
		FD_SET( m_fd, &set );

		numFdReady = ::select( FD_SETSIZE, NULL, &set, NULL, &timeout );
		err = errno;
		
		if ( m_stopped ) return( false );
		
		if ( numFdReady < 0 ) {
			if ( err == EINTR ) return( false );
			throw Primitives2Exception(
				"Socket::connect() : Error from select()");
		}

		if ( numFdReady > 0 ) break;
	}

	m_connectedAddress.reset( new Ipv4Address( adr4 ) );

	m_state = eReady;
	return( true );
}

bool Socket::read( Buffer & buf, uint64_t & numbytes )
{
	if ( m_stopped ) return( false );

	uint64_t index = 0;
	uint64_t length = buf.size();

	if ( m_state != eReady ) throw Primitives2Exception(
		"Socket::read() : Wrong state");

	if ( length == 0 ) throw Primitives2Exception(
		"Socket::read() : Zero length");

	ssize_t nbytes;
	int err;
	int numFdReady;
	uint64_t length0 = length;

	while ( true ) {

		nbytes = ::read( m_fd, &buf[index], length );
		err = errno;

		if ( m_stopped ) return( false );
		
		if ( nbytes == 0 ) {
			if ( index == 0 ) return( false );
			numbytes = index;
			return( true );
		}
		
		if ( nbytes < 0 ) {
			if ( err == EINTR ) return( false );
			if ( err != EAGAIN ) throw Primitives2Exception(
				"Socket::read() : Error from read()");
			nbytes = 0;
		}

		index += nbytes;
		length -= nbytes;

		if ( length == 0 ) {
			numbytes = index;
			return( true );
		}

		uint32_t count = 0;
		uint32_t COUNT_MAX_READ = 2;

		while ( true ) {

			struct timeval timeout;
			timeout.tv_sec = 0;
			timeout.tv_usec = 500000;

			fd_set set;
		
			FD_ZERO( &set );
			FD_SET( m_fd, &set );

			numFdReady = ::select( FD_SETSIZE, &set, NULL, NULL, &timeout );
			err = errno;
			
			if ( m_stopped ) return( false );
			
			if ( numFdReady < 0 ) {
				if ( err == EINTR ) return( false );
				throw Primitives2Exception("Socket::read() : Error from select()");
			}

			if ( numFdReady > 0 ) break;

			// Dont wait in this select loop for ever.
			// If no input arrives , just return data that has already been read.
			count++;
			if ( count == COUNT_MAX_READ ) {
				numbytes = index;
				return( true );
			}
		}
	}

	return( true );
}

bool Socket::readWait( Buffer & buf )
{
	uint64_t size = buf.size();

	if ( size == 0 ) throw Primitives2Exception(
		"Socket::readWait() : Zero size buffer");

	uint64_t index = 0;
	uint64_t numbytes;
	bool ret;

	while ( true ) {
		Buffer sb = buf.getSubBuffer(index);
		ret = read( sb, numbytes );
		if ( ! ret ) return( false );
		if ( m_stopped ) return( false );
		index += numbytes;
		if ( index >= size ) break;
	}

	return( true );
}

bool Socket::write( const Buffer & buf )
{
	if ( m_stopped ) return( false );

	uint64_t index = 0;
	uint64_t length = buf.size();

	if ( m_state != eReady ) throw Primitives2Exception(
		"Socket::write() : Wrong state");

	if ( length == 0 ) throw Primitives2Exception(
		"Socket::write() : Zero length");

	ssize_t nbytes;
	int err;
	int numFdReady;

	while ( true ) {

		nbytes = ::write( m_fd, &buf[index], length );
		err = errno;

		if ( nbytes < 0 ) {
			if ( ( err == EINTR ) || ( err == ECONNRESET ) || ( err == ECONNREFUSED ))
				return( false );
			if ( err != EAGAIN ) throw Primitives2Exception(
				StrParam("Socket::write() : Error from write() : err=")(err));
			nbytes = 0;
		}

		index += nbytes;
		length -= nbytes;

		if ( length == 0 ) return( true );

		if ( m_stopped ) return( false );

		while ( true ) {
			struct timeval timeout;
			timeout.tv_sec = 0;
			timeout.tv_usec = 500000;

			fd_set set;

			FD_ZERO( &set );
			FD_SET( m_fd, &set );

			numFdReady = ::select( FD_SETSIZE, NULL, &set, NULL, &timeout );
			err = errno;

			if ( numFdReady < 0 ) {
				if ( err == EINTR ) return( false );
				throw Primitives2Exception("Socket::write() : Error from select()");
			}

			if ( numFdReady > 0 ) break;

			if ( m_stopped ) return( false );
		}
	}

	return( true );
}

Socket & Socket::operator = ( Socket && s )
{
	move((SocketBase &&)s);
	return(*this);
}

const IpAddress & Socket::getConnectedAddress() const
{
	if (!m_connectedAddress) throw Primitives2Exception("No connected Address");
	return(*m_connectedAddress);
}

const IpAddress & Socket::getAcceptedAddress() const
{
	if (!m_acceptedAddress) throw Primitives2Exception("No accepted Address");
	return(*m_acceptedAddress);
}

/////////////////////////////////////////////////////////////////////

