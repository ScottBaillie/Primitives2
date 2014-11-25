/////////////////////////////////////////////////////////////////////
//
// DataField.cc
//
/////////////////////////////////////////////////////////////////////

#include "DataField.h"
#include "Exception.h"

using namespace std;

/////////////////////////////////////////////////////////////////////

DataField::DataField()
{
	m_type = eUint8;
}

DataField::DataField( const DataField & f )
{
	m_type = eUint8;
	*this = f;
}

DataField::DataField( DataField && f )
{
	m_type = eUint8;
	*this = f;
}

DataField & DataField::operator = ( const DataField & f )
{
	m_numericValue = f.m_numericValue;
	m_stringValue = f.m_stringValue;
	m_dimensions = f.m_dimensions;
	m_type = f.m_type;
	return( *this );
}

DataField & DataField::operator = ( DataField && f )
{
	m_numericValue = std::move( f.m_numericValue );
	m_stringValue = std::move( f.m_stringValue );
	m_dimensions = std::move( f.m_dimensions );
	m_type = std::move( f.m_type );
	return( *this );
}

template<typename T> void DataField::initNumeric(
				const std::vector<uint64_t> & dim,
				T value )
{
	uint64_t size = getDimensionProduct( dim );
	m_dimensions = dim;
	m_numericValue.resize( size * sizeof( T ) );
	T * p = (T *)&m_numericValue[0];
	for ( uint64_t u0 = 0; u0 < size; u0++ ) p[u0] = value;
}

void DataField::initUint8( const vector<uint64_t> & dim, uint8_t value )
{
	m_type = eUint8;
	initNumeric( dim, value );
}

void DataField::initUint16( const vector<uint64_t> & dim, uint16_t value )
{
	m_type = eUint16;
	initNumeric( dim, value );
}

void DataField::initUint32( const vector<uint64_t> & dim, uint32_t value )
{
	m_type = eUint32;
	initNumeric( dim, value );
}

void DataField::initUint64( const vector<uint64_t> & dim, uint64_t value )
{
	m_type = eUint64;
	initNumeric( dim, value );
}

void DataField::initInt8( const vector<uint64_t> & dim, int8_t value )
{
	m_type = eInt8;
	initNumeric( dim, value );
}

void DataField::initInt16( const vector<uint64_t> & dim, int16_t value )
{
	m_type = eInt16;
	initNumeric( dim, value );
}

void DataField::initInt32( const vector<uint64_t> & dim, int32_t value )
{
	m_type = eInt32;
	initNumeric( dim, value );
}

void DataField::initInt64( const vector<uint64_t> & dim, int64_t value )
{
	m_type = eInt64;
	initNumeric( dim, value );
}

void DataField::initDouble( const vector<uint64_t> & dim, double value )
{
	m_type = eDouble;
	initNumeric( dim, value );
}

void DataField::initString( const vector<uint64_t> & dim, const string & value )
{
	uint64_t size = getDimensionProduct( dim );
	m_type = eString;
	m_dimensions = dim;
	m_stringValue.resize( size );
	for ( uint64_t u0 = 0; u0 < size; u0++ ) m_stringValue[u0] = value;
}


template<typename T> const T & DataField::getNumeric( const std::vector<uint64_t> & indexes ) const
{
	uint64_t offset = getOffset( indexes, m_dimensions );
	const T * p = (const T *)&m_numericValue[0];
	return( p[offset] );
}


const uint8_t & DataField::getUint8( const vector<uint64_t> & indexes ) const
{
	return( getNumeric<uint8_t>( indexes ) );
}

const uint16_t & DataField::getUint16( const vector<uint64_t> & indexes ) const
{
	return( getNumeric<uint16_t>( indexes ) );
}

const uint32_t & DataField::getUint32( const vector<uint64_t> & indexes ) const
{
	return( getNumeric<uint32_t>( indexes ) );
}

const uint64_t & DataField::getUint64( const vector<uint64_t> & indexes ) const
{
	return( getNumeric<uint64_t>( indexes ) );
}

const int8_t & DataField::getInt8( const vector<uint64_t> & indexes ) const
{
	return( getNumeric<int8_t>( indexes ) );
}

const int16_t & DataField::getInt16( const vector<uint64_t> & indexes ) const
{
	return( getNumeric<int16_t>( indexes ) );
}

const int32_t & DataField::getInt32( const vector<uint64_t> & indexes ) const
{
	return( getNumeric<int32_t>( indexes ) );
}

const int64_t & DataField::getInt64( const vector<uint64_t> & indexes ) const
{
	return( getNumeric<int64_t>( indexes ) );
}

const double & DataField::getDouble( const vector<uint64_t> & indexes ) const
{
	return( getNumeric<double>( indexes ) );
}

const string & DataField::getString( const vector<uint64_t> & indexes ) const
{
	uint64_t offset = getOffset( indexes, m_dimensions );
	return( m_stringValue[offset] );
}

void DataField::setUint8( const vector<uint64_t> & indexes, uint8_t val )
{
	const DataField & obj = *this;
	uint8_t & wval = const_cast<uint8_t &>( obj.getUint8( indexes ) );
	wval = val;
}

void DataField::setUint16( const vector<uint64_t> & indexes, uint16_t val )
{
	const DataField & obj = *this;
	uint16_t & wval = const_cast<uint16_t &>( obj.getUint16( indexes ) );
	wval = val;
}

void DataField::setUint32( const vector<uint64_t> & indexes, uint32_t val )
{
	const DataField & obj = *this;
	uint32_t & wval = const_cast<uint32_t &>( obj.getUint32( indexes ) );
	wval = val;
}

void DataField::setUint64( const vector<uint64_t> & indexes, uint64_t val )
{
	const DataField & obj = *this;
	uint64_t & wval = const_cast<uint64_t &>( obj.getUint64( indexes ) );
	wval = val;
}

void DataField::setInt8( const vector<uint64_t> & indexes, int8_t val )
{
	const DataField & obj = *this;
	int8_t & wval = const_cast<int8_t &>( obj.getInt8( indexes ) );
	wval = val;
}

void DataField::setInt16( const vector<uint64_t> & indexes, int16_t val )
{
	const DataField & obj = *this;
	int16_t & wval = const_cast<int16_t &>( obj.getInt16( indexes ) );
	wval = val;
}

void DataField::setInt32( const vector<uint64_t> & indexes, int32_t val )
{
	const DataField & obj = *this;
	int32_t & wval = const_cast<int32_t &>( obj.getInt32( indexes ) );
	wval = val;
}

void DataField::setInt64( const vector<uint64_t> & indexes, int64_t val )
{
	const DataField & obj = *this;
	int64_t & wval = const_cast<int64_t &>( obj.getInt64( indexes ) );
	wval = val;
}

void DataField::setDouble( const vector<uint64_t> & indexes, double val )
{
	const DataField & obj = *this;
	double & wval = const_cast<double &>( obj.getDouble( indexes ) );
	wval = val;
}

void DataField::setString( const vector<uint64_t> & indexes, const string & val )
{
	const DataField & obj = *this;
	string & wval = const_cast<string &>( obj.getString( indexes ) );
	wval = val;
}


uint64_t DataField::getDimensionProduct( const vector<uint64_t> & dim )
{
	if ( dim.size() == 0 )
		throw Primitives2Exception("Invalid dimensions");

	for ( auto i : dim )
		if ( i == 0 )
			throw Primitives2Exception("Invalid dimensions");

	uint64_t product = 1;
	for ( auto i : dim ) product *= i;

	return( product );
}

uint64_t DataField::getOffset( const vector<uint64_t> & indexes,
				const vector<uint64_t> & dimensions )
{
	uint64_t size = indexes.size();

	if ( size == 0 )
		throw Primitives2Exception("Invalid dimensions");

	if ( dimensions.size() != size )
		throw Primitives2Exception("Invalid dimensions");

	for ( uint64_t u0 = 0; u0 < size; u0++ ) {
		if ( indexes[u0] >= dimensions[u0] )
			throw Primitives2Exception("Index out of range");
	}

	uint64_t offset = 0;
	uint64_t multfact;

	for ( uint64_t u0 = 0; u0 < size; u0++ ) {

		multfact = 1;
		for ( uint64_t u1 = u0+1; u1 < size; u1++ ) {
			multfact *= dimensions[u1];
		}

		offset += indexes[u0] * multfact;
	}

	return( offset );
}

/////////////////////////////////////////////////////////////////////

vector<uint64_t> DataField::initIndex( const vector<uint64_t> & dimensions )
{
	vector<uint64_t> indexes = dimensions;
	for ( auto & i : indexes ) i = 0;
	return( indexes );
}

bool DataField::incIndex( vector<uint64_t> & indexes,
				const vector<uint64_t> & dimensions )
{
	uint64_t size = dimensions.size();

	if ( size == 0 )
		throw Primitives2Exception("Invalid dimensions");

	if ( size != indexes.size() )
		throw Primitives2Exception("Invalid dimensions");

	uint64_t index;

	for ( uint64_t u0 = 0; u0 < size; u0++ ) {
		index = size - 1 - u0;
		indexes[index] += 1;
		if ( indexes[index] != dimensions[index] ) break;
		indexes[index] = 0;
		if ( index == 0 ) return( false );
	}
	return( true );
}

void DataField::setIndex( std::vector<uint64_t> & indexes, uint64_t index )
{
	uint64_t size = indexes.size();

	if ( size != 1 )
		throw Primitives2Exception("Invalid dimensions");

	indexes[0] = index;
}

void DataField::setIndex( std::vector<uint64_t> & indexes, uint64_t index0, uint64_t index1 )
{
	uint64_t size = indexes.size();

	if ( size != 2 )
		throw Primitives2Exception("Invalid dimensions");

	indexes[0] = index0;
	indexes[1] = index1;
}

void DataField::setIndex( std::vector<uint64_t> & indexes, uint64_t index0, uint64_t index1, uint64_t index2 )
{
	uint64_t size = indexes.size();

	if ( size != 3 )
		throw Primitives2Exception("Invalid dimensions");

	indexes[0] = index0;
	indexes[1] = index1;
	indexes[2] = index2;
}

/////////////////////////////////////////////////////////////////////

