#ifndef PRIMITIVES2_BUFFER_H
#define PRIMITIVES2_BUFFER_H
/////////////////////////////////////////////////////////////////////
//
// Buffer.h
//
/////////////////////////////////////////////////////////////////////

#include <string>
#include <vector>

/////////////////////////////////////////////////////////////////////


class Buffer {
public:

  Buffer();
  Buffer( uint8_t * ptr, uint64_t size );
  Buffer( const char * str );
  Buffer( const std::string & str );
  Buffer( const std::vector<uint8_t> & vec );
  Buffer( const Buffer & buf );
  Buffer( Buffer && buf );
  Buffer( uint64_t size );

  ~Buffer();

  uint64_t size() const;

  void resize( uint64_t size );

  Buffer & operator = ( const char * str );
  Buffer & operator = ( const std::string & str );
  Buffer & operator = ( const std::vector<uint8_t> & vec );
  Buffer & operator = ( const Buffer & buf );
  Buffer & operator = ( Buffer && buf );

  void fill( uint8_t value );

  uint16_t & getValue16( uint64_t index );
  uint32_t & getValue32( uint64_t index );
  uint64_t & getValue64( uint64_t index );
  std::string getString( uint64_t index, uint64_t length );
  std::string getString();

  Buffer getSubBuffer( uint64_t index, uint64_t length ) const;
  Buffer getSubBuffer( uint64_t index ) const;

  const uint8_t & operator [] ( uint64_t index ) const;
  uint8_t & operator [] ( uint64_t index );

  bool operator ! () const;
  bool operator == ( const Buffer & buf ) const;
  bool operator != ( const Buffer & buf ) const;

private:
  void init();

private:

  uint8_t *	m_ptr;
  uint64_t	m_size;
  bool		m_own;
};

Buffer operator + ( const Buffer & buf1, const Buffer & buf2 );

/////////////////////////////////////////////////////////////////////

#endif
