#ifndef PRIMITIVES2_DATAMAP_H
#define PRIMITIVES2_DATAMAP_H
/////////////////////////////////////////////////////////////////////
//
// DataMap.h
//
/////////////////////////////////////////////////////////////////////

#include <map>

#include "DataField.h"

/////////////////////////////////////////////////////////////////////

class DataMap {
public:

	DataMap();
	DataMap( const DataMap & dm );
	DataMap( DataMap && dm );

	DataMap & operator = ( const DataMap & dm );
	DataMap & operator = ( DataMap && dm );

	const DataField &	find( const std::string & name ) const;
	DataField &		find( const std::string & name );

	std::vector<std::string>
				getNames() const;

	DataField::Type		getType( const std::string & name ) const;
	bool			exists( const std::string & name ) const;
	std::vector<uint64_t>	getDimensions( const std::string & name ) const;

	void			clear();
	void			erase( const std::string & name );

	/////////////////////////

	void			createUint8( const std::string & name,
					uint8_t value,
					const std::vector<uint64_t> & dim );
	void			createUint8( const std::string & name,
					uint8_t value );
	void			createUint8( const std::string & name,
					uint8_t value,
					uint64_t size );
	void			createUint8( const std::string & name,
					uint8_t value,
					uint64_t size0,
					uint64_t size1 );

	const uint8_t &		getUint8( const std::string & name,
					const std::vector<uint64_t> & indexes ) const;
	const uint8_t &		getUint8( const std::string & name ) const;
	const uint8_t &		getUint8( const std::string & name,
					uint64_t index ) const;
	const uint8_t &		getUint8( const std::string & name,
					uint64_t index0,
					uint64_t index1 ) const;

	void			setUint8( const std::string & name,
					uint8_t value,
					const std::vector<uint64_t> & indexes );
	void			setUint8( const std::string & name,
					uint8_t value );
	void			setUint8( const std::string & name,
					uint8_t value,
					uint64_t index );
	void			setUint8( const std::string & name,
					uint8_t value,
					uint64_t index0,
					uint64_t index1 );

	/////////////////////////

	void			createUint16( const std::string & name,
					uint16_t value,
					const std::vector<uint64_t> & dim );
	void			createUint16( const std::string & name,
					uint16_t value );
	void			createUint16( const std::string & name,
					uint16_t value,
					uint64_t size );
	void			createUint16( const std::string & name,
					uint16_t value,
					uint64_t size0,
					uint64_t size1 );

	const uint16_t &	getUint16( const std::string & name,
					const std::vector<uint64_t> & indexes ) const;
	const uint16_t &	getUint16( const std::string & name ) const;
	const uint16_t &	getUint16( const std::string & name,
					uint64_t index ) const;
	const uint16_t &	getUint16( const std::string & name,
					uint64_t index0,
					uint64_t index1 ) const;

	void			setUint16( const std::string & name,
					uint16_t value,
					const std::vector<uint64_t> & indexes );
	void			setUint16( const std::string & name,
					uint16_t value );
	void			setUint16( const std::string & name,
					uint16_t value,
					uint64_t index );
	void			setUint16( const std::string & name,
					uint16_t value,
					uint64_t index0,
					uint64_t index1 );

	/////////////////////////

	void			createUint32( const std::string & name,
					uint32_t value,
					const std::vector<uint64_t> & dim );
	void			createUint32( const std::string & name,
					uint32_t value );
	void			createUint32( const std::string & name,
					uint32_t value,
					uint64_t size );
	void			createUint32( const std::string & name,
					uint32_t value,
					uint64_t size0,
					uint64_t size1 );

	const uint32_t &	getUint32( const std::string & name,
					const std::vector<uint64_t> & indexes ) const;
	const uint32_t &	getUint32( const std::string & name ) const;
	const uint32_t &	getUint32( const std::string & name,
					uint64_t index ) const;
	const uint32_t &	getUint32( const std::string & name,
					uint64_t index0,
					uint64_t index1 ) const;

	void			setUint32( const std::string & name,
					uint32_t value,
					const std::vector<uint64_t> & indexes );
	void			setUint32( const std::string & name,
					uint32_t value );
	void			setUint32( const std::string & name,
					uint32_t value,
					uint64_t index );
	void			setUint32( const std::string & name,
					uint32_t value,
					uint64_t index0,
					uint64_t index1 );

	/////////////////////////

	void			createUint64( const std::string & name,
					uint64_t value,
					const std::vector<uint64_t> & dim );
	void			createUint64( const std::string & name,
					uint64_t value );
	void			createUint64( const std::string & name,
					uint64_t value,
					uint64_t size );
	void			createUint64( const std::string & name,
					uint64_t value,
					uint64_t size0,
					uint64_t size1 );

	const uint64_t &	getUint64( const std::string & name,
					const std::vector<uint64_t> & indexes ) const;
	const uint64_t &	getUint64( const std::string & name ) const;
	const uint64_t &	getUint64( const std::string & name,
					uint64_t index ) const;
	const uint64_t &	getUint64( const std::string & name,
					uint64_t index0,
					uint64_t index1 ) const;

	void			setUint64( const std::string & name,
					uint64_t value,
					const std::vector<uint64_t> & indexes );
	void			setUint64( const std::string & name,
					uint64_t value );
	void			setUint64( const std::string & name,
					uint64_t value,
					uint64_t index );
	void			setUint64( const std::string & name,
					uint64_t value,
					uint64_t index0,
					uint64_t index1 );

	/////////////////////////

	void			createInt8( const std::string & name,
					int8_t value,
					const std::vector<uint64_t> & dim );
	void			createInt8( const std::string & name,
					int8_t value );
	void			createInt8( const std::string & name,
					int8_t value,
					uint64_t size );
	void			createInt8( const std::string & name,
					int8_t value,
					uint64_t size0,
					uint64_t size1 );

	const int8_t &		getInt8( const std::string & name,
					const std::vector<uint64_t> & indexes ) const;
	const int8_t &		getInt8( const std::string & name ) const;
	const int8_t &		getInt8( const std::string & name,
					uint64_t index ) const;
	const int8_t &		getInt8( const std::string & name,
					uint64_t index0,
					uint64_t index1 ) const;

	void			setInt8( const std::string & name,
					int8_t value,
					const std::vector<uint64_t> & indexes );
	void			setInt8( const std::string & name,
					int8_t value );
	void			setInt8( const std::string & name,
					int8_t value,
					uint64_t index );
	void			setInt8( const std::string & name,
					int8_t value,
					uint64_t index0,
					uint64_t index1 );

	/////////////////////////

	void			createInt16( const std::string & name,
					int16_t value,
					const std::vector<uint64_t> & dim );
	void			createInt16( const std::string & name,
					int16_t value );
	void			createInt16( const std::string & name,
					int16_t value,
					uint64_t size );
	void			createInt16( const std::string & name,
					int16_t value,
					uint64_t size0,
					uint64_t size1 );

	const int16_t &		getInt16( const std::string & name,
					const std::vector<uint64_t> & indexes ) const;
	const int16_t &		getInt16( const std::string & name ) const;
	const int16_t &		getInt16( const std::string & name,
					uint64_t index ) const;
	const int16_t &		getInt16( const std::string & name,
					uint64_t index0,
					uint64_t index1 ) const;

	void			setInt16( const std::string & name,
					int16_t value,
					const std::vector<uint64_t> & indexes );
	void			setInt16( const std::string & name,
					int16_t value );
	void			setInt16( const std::string & name,
					int16_t value,
					uint64_t index );
	void			setInt16( const std::string & name,
					int16_t value,
					uint64_t index0,
					uint64_t index1 );

	/////////////////////////

	void			createInt32( const std::string & name,
					int32_t value,
					const std::vector<uint64_t> & dim );
	void			createInt32( const std::string & name,
					int32_t value );
	void			createInt32( const std::string & name,
					int32_t value,
					uint64_t size );
	void			createInt32( const std::string & name,
					int32_t value,
					uint64_t size0,
					uint64_t size1 );

	const int32_t &		getInt32( const std::string & name,
					const std::vector<uint64_t> & indexes ) const;
	const int32_t &		getInt32( const std::string & name ) const;
	const int32_t &		getInt32( const std::string & name,
					uint64_t index ) const;
	const int32_t &		getInt32( const std::string & name,
					uint64_t index0,
					uint64_t index1 ) const;

	void			setInt32( const std::string & name,
					int32_t value,
					const std::vector<uint64_t> & indexes );
	void			setInt32( const std::string & name,
					int32_t value );
	void			setInt32( const std::string & name,
					int32_t value,
					uint64_t index );
	void			setInt32( const std::string & name,
					int32_t value,
					uint64_t index0,
					uint64_t index1 );

	/////////////////////////

	void			createInt64( const std::string & name,
					int64_t value,
					const std::vector<uint64_t> & dim );
	void			createInt64( const std::string & name,
					int64_t value );
	void			createInt64( const std::string & name,
					int64_t value,
					uint64_t size );
	void			createInt64( const std::string & name,
					int64_t value,
					uint64_t size0,
					uint64_t size1 );

	const int64_t &		getInt64( const std::string & name,
					const std::vector<uint64_t> & indexes ) const;
	const int64_t &		getInt64( const std::string & name ) const;
	const int64_t &		getInt64( const std::string & name,
					uint64_t index ) const;
	const int64_t &		getInt64( const std::string & name,
					uint64_t index0,
					uint64_t index1 ) const;

	void			setInt64( const std::string & name,
					int64_t value,
					const std::vector<uint64_t> & indexes );
	void			setInt64( const std::string & name,
					int64_t value );
	void			setInt64( const std::string & name,
					int64_t value,
					uint64_t index );
	void			setInt64( const std::string & name,
					int64_t value,
					uint64_t index0,
					uint64_t index1 );

	/////////////////////////

	void			createDouble( const std::string & name,
					double value,
					const std::vector<uint64_t> & dim );
	void			createDouble( const std::string & name,
					double value );
	void			createDouble( const std::string & name,
					double value,
					uint64_t size );
	void			createDouble( const std::string & name,
					double value,
					uint64_t size0,
					uint64_t size1 );

	const double &		getDouble( const std::string & name,
					const std::vector<uint64_t> & indexes ) const;
	const double &		getDouble( const std::string & name ) const;
	const double &		getDouble( const std::string & name,
					uint64_t index ) const;
	const double &		getDouble( const std::string & name,
					uint64_t index0,
					uint64_t index1 ) const;

	void			setDouble( const std::string & name,
					double value,
					const std::vector<uint64_t> & indexes );
	void			setDouble( const std::string & name,
					double value );
	void			setDouble( const std::string & name,
					double value,
					uint64_t index );
	void			setDouble( const std::string & name,
					double value,
					uint64_t index0,
					uint64_t index1 );

	/////////////////////////

	void			createString( const std::string & name,
					const std::string & value,
					const std::vector<uint64_t> & dim );
	void			createString( const std::string & name,
					const std::string & value );
	void			createString( const std::string & name,
					const std::string & value,
					uint64_t size );
	void			createString( const std::string & name,
					const std::string & value,
					uint64_t size0,
					uint64_t size1 );

	const std::string &	getString( const std::string & name,
					const std::vector<uint64_t> & indexes ) const;
	const std::string &	getString( const std::string & name ) const;
	const std::string &	getString( const std::string & name,
					uint64_t index ) const;
	const std::string &	getString( const std::string & name,
					uint64_t index0,
					uint64_t index1 ) const;

	void			setString( const std::string & name,
					const std::string & value,
					const std::vector<uint64_t> & indexes );
	void			setString( const std::string & name,
					const std::string & value );
	void			setString( const std::string & name,
					const std::string & value,
					uint64_t index );
	void			setString( const std::string & name,
					const std::string & value,
					uint64_t index0,
					uint64_t index1 );

	/////////////////////////

private:
	std::map<std::string,DataField>		m_dataMap;
};

/////////////////////////////////////////////////////////////////////

#endif
