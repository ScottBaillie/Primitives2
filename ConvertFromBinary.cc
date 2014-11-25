/////////////////////////////////////////////////////////////////////
//
// ConvertFromBinary.cc
//
/////////////////////////////////////////////////////////////////////

#include "Exception.h"
#include "ConvertFromBinary.h"

using namespace std;

/////////////////////////////////////////////////////////////////////

DataMap ConvertFromBinary::fromBinary( const Buffer & buf )
{
	if ( buf.size() < sizeof(BinaryHeader) )
		throw Primitives2Exception("ConvertFromBinary::fromBinary() : Buffer is too small");

	BinaryHeader * pHeader = (BinaryHeader *)&buf[0];

	if ( ( pHeader->m_header[0] != 0x1836 ) ||
		( pHeader->m_header[1] !=0x00ff  ) ||
		( pHeader->m_header[2] != 0xe7ca ) ||
		 ( pHeader->m_version == 0 ) )
		throw Primitives2Exception("ConvertFromBinary::fromBinary() : Invalid header");

	uint64_t u0;
	uint64_t u1;
	uint64_t * pDimArray;
	uint8_t * pName;
	uint8_t * pData;
	vector<uint64_t> dim;
	DataMap dm;
	BinaryField * pField = (BinaryField *)(pHeader+1);

	for ( u0 = 0; u0 < pHeader->m_numFields; u0++ ) {
		dim.resize( pField->m_numDimensions );
		pDimArray = (uint64_t *)(pField+1);
		for ( u1 = 0; u1 < pField->m_numDimensions; u1++ )
			dim[u1] = pDimArray[u1];
		pName = (uint8_t *)(pDimArray+pField->m_numDimensions);
		string name( (const char *)pName, pField->m_nameLength );
		pData = pName + pField->m_nameLength;

		pData = valueFromBinary( dm, name, dim, pField->m_type, pData );

		pField = (BinaryField *)pData;
	}

	return( dm );
}

/////////////////////////////////////////////////////////////////////

uint8_t * ConvertFromBinary::valueFromBinary( DataMap & dm,
					const string & name,
					const vector<uint64_t> & dim,
					DataField::Type type,
					uint8_t * pData )
{
	switch (type) {
		case DataField::eUint8 :
			dm.createUint8( name, 0, dim );
			return(valueFromBinaryNumeric( dm.find( name ), pData ));
		case DataField::eUint16 :
			dm.createUint16( name, 0, dim );
			return(valueFromBinaryNumeric( dm.find( name ), pData ));
		case DataField::eUint32 :
			dm.createUint32( name, 0, dim );
			return(valueFromBinaryNumeric( dm.find( name ), pData ));
		case DataField::eUint64 :
			dm.createUint64( name, 0, dim );
			return(valueFromBinaryNumeric( dm.find( name ), pData ));
		case DataField::eInt8 :
			dm.createInt8( name, 0, dim );
			return(valueFromBinaryNumeric( dm.find( name ), pData ));
		case DataField::eInt16 :
			dm.createInt16( name, 0, dim );
			return(valueFromBinaryNumeric( dm.find( name ), pData ));
		case DataField::eInt32 :
			dm.createInt32( name, 0, dim );
			return(valueFromBinaryNumeric( dm.find( name ), pData ));
		case DataField::eInt64 :
			dm.createInt64( name, 0, dim );
			return(valueFromBinaryNumeric( dm.find( name ), pData ));
		case DataField::eDouble :
			dm.createDouble( name, 0, dim );
			return(valueFromBinaryNumeric( dm.find( name ), pData ));
		case DataField::eString :
			dm.createString( name, "", dim );
			return(valueFromBinaryString( dm.find( name ), pData ));
	}

	throw Primitives2Exception("ConvertFromBinary::valueFromBinary() : Invalid type");

	return(pData);
}

uint8_t * ConvertFromBinary::valueFromBinaryNumeric( DataField & f, uint8_t * pData )
{
	Buffer buf1( pData, f.m_numericValue.size() );
	f.m_numericValue = buf1;
	return( pData + f.m_numericValue.size() );
}

uint8_t * ConvertFromBinary::valueFromBinaryString( DataField & f, uint8_t * pData )
{
	uint64_t * pLength = (uint64_t *)pData;
	uint8_t * pValue;

	for ( auto & i : f.m_stringValue ) {
		pValue = (uint8_t *)(pLength+1);
		i = string( (const char *)pValue, *pLength );
		pLength = (uint64_t *)(pValue + *pLength );
	}

	return( (uint8_t *)pLength );
}

/////////////////////////////////////////////////////////////////////


