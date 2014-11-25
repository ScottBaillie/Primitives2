#ifndef PRIMITIVES2_DATAFIELD_H
#define PRIMITIVES2_DATAFIELD_H
/////////////////////////////////////////////////////////////////////
//
// DataField.h
//
/////////////////////////////////////////////////////////////////////

#include "Buffer.h"

/////////////////////////////////////////////////////////////////////

struct DataField {

public:

	friend class ConvertToBinary;
	friend class ConvertFromBinary;
	friend class DataMap;

	enum Type {
		eUint8,
		eUint16,
		eUint32,
		eUint64,
		eInt8,
		eInt16,
		eInt32,
		eInt64,
		eDouble,
		eString
	};

	DataField();
	DataField( const DataField & f );
	DataField( DataField && f );

	DataField & operator = ( DataField && f );

	/////////////////////////

private:

	DataField & operator = ( const DataField & f );

	template<typename T> void
				initNumeric( const std::vector<uint64_t> & dim, T value );

	void			initUint8( const std::vector<uint64_t> & dim,
					uint8_t value );
	void			initUint16( const std::vector<uint64_t> & dim,
					uint16_t value );
	void			initUint32( const std::vector<uint64_t> & dim,
					uint32_t value );
	void			initUint64( const std::vector<uint64_t> & dim,
					uint64_t value );

	void			initInt8( const std::vector<uint64_t> & dim,
				       int8_t value );
	void			initInt16( const std::vector<uint64_t> & dim,
				       int16_t value );
	void			initInt32( const std::vector<uint64_t> & dim,
				       int32_t value );
	void			initInt64( const std::vector<uint64_t> & dim,
					int64_t value );

	void			initDouble( const std::vector<uint64_t> & dim,
					double value );

	void			initString( const std::vector<uint64_t> & dim,
					const std::string & value );

	/////////////////////////

	template<typename T> const T &
				getNumeric( const std::vector<uint64_t> & indexes ) const;

public:

	const uint8_t &		getUint8( const std::vector<uint64_t> & indexes ) const;
	const uint16_t &	getUint16( const std::vector<uint64_t> & indexes ) const;
	const uint32_t &	getUint32( const std::vector<uint64_t> & indexes ) const;
	const uint64_t &	getUint64( const std::vector<uint64_t> & indexes ) const;
	const int8_t &		getInt8( const std::vector<uint64_t> & indexes ) const;
	const int16_t &		getInt16( const std::vector<uint64_t> & indexes ) const;
	const int32_t &		getInt32( const std::vector<uint64_t> & indexes ) const;
	const int64_t &		getInt64( const std::vector<uint64_t> & indexes ) const;
	const double &		getDouble( const std::vector<uint64_t> & indexes ) const;
	const std::string &	getString( const std::vector<uint64_t> & indexes ) const;

	void			setUint8( const std::vector<uint64_t> & indexes, uint8_t val );
	void			setUint16( const std::vector<uint64_t> & indexes, uint16_t val );
	void			setUint32( const std::vector<uint64_t> & indexes, uint32_t val );
	void			setUint64( const std::vector<uint64_t> & indexes, uint64_t val );
	void			setInt8( const std::vector<uint64_t> & indexes, int8_t val );
	void			setInt16( const std::vector<uint64_t> & indexes, int16_t val );
	void			setInt32( const std::vector<uint64_t> & indexes, int32_t val );
	void			setInt64( const std::vector<uint64_t> & indexes, int64_t val );
	void			setDouble( const std::vector<uint64_t> & indexes, double val );
	void			setString( const std::vector<uint64_t> & indexes, const std::string & val );

	/////////////////////////

	static uint64_t		getDimensionProduct( const std::vector<uint64_t> & dim );

	static uint64_t		getOffset( const std::vector<uint64_t> & indexes,
					const std::vector<uint64_t> & dimensions );

	static std::vector<uint64_t>
				initIndex( const std::vector<uint64_t> & dimensions );

	static bool		incIndex( std::vector<uint64_t> & indexes,
					const std::vector<uint64_t> & dimensions );

	static void		setIndex( std::vector<uint64_t> & indexes, uint64_t index );
	static void		setIndex( std::vector<uint64_t> & indexes, uint64_t index0, uint64_t index1 );
	static void		setIndex( std::vector<uint64_t> & indexes, uint64_t index0, uint64_t index1, uint64_t index2 );

private:
	Buffer				m_numericValue;
	std::vector<std::string>	m_stringValue;
	std::vector<uint64_t>		m_dimensions;
	Type				m_type;
};

/////////////////////////////////////////////////////////////////////

#endif
