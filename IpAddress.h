#ifndef PRIMITIVES2_IPADDRESS_H
#define PRIMITIVES2_IPADDRESS_H
/////////////////////////////////////////////////////////////////////
//
// IpAddress.h
//
/////////////////////////////////////////////////////////////////////

#include <arpa/inet.h>

/////////////////////////////////////////////////////////////////////
//
//struct sockaddr_in6 {
//    sa_family_t     sin6_family;   /* AF_INET6 */
//    in_port_t       sin6_port;     /* port number */
//    uint32_t        sin6_flowinfo; /* IPv6 flow information */
//    struct in6_addr sin6_addr;     /* IPv6 address */
//    uint32_t        sin6_scope_id; /* Scope ID (new in 2.4) */
//};
//
//struct in6_addr {
//    unsigned char   s6_addr[16];   /* IPv6 address */
//};
//
/////////////////////////////////////////////////////////////////////
//
//struct sockaddr_in {
//    sa_family_t    sin_family; /* address family: AF_INET */
//    in_port_t      sin_port;   /* port in network byte order */
//    struct in_addr sin_addr;   /* internet address */
//};
//
///* Internet address. */
//struct in_addr {
//    uint32_t       s_addr;     /* address in network byte order */
//};
//
/////////////////////////////////////////////////////////////////////
//
//struct sockaddr_in   in4;
//struct sockaddr_in6  in6;
//struct sockaddr *    p4 = &in4;
//struct sockaddr *    p6 = &in6;
//
/////////////////////////////////////////////////////////////////////

class IpAddress {
public:

  enum Type {
    eIPv4,
    eIPv6
  };

  virtual ~IpAddress();

  Type getType() const;

  virtual std::string getAddressPortString() const = 0;

  static std::shared_ptr<IpAddress> create( const std::string & adrPortStr );

protected:

  IpAddress() {}

private:

  IpAddress & operator = ( const IpAddress & s ) { return(*this); }

protected:

  Type m_type;
};

//////////////////////////////////////

class Ipv4Address : public IpAddress {
public:

  Ipv4Address( const sockaddr_in & adr );
  Ipv4Address( const std::string & adr, uint16_t port );
  Ipv4Address( const std::string & adrPortStr = "0.0.0.0:0" );

  ~Ipv4Address();

  void setAddressPort( const std::string & adrPortStr );

  void setAddress( const std::string & adr );
  void setAddress( const in_addr & adr );
  void setAddress( uint32_t adr, bool convertToNetworkOrder = true );

  void setPort( uint16_t port, bool convertToNetworkOrder = true );

  void set( const sockaddr_in & adr );

  std::string getAddressString() const;
  uint32_t getAddress( bool hostOrder = true ) const;

  uint16_t getPort( bool hostOrder = true ) const;

  virtual std::string getAddressPortString() const;

  struct sockaddr_in get() const;

private:
  struct sockaddr_in m_adr;
};

//////////////////////////////////////

class Ipv6Address : public IpAddress {
public:

  Ipv6Address( const std::string & adrPortStr = "1::1.0" );

  ~Ipv6Address();

  virtual std::string getAddressPortString() const;

private:
  struct sockaddr_in6 m_adr;

};

/////////////////////////////////////////////////////////////////////

#endif
