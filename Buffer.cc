/////////////////////////////////////////////////////////////////////
//
// Buffer.cc
//
/////////////////////////////////////////////////////////////////////

#include <string.h>

#include "Buffer.h"
#include "Exception.h"

using namespace std;

/////////////////////////////////////////////////////////////////////

void Buffer::init()
{
	m_ptr = 0;
	m_size = 0;
	m_own = true;
}

Buffer::Buffer()
{
	init();
}

Buffer::Buffer( uint8_t * ptr, uint64_t size )
{
	if ( (ptr==0) || (size==0) )
		throw Primitives2Exception("Invalid args");

	m_ptr = ptr;
	m_size = size;
	m_own = false;
}

Buffer::Buffer( const char * str )
{
	init();
	*this = str;
}

Buffer::Buffer( const std::string & str )
{
	init();
	*this = str;
}

Buffer::Buffer( const std::vector<uint8_t> & vec )
{
	init();
	*this = vec;
}

Buffer::Buffer( const Buffer & buf )
{
	init();
	*this = buf;
}

Buffer::Buffer( Buffer && buf )
{
	init();
	*this = buf;
}

Buffer::Buffer( uint64_t size )
{
	init();
	resize( size );
}

Buffer::~Buffer()
{
	resize( 0 );
}

uint64_t Buffer::size() const
{
	return( m_size );
}

void Buffer::resize( uint64_t size )
{
	if ( size == 0 ) {
		if ( m_ptr != 0 ) {
			m_size = 0;
			uint8_t * ptr = m_ptr;
			m_ptr = 0;
			if (m_own) delete [] ptr;
			m_own = true;
		}
	} else {
		if ( m_size == 0 ) {
			m_ptr = new uint8_t[ size ];
			m_size = size;
			m_own = true;
			return;
		}
		if ( size != m_size ) {
			if ( m_ptr == 0 )
				throw Primitives2Exception("Buffer has inconsistent state");
			uint8_t * ptr = m_ptr;
			uint64_t min_size = size;
			if ( m_size < min_size ) min_size = m_size;
			m_ptr = new uint8_t[ size ];
			m_size = size;
			::memcpy( m_ptr, ptr, min_size );
			if (m_own) delete [] ptr;
			m_own = true;
		}
	}
}


Buffer & Buffer::operator = ( const char * str )
{
	if ( str == 0 ) throw Primitives2Exception("Null Ptr");
	size_t length = ::strlen( str );
	Buffer buf( (uint8_t*)str, length );
	return( operator=(buf) );
}

Buffer & Buffer::operator = ( const string & str )
{
	size_t length = str.size();
	Buffer buf( (uint8_t*)str.c_str(), length );
	return( operator=(buf) );
}

Buffer & Buffer::operator = ( const vector<uint8_t> & vec )
{
	throw Primitives2Exception("Not impl");
}


Buffer & Buffer::operator = ( const Buffer & buf )
{
	uint64_t size = buf.size();

	if ( size == 0 ) {
		resize(0);
		return( *this );
	}

	if ( m_size != size ) {
		resize( 0 );
		resize( size );
	}

	::memcpy( m_ptr, buf.m_ptr, size );

	return( *this );
}

Buffer & Buffer::operator = ( Buffer && buf )
{
	resize(0);
	m_ptr = std::move( buf.m_ptr );
	m_size = std::move( buf.m_size );
	m_own = std::move( buf.m_own );
	buf.init();
}

void Buffer::fill( uint8_t value )
{
	for ( uint64_t u0 = 0; u0 < m_size; u0++ )
		m_ptr[u0] = value;
}

uint16_t & Buffer::getValue16( uint64_t index )
{
	if ( m_ptr == 0 )
		throw Primitives2Exception("Buffer Not Initialised");

	if ( (index+1) >= m_size )
		throw Primitives2Exception("Index out of range");

	return( *(uint16_t*)&m_ptr[index]  );
}

uint32_t & Buffer::getValue32( uint64_t index )
{
	if ( m_ptr == 0 )
		throw Primitives2Exception("Buffer Not Initialised");

	if ( (index+3) >= m_size )
		throw Primitives2Exception("Index out of range");

	return( *(uint32_t*)&m_ptr[index]  );
}

uint64_t & Buffer::getValue64( uint64_t index )
{
	if ( m_ptr == 0 )
		throw Primitives2Exception("Buffer Not Initialised");

	if ( (index+7) >= m_size )
		throw Primitives2Exception("Index out of range");

	return( *(uint64_t*)&m_ptr[index]  );
}

string Buffer::getString( uint64_t index, uint64_t length )
{
	if ( m_ptr == 0 )
		throw Primitives2Exception("Buffer Not Initialised");

	if ( length == 0 )
		throw Primitives2Exception("Invalid length");

	if ( (index+length-1) >= m_size )
		throw Primitives2Exception("Index out of range");

	return( string((const char *)&m_ptr[index],length) );
}

string Buffer::getString()
{
	return( getString( 0, m_size ) );
}

Buffer Buffer::getSubBuffer( uint64_t index, uint64_t length ) const
{
	if ( m_ptr == 0 )
		throw Primitives2Exception("Buffer Not Initialised");

	if ( length == 0 )
		throw Primitives2Exception("Invalid length");

	if ( (index+length-1) >= m_size )
		throw Primitives2Exception("Index out of range");

	Buffer subBuffer( &m_ptr[index], length );
	return( subBuffer );
}

Buffer Buffer::getSubBuffer( uint64_t index ) const
{
	if ( index >= m_size )
		throw Primitives2Exception("Index out of range");

	uint64_t length = m_size - index;
	return(getSubBuffer(index,length));
}

const uint8_t & Buffer::operator [] ( uint64_t index ) const
{
	if ( m_ptr == 0 )
		throw Primitives2Exception("Buffer Not Initialised");

	if ( index >= m_size )
		throw Primitives2Exception("Index out of range");

	return( m_ptr[index] );
}

uint8_t & Buffer::operator [] ( uint64_t index )
{
	if ( m_ptr == 0 )
		throw Primitives2Exception("Buffer Not Initialised");

	if ( index >= m_size )
		throw Primitives2Exception("Index out of range");

	return( m_ptr[index] );
}

bool Buffer::operator ! () const
{
	return( m_ptr != 0 );
}

bool Buffer::operator == ( const Buffer & buf ) const
{
	if ( m_size != buf.m_size ) return( false );
	if ( m_size == 0 ) return( true );

	int ret = ::memcmp( m_ptr, buf.m_ptr, m_size );

	if ( ret == 0 ) return( true );
	return( false );
}

bool Buffer::operator != ( const Buffer & buf ) const
{
	return( !operator==(buf) );
}

/////////////////////////////////////////////////////////////////////

Buffer operator + ( const Buffer & buf1, const Buffer & buf2 )
{
	uint64_t size1 = buf1.size();
	uint64_t size2 = buf2.size();
	uint64_t size = size1 + size2;

	Buffer res( size );

	if ( size == 0 ) return( res );

	if ( size1 == 0 ) {
		res = buf2;
		return( res );
	}

	if ( size2 == 0 ) {
		res = buf1;
		return( res );
	}

	Buffer sb1 = res.getSubBuffer( 0, size1 );
	Buffer sb2 = res.getSubBuffer( size1 );

	sb1 = buf1;
	sb2 = buf2;

	return( res );
}

/////////////////////////////////////////////////////////////////////


