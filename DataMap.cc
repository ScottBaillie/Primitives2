/////////////////////////////////////////////////////////////////////
//
// DataMap.cc
//
/////////////////////////////////////////////////////////////////////

#include "DataMap.h"
#include "Exception.h"

using namespace std;

/////////////////////////////////////////////////////////////////////

DataMap::DataMap()
{
}

DataMap::DataMap( const DataMap & dm )
{
	*this = dm;
}

DataMap::DataMap( DataMap && dm )
{
	*this = dm;
}

DataMap & DataMap::operator = ( const DataMap & dm )
{
	m_dataMap = dm.m_dataMap;
	return( *this );
}

DataMap & DataMap::operator = ( DataMap && dm )
{
	m_dataMap = std::move( dm.m_dataMap );
	return( *this );
}

const DataField & DataMap::find( const std::string & name ) const
{
	auto i = m_dataMap.find( name );
	if ( i == m_dataMap.end() )
		throw Primitives2Exception("Name not found");
	return( i->second );
}

DataField & DataMap::find( const std::string & name )
{
	const DataMap & obj = *this;
	return( const_cast<DataField &>( obj.find( name ) ) );
}

vector<string> DataMap::getNames() const
{
	vector<string> names;
	for ( auto i : m_dataMap ) names.push_back( i.first );
	return( names );
}

DataField::Type DataMap::getType( const string & name ) const
{
	const DataField & d = find( name );
	return( d.m_type );
}

bool DataMap::exists( const string & name ) const
{
	auto i = m_dataMap.find( name );
	if ( i == m_dataMap.end() ) return( false );
	return( true );
}

vector<uint64_t> DataMap::getDimensions( const string & name ) const
{
	const DataField & d = find( name );
	return( d.m_dimensions );
}

void DataMap::clear()
{
	m_dataMap.clear();
}

void DataMap::erase( const string & name )
{
	auto i = m_dataMap.find( name );
	if ( i == m_dataMap.end() )
		throw Primitives2Exception("Name not found");
	m_dataMap.erase( i );
}

/////////////////////////////////////////////////////////////////////

void DataMap::createUint8( const string & name,
				uint8_t value,
				const vector<uint64_t> & dim )
{
	if ( name.size() == 0 )
		throw Primitives2Exception("Invalid name");

	if ( exists( name ) )
		throw Primitives2Exception("Name exists");

	DataField & d = m_dataMap[ name ];

	d.initUint8( dim, value );
}

void DataMap::createUint8( const string & name,
				uint8_t value )
{
	vector<uint64_t> dim;
	dim.push_back(1);
	createUint8( name, value, dim );
}

void DataMap::createUint8( const string & name,
				uint8_t value,
				uint64_t size )
{
	vector<uint64_t> dim;
	dim.push_back( size );
	createUint8( name, value, dim );
}

void DataMap::createUint8( const string & name,
				uint8_t value,
				uint64_t size0,
				uint64_t size1 )
{
	vector<uint64_t> dim;
	dim.push_back( size0 );
	dim.push_back( size1 );
	createUint8( name, value, dim );
}


const uint8_t & DataMap::getUint8( const string & name,
				const vector<uint64_t> & indexes ) const
{
	return( find( name ).getUint8( indexes ) );
}

const uint8_t & DataMap::getUint8( const string & name ) const
{
	vector<uint64_t> indexes;
	indexes.push_back(0);
	return( getUint8( name, indexes ) );
}

const uint8_t & DataMap::getUint8( const string & name,
				uint64_t index ) const
{
	vector<uint64_t> indexes;
	indexes.push_back( index );
	return( getUint8( name, indexes ) );
}

const uint8_t & DataMap::getUint8( const string & name,
				uint64_t index0,
				uint64_t index1 ) const
{
	vector<uint64_t> indexes;
	indexes.push_back( index0 );
	indexes.push_back( index1 );
	return( getUint8( name, indexes ) );
}


void DataMap::setUint8( const string & name,
				uint8_t value,
				const vector<uint64_t> & indexes )
{
	find( name ).setUint8( indexes, value );
}

void DataMap::setUint8( const string & name,
				uint8_t value )
{
	vector<uint64_t> indexes;
	indexes.push_back(0);
	setUint8( name, value, indexes );
}

void DataMap::setUint8( const string & name,
				uint8_t value,
				uint64_t index )
{
	vector<uint64_t> indexes;
	indexes.push_back( index );
	setUint8( name, value, indexes );
}

void DataMap::setUint8( const string & name,
				uint8_t value,
				uint64_t index0,
				uint64_t index1 )
{
	vector<uint64_t> indexes;
	indexes.push_back( index0 );
	indexes.push_back( index1 );
	setUint8( name, value, indexes );
}

/////////////////////////////////////////////////////////////////////

void DataMap::createUint16( const string & name,
				uint16_t value,
				const vector<uint64_t> & dim )
{
	if ( name.size() == 0 )
		throw Primitives2Exception("Invalid name");

	if ( exists( name ) )
		throw Primitives2Exception("Name exists");

	DataField & d = m_dataMap[ name ];

	d.initUint16( dim, value );
}

void DataMap::createUint16( const string & name,
				uint16_t value )
{
	vector<uint64_t> dim;
	dim.push_back(1);
	createUint16( name, value, dim );
}

void DataMap::createUint16( const string & name,
				uint16_t value,
				uint64_t size )
{
	vector<uint64_t> dim;
	dim.push_back( size );
	createUint16( name, value, dim );
}

void DataMap::createUint16( const string & name,
				uint16_t value,
				uint64_t size0,
				uint64_t size1 )
{
	vector<uint64_t> dim;
	dim.push_back( size0 );
	dim.push_back( size1 );
	createUint16( name, value, dim );
}


const uint16_t & DataMap::getUint16( const string & name,
				const vector<uint64_t> & indexes ) const
{
	return( find( name ).getUint16( indexes ) );
}

const uint16_t & DataMap::getUint16( const string & name ) const
{
	vector<uint64_t> indexes;
	indexes.push_back(0);
	return( getUint16( name, indexes ) );
}

const uint16_t & DataMap::getUint16( const string & name,
				uint64_t index ) const
{
	vector<uint64_t> indexes;
	indexes.push_back( index );
	return( getUint16( name, indexes ) );
}

const uint16_t & DataMap::getUint16( const string & name,
				uint64_t index0,
				uint64_t index1 ) const
{
	vector<uint64_t> indexes;
	indexes.push_back( index0 );
	indexes.push_back( index1 );
	return( getUint16( name, indexes ) );
}


void DataMap::setUint16( const string & name,
				uint16_t value,
				const vector<uint64_t> & indexes )
{
	find( name ).setUint16( indexes, value );
}

void DataMap::setUint16( const string & name,
				uint16_t value )
{
	vector<uint64_t> indexes;
	indexes.push_back(0);
	setUint16( name, value, indexes );
}

void DataMap::setUint16( const string & name,
				uint16_t value,
				uint64_t index )
{
	vector<uint64_t> indexes;
	indexes.push_back( index );
	setUint16( name, value, indexes );
}

void DataMap::setUint16( const string & name,
				uint16_t value,
				uint64_t index0,
				uint64_t index1 )
{
	vector<uint64_t> indexes;
	indexes.push_back( index0 );
	indexes.push_back( index1 );
	setUint16( name, value, indexes );
}

/////////////////////////////////////////////////////////////////////

void DataMap::createUint32( const string & name,
				uint32_t value,
				const vector<uint64_t> & dim )
{
	if ( name.size() == 0 )
		throw Primitives2Exception("Invalid name");

	if ( exists( name ) )
		throw Primitives2Exception("Name exists");

	DataField & d = m_dataMap[ name ];

	d.initUint32( dim, value );
}

void DataMap::createUint32( const string & name,
				uint32_t value )
{
	vector<uint64_t> dim;
	dim.push_back(1);
	createUint32( name, value, dim );
}

void DataMap::createUint32( const string & name,
				uint32_t value,
				uint64_t size )
{
	vector<uint64_t> dim;
	dim.push_back( size );
	createUint32( name, value, dim );
}

void DataMap::createUint32( const string & name,
				uint32_t value,
				uint64_t size0,
				uint64_t size1 )
{
	vector<uint64_t> dim;
	dim.push_back( size0 );
	dim.push_back( size1 );
	createUint32( name, value, dim );
}


const uint32_t & DataMap::getUint32( const string & name,
				const vector<uint64_t> & indexes ) const
{
	return( find( name ).getUint32( indexes ) );
}

const uint32_t & DataMap::getUint32( const string & name ) const
{
	vector<uint64_t> indexes;
	indexes.push_back(0);
	return( getUint32( name, indexes ) );
}

const uint32_t & DataMap::getUint32( const string & name,
				uint64_t index ) const
{
	vector<uint64_t> indexes;
	indexes.push_back( index );
	return( getUint32( name, indexes ) );
}

const uint32_t & DataMap::getUint32( const string & name,
				uint64_t index0,
				uint64_t index1 ) const
{
	vector<uint64_t> indexes;
	indexes.push_back( index0 );
	indexes.push_back( index1 );
	return( getUint32( name, indexes ) );
}


void DataMap::setUint32( const string & name,
				uint32_t value,
				const vector<uint64_t> & indexes )
{
	find( name ).setUint32( indexes, value );
}

void DataMap::setUint32( const string & name,
				uint32_t value )
{
	vector<uint64_t> indexes;
	indexes.push_back(0);
	setUint32( name, value, indexes );
}

void DataMap::setUint32( const string & name,
				uint32_t value,
				uint64_t index )
{
	vector<uint64_t> indexes;
	indexes.push_back( index );
	setUint32( name, value, indexes );
}

void DataMap::setUint32( const string & name,
				uint32_t value,
				uint64_t index0,
				uint64_t index1 )
{
	vector<uint64_t> indexes;
	indexes.push_back( index0 );
	indexes.push_back( index1 );
	setUint32( name, value, indexes );
}

/////////////////////////////////////////////////////////////////////

void DataMap::createUint64( const string & name,
				uint64_t value,
				const vector<uint64_t> & dim )
{
	if ( name.size() == 0 )
		throw Primitives2Exception("Invalid name");

	if ( exists( name ) )
		throw Primitives2Exception("Name exists");

	DataField & d = m_dataMap[ name ];

	d.initUint64( dim, value );
}

void DataMap::createUint64( const string & name,
				uint64_t value )
{
	vector<uint64_t> dim;
	dim.push_back(1);
	createUint64( name, value, dim );
}

void DataMap::createUint64( const string & name,
				uint64_t value,
				uint64_t size )
{
	vector<uint64_t> dim;
	dim.push_back( size );
	createUint64( name, value, dim );
}

void DataMap::createUint64( const string & name,
				uint64_t value,
				uint64_t size0,
				uint64_t size1 )
{
	vector<uint64_t> dim;
	dim.push_back( size0 );
	dim.push_back( size1 );
	createUint64( name, value, dim );
}


const uint64_t & DataMap::getUint64( const string & name,
				const vector<uint64_t> & indexes ) const
{
	return( find( name ).getUint64( indexes ) );
}

const uint64_t & DataMap::getUint64( const string & name ) const
{
	vector<uint64_t> indexes;
	indexes.push_back(0);
	return( getUint64( name, indexes ) );
}

const uint64_t & DataMap::getUint64( const string & name,
				uint64_t index ) const
{
	vector<uint64_t> indexes;
	indexes.push_back( index );
	return( getUint64( name, indexes ) );
}

const uint64_t & DataMap::getUint64( const string & name,
				uint64_t index0,
				uint64_t index1 ) const
{
	vector<uint64_t> indexes;
	indexes.push_back( index0 );
	indexes.push_back( index1 );
	return( getUint64( name, indexes ) );
}


void DataMap::setUint64( const string & name,
				uint64_t value,
				const vector<uint64_t> & indexes )
{
	find( name ).setUint64( indexes, value );
}

void DataMap::setUint64( const string & name,
				uint64_t value )
{
	vector<uint64_t> indexes;
	indexes.push_back(0);
	setUint64( name, value, indexes );
}

void DataMap::setUint64( const string & name,
				uint64_t value,
				uint64_t index )
{
	vector<uint64_t> indexes;
	indexes.push_back( index );
	setUint64( name, value, indexes );
}

void DataMap::setUint64( const string & name,
				uint64_t value,
				uint64_t index0,
				uint64_t index1 )
{
	vector<uint64_t> indexes;
	indexes.push_back( index0 );
	indexes.push_back( index1 );
	setUint64( name, value, indexes );
}

/////////////////////////////////////////////////////////////////////

void DataMap::createInt8( const string & name,
				int8_t value,
				const vector<uint64_t> & dim )
{
	if ( name.size() == 0 )
		throw Primitives2Exception("Invalid name");

	if ( exists( name ) )
		throw Primitives2Exception("Name exists");

	DataField & d = m_dataMap[ name ];

	d.initInt8( dim, value );
}

void DataMap::createInt8( const string & name,
				int8_t value )
{
	vector<uint64_t> dim;
	dim.push_back(1);
	createInt8( name, value, dim );
}

void DataMap::createInt8( const string & name,
				int8_t value,
				uint64_t size )
{
	vector<uint64_t> dim;
	dim.push_back( size );
	createInt8( name, value, dim );
}

void DataMap::createInt8( const string & name,
				int8_t value,
				uint64_t size0,
				uint64_t size1 )
{
	vector<uint64_t> dim;
	dim.push_back( size0 );
	dim.push_back( size1 );
	createInt8( name, value, dim );
}


const int8_t & DataMap::getInt8( const string & name,
				const vector<uint64_t> & indexes ) const
{
	return( find( name ).getInt8( indexes ) );
}

const int8_t & DataMap::getInt8( const string & name ) const
{
	vector<uint64_t> indexes;
	indexes.push_back(0);
	return( getInt8( name, indexes ) );
}

const int8_t & DataMap::getInt8( const string & name,
				uint64_t index ) const
{
	vector<uint64_t> indexes;
	indexes.push_back( index );
	return( getInt8( name, indexes ) );
}

const int8_t & DataMap::getInt8( const string & name,
				uint64_t index0,
				uint64_t index1 ) const
{
	vector<uint64_t> indexes;
	indexes.push_back( index0 );
	indexes.push_back( index1 );
	return( getInt8( name, indexes ) );
}


void DataMap::setInt8( const string & name,
				int8_t value,
				const vector<uint64_t> & indexes )
{
	find( name ).setInt8( indexes, value );
}

void DataMap::setInt8( const string & name,
				int8_t value )
{
	vector<uint64_t> indexes;
	indexes.push_back(0);
	setInt8( name, value, indexes );
}

void DataMap::setInt8( const string & name,
				int8_t value,
				uint64_t index )
{
	vector<uint64_t> indexes;
	indexes.push_back( index );
	setInt8( name, value, indexes );
}

void DataMap::setInt8( const string & name,
				int8_t value,
				uint64_t index0,
				uint64_t index1 )
{
	vector<uint64_t> indexes;
	indexes.push_back( index0 );
	indexes.push_back( index1 );
	setInt8( name, value, indexes );
}

/////////////////////////////////////////////////////////////////////

void DataMap::createInt16( const string & name,
				int16_t value,
				const vector<uint64_t> & dim )
{
	if ( name.size() == 0 )
		throw Primitives2Exception("Invalid name");

	if ( exists( name ) )
		throw Primitives2Exception("Name exists");

	DataField & d = m_dataMap[ name ];

	d.initInt16( dim, value );
}

void DataMap::createInt16( const string & name,
				int16_t value )
{
	vector<uint64_t> dim;
	dim.push_back(1);
	createInt16( name, value, dim );
}

void DataMap::createInt16( const string & name,
				int16_t value,
				uint64_t size )
{
	vector<uint64_t> dim;
	dim.push_back( size );
	createInt16( name, value, dim );
}

void DataMap::createInt16( const string & name,
				int16_t value,
				uint64_t size0,
				uint64_t size1 )
{
	vector<uint64_t> dim;
	dim.push_back( size0 );
	dim.push_back( size1 );
	createInt16( name, value, dim );
}


const int16_t & DataMap::getInt16( const string & name,
				const vector<uint64_t> & indexes ) const
{
	return( find( name ).getInt16( indexes ) );
}

const int16_t & DataMap::getInt16( const string & name ) const
{
	vector<uint64_t> indexes;
	indexes.push_back(0);
	return( getInt16( name, indexes ) );
}

const int16_t & DataMap::getInt16( const string & name,
				uint64_t index ) const
{
	vector<uint64_t> indexes;
	indexes.push_back( index );
	return( getInt16( name, indexes ) );
}

const int16_t & DataMap::getInt16( const string & name,
				uint64_t index0,
				uint64_t index1 ) const
{
	vector<uint64_t> indexes;
	indexes.push_back( index0 );
	indexes.push_back( index1 );
	return( getInt16( name, indexes ) );
}


void DataMap::setInt16( const string & name,
				int16_t value,
				const vector<uint64_t> & indexes )
{
	find( name ).setInt16( indexes, value );
}

void DataMap::setInt16( const string & name,
				int16_t value )
{
	vector<uint64_t> indexes;
	indexes.push_back(0);
	setInt16( name, value, indexes );
}

void DataMap::setInt16( const string & name,
				int16_t value,
				uint64_t index )
{
	vector<uint64_t> indexes;
	indexes.push_back( index );
	setInt16( name, value, indexes );
}

void DataMap::setInt16( const string & name,
				int16_t value,
				uint64_t index0,
				uint64_t index1 )
{
	vector<uint64_t> indexes;
	indexes.push_back( index0 );
	indexes.push_back( index1 );
	setInt16( name, value, indexes );
}

/////////////////////////////////////////////////////////////////////

void DataMap::createInt32( const string & name,
				int32_t value,
				const vector<uint64_t> & dim )
{
	if ( name.size() == 0 )
		throw Primitives2Exception("Invalid name");

	if ( exists( name ) )
		throw Primitives2Exception("Name exists");

	DataField & d = m_dataMap[ name ];

	d.initInt32( dim, value );
}

void DataMap::createInt32( const string & name,
				int32_t value )
{
	vector<uint64_t> dim;
	dim.push_back(1);
	createInt32( name, value, dim );
}

void DataMap::createInt32( const string & name,
				int32_t value,
				uint64_t size )
{
	vector<uint64_t> dim;
	dim.push_back( size );
	createInt32( name, value, dim );
}

void DataMap::createInt32( const string & name,
				int32_t value,
				uint64_t size0,
				uint64_t size1 )
{
	vector<uint64_t> dim;
	dim.push_back( size0 );
	dim.push_back( size1 );
	createInt32( name, value, dim );
}


const int32_t & DataMap::getInt32( const string & name,
				const vector<uint64_t> & indexes ) const
{
	return( find( name ).getInt32( indexes ) );
}

const int32_t & DataMap::getInt32( const string & name ) const
{
	vector<uint64_t> indexes;
	indexes.push_back(0);
	return( getInt32( name, indexes ) );
}

const int32_t & DataMap::getInt32( const string & name,
				uint64_t index ) const
{
	vector<uint64_t> indexes;
	indexes.push_back( index );
	return( getInt32( name, indexes ) );
}

const int32_t & DataMap::getInt32( const string & name,
				uint64_t index0,
				uint64_t index1 ) const
{
	vector<uint64_t> indexes;
	indexes.push_back( index0 );
	indexes.push_back( index1 );
	return( getInt32( name, indexes ) );
}


void DataMap::setInt32( const string & name,
				int32_t value,
				const vector<uint64_t> & indexes )
{
	find( name ).setInt32( indexes, value );
}

void DataMap::setInt32( const string & name,
				int32_t value )
{
	vector<uint64_t> indexes;
	indexes.push_back(0);
	setInt32( name, value, indexes );
}

void DataMap::setInt32( const string & name,
				int32_t value,
				uint64_t index )
{
	vector<uint64_t> indexes;
	indexes.push_back( index );
	setInt32( name, value, indexes );
}

void DataMap::setInt32( const string & name,
				int32_t value,
				uint64_t index0,
				uint64_t index1 )
{
	vector<uint64_t> indexes;
	indexes.push_back( index0 );
	indexes.push_back( index1 );
	setInt32( name, value, indexes );
}

/////////////////////////////////////////////////////////////////////

void DataMap::createInt64( const string & name,
				int64_t value,
				const vector<uint64_t> & dim )
{
	if ( name.size() == 0 )
		throw Primitives2Exception("Invalid name");

	if ( exists( name ) )
		throw Primitives2Exception("Name exists");

	DataField & d = m_dataMap[ name ];

	d.initInt64( dim, value );
}

void DataMap::createInt64( const string & name,
				int64_t value )
{
	vector<uint64_t> dim;
	dim.push_back(1);
	createInt64( name, value, dim );
}

void DataMap::createInt64( const string & name,
				int64_t value,
				uint64_t size )
{
	vector<uint64_t> dim;
	dim.push_back( size );
	createInt64( name, value, dim );
}

void DataMap::createInt64( const string & name,
				int64_t value,
				uint64_t size0,
				uint64_t size1 )
{
	vector<uint64_t> dim;
	dim.push_back( size0 );
	dim.push_back( size1 );
	createInt64( name, value, dim );
}


const int64_t & DataMap::getInt64( const string & name,
				const vector<uint64_t> & indexes ) const
{
	return( find( name ).getInt64( indexes ) );
}

const int64_t & DataMap::getInt64( const string & name ) const
{
	vector<uint64_t> indexes;
	indexes.push_back(0);
	return( getInt64( name, indexes ) );
}

const int64_t & DataMap::getInt64( const string & name,
				uint64_t index ) const
{
	vector<uint64_t> indexes;
	indexes.push_back( index );
	return( getInt64( name, indexes ) );
}

const int64_t & DataMap::getInt64( const string & name,
				uint64_t index0,
				uint64_t index1 ) const
{
	vector<uint64_t> indexes;
	indexes.push_back( index0 );
	indexes.push_back( index1 );
	return( getInt64( name, indexes ) );
}


void DataMap::setInt64( const string & name,
				int64_t value,
				const vector<uint64_t> & indexes )
{
	find( name ).setInt64( indexes, value );
}

void DataMap::setInt64( const string & name,
				int64_t value )
{
	vector<uint64_t> indexes;
	indexes.push_back(0);
	setInt64( name, value, indexes );
}

void DataMap::setInt64( const string & name,
				int64_t value,
				uint64_t index )
{
	vector<uint64_t> indexes;
	indexes.push_back( index );
	setInt64( name, value, indexes );
}

void DataMap::setInt64( const string & name,
				int64_t value,
				uint64_t index0,
				uint64_t index1 )
{
	vector<uint64_t> indexes;
	indexes.push_back( index0 );
	indexes.push_back( index1 );
	setInt64( name, value, indexes );
}

/////////////////////////////////////////////////////////////////////

void DataMap::createDouble( const string & name,
				double value,
				const vector<uint64_t> & dim )
{
	if ( name.size() == 0 )
		throw Primitives2Exception("Invalid name");

	if ( exists( name ) )
		throw Primitives2Exception("Name exists");

	DataField & d = m_dataMap[ name ];

	d.initDouble( dim, value );
}

void DataMap::createDouble( const string & name,
				double value )
{
	vector<uint64_t> dim;
	dim.push_back(1);
	createDouble( name, value, dim );
}

void DataMap::createDouble( const string & name,
				double value,
				uint64_t size )
{
	vector<uint64_t> dim;
	dim.push_back( size );
	createDouble( name, value, dim );
}

void DataMap::createDouble( const string & name,
				double value,
				uint64_t size0,
				uint64_t size1 )
{
	vector<uint64_t> dim;
	dim.push_back( size0 );
	dim.push_back( size1 );
	createDouble( name, value, dim );
}


const double & DataMap::getDouble( const string & name,
				const vector<uint64_t> & indexes ) const
{
	return( find( name ).getDouble( indexes ) );
}

const double & DataMap::getDouble( const string & name ) const
{
	vector<uint64_t> indexes;
	indexes.push_back(0);
	return( getDouble( name, indexes ) );
}

const double & DataMap::getDouble( const string & name,
				uint64_t index ) const
{
	vector<uint64_t> indexes;
	indexes.push_back( index );
	return( getDouble( name, indexes ) );
}

const double & DataMap::getDouble( const string & name,
				uint64_t index0,
				uint64_t index1 ) const
{
	vector<uint64_t> indexes;
	indexes.push_back( index0 );
	indexes.push_back( index1 );
	return( getDouble( name, indexes ) );
}


void DataMap::setDouble( const string & name,
				double value,
				const vector<uint64_t> & indexes )
{
	find( name ).setDouble( indexes, value );
}

void DataMap::setDouble( const string & name,
				double value )
{
	vector<uint64_t> indexes;
	indexes.push_back(0);
	setDouble( name, value, indexes );
}

void DataMap::setDouble( const string & name,
				double value,
				uint64_t index )
{
	vector<uint64_t> indexes;
	indexes.push_back( index );
	setDouble( name, value, indexes );
}

void DataMap::setDouble( const string & name,
				double value,
				uint64_t index0,
				uint64_t index1 )
{
	vector<uint64_t> indexes;
	indexes.push_back( index0 );
	indexes.push_back( index1 );
	setDouble( name, value, indexes );
}

/////////////////////////////////////////////////////////////////////

void DataMap::createString( const string & name,
				const string & value,
				const vector<uint64_t> & dim )
{
	if ( name.size() == 0 )
		throw Primitives2Exception("Invalid name");

	if ( exists( name ) )
		throw Primitives2Exception("Name exists");

	DataField & d = m_dataMap[ name ];

	d.initString( dim, value );
}

void DataMap::createString( const string & name,
				const string & value )
{
	vector<uint64_t> dim;
	dim.push_back(1);
	createString( name, value, dim );
}

void DataMap::createString( const string & name,
				const string & value,
				uint64_t size )
{
	vector<uint64_t> dim;
	dim.push_back( size );
	createString( name, value, dim );
}

void DataMap::createString( const string & name,
				const string & value,
				uint64_t size0,
				uint64_t size1 )
{
	vector<uint64_t> dim;
	dim.push_back( size0 );
	dim.push_back( size1 );
	createString( name, value, dim );
}


const string & DataMap::getString( const string & name,
				const vector<uint64_t> & indexes ) const
{
	return( find( name ).getString( indexes ) );
}

const string & DataMap::getString( const string & name ) const
{
	vector<uint64_t> indexes;
	indexes.push_back(0);
	return( getString( name, indexes ) );
}

const string & DataMap::getString( const string & name,
				uint64_t index ) const
{
	vector<uint64_t> indexes;
	indexes.push_back( index );
	return( getString( name, indexes ) );
}

const string & DataMap::getString( const string & name,
				uint64_t index0,
				uint64_t index1 ) const
{
	vector<uint64_t> indexes;
	indexes.push_back( index0 );
	indexes.push_back( index1 );
	return( getString( name, indexes ) );
}


void DataMap::setString( const string & name,
				const string & value,
				const vector<uint64_t> & indexes )
{
	find( name ).setString( indexes, value );
}

void DataMap::setString( const string & name,
				const string & value )
{
	vector<uint64_t> indexes;
	indexes.push_back(0);
	setString( name, value, indexes );
}

void DataMap::setString( const string & name,
				const string & value,
				uint64_t index )
{
	vector<uint64_t> indexes;
	indexes.push_back( index );
	setString( name, value, indexes );
}

void DataMap::setString( const string & name,
				const string & value,
				uint64_t index0,
				uint64_t index1 )
{
	vector<uint64_t> indexes;
	indexes.push_back( index0 );
	indexes.push_back( index1 );
	setString( name, value, indexes );
}

/////////////////////////////////////////////////////////////////////

