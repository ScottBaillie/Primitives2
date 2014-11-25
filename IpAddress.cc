/////////////////////////////////////////////////////////////////////
//
// IpAddress.cc
//
/////////////////////////////////////////////////////////////////////

#include <memory>

#include "IpAddress.h"
#include "String.h"
#include "Exception.h"

using namespace std;

/////////////////////////////////////////////////////////////////////

IpAddress::~IpAddress()
{
}

IpAddress::Type IpAddress::getType() const
{
	return(m_type);
}

shared_ptr<IpAddress> IpAddress::create( const string & adrPortStr )
{
	bool isIpv4 = true;

	if ( Str::match( adrPortStr, ".*:.*:.*" ) ) isIpv4 = false;

	if ( isIpv4 ) {
		return( shared_ptr<IpAddress>( new Ipv4Address(adrPortStr) ) );
	} else {
		return( shared_ptr<IpAddress>( new Ipv6Address(adrPortStr) ) );
	}
}

/////////////////////////////////////////////////////////////////////

Ipv4Address::Ipv4Address( const sockaddr_in & adr )
{
	m_type = eIPv4;
	set( adr );
}

Ipv4Address::Ipv4Address( const std::string & adr, uint16_t port )
{
	m_type = eIPv4;
	m_adr.sin_family = AF_INET;
	setAddress( adr );
	setPort( port );
}

Ipv4Address::Ipv4Address( const std::string & adrPortStr )
{
	m_type = eIPv4;
	m_adr.sin_family = AF_INET;
	setAddressPort( adrPortStr );
}

Ipv4Address::~Ipv4Address()
{
}

void Ipv4Address::setAddressPort( const std::string & adrPortStr )
{
	vector<string> tokens = Str::getToken( adrPortStr, ":", "[^:]+" );
	if ( tokens.size() != 2 )
		throw Primitives2Exception("Ipv4Address::setAddressPort() : Invalid address");
	uint16_t port = stoul( tokens[1] );
	setAddress( tokens[0] );
	setPort( port );
}

void Ipv4Address::setAddress( const string & adr )
{
	if ( ::inet_aton( adr.c_str(), &m_adr.sin_addr ) == 0 )
		throw Primitives2Exception("Ipv4Address::setAddress() : Error from inet_aton()");
}

void Ipv4Address::setAddress( const in_addr & adr )
{
	m_adr.sin_addr = adr;
}

void Ipv4Address::setAddress( uint32_t adr, bool convertToNetworkOrder )
{
	if ( convertToNetworkOrder )
		m_adr.sin_addr.s_addr = ::htonl( adr );
	else
		m_adr.sin_addr.s_addr = adr;
}

void Ipv4Address::setPort( uint16_t port, bool convertToNetworkOrder )
{
	if ( convertToNetworkOrder )
		m_adr.sin_port = ::htons( port );
	else
		m_adr.sin_port = port;
}

void Ipv4Address::set( const sockaddr_in & adr )
{
	m_adr = adr;
}

std::string Ipv4Address::getAddressString() const
{
	char * p = ::inet_ntoa( m_adr.sin_addr );

	if ( p == 0 ) throw Primitives2Exception(
		"Ipv4Address::getAddressString() : Error from inet_ntoa()");

	string str(p);
	return( str );
}

uint32_t Ipv4Address::getAddress( bool hostOrder ) const
{
	if ( hostOrder )
		return( ::ntohl( m_adr.sin_addr.s_addr ) );
	else
		return( m_adr.sin_addr.s_addr );
}

uint16_t Ipv4Address::getPort( bool hostOrder ) const
{
	if ( hostOrder )
		return( ::ntohs( m_adr.sin_port ) );
	else
		return( m_adr.sin_port );
}

string Ipv4Address::getAddressPortString() const
{
	string str = getAddressString();
	uint16_t port = getPort();
	str += ":";
	str += to_string( port );
	return( str );
}

struct sockaddr_in Ipv4Address::get() const
{
	return( m_adr );
}

/////////////////////////////////////////////////////////////////////

Ipv6Address::Ipv6Address( const std::string & adrPortStr )
{
	m_type = eIPv6;
}

Ipv6Address::~Ipv6Address()
{
}

string Ipv6Address::getAddressPortString() const
{
	return( "" );
}

/////////////////////////////////////////////////////////////////////

