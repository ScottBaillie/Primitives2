/////////////////////////////////////////////////////////////////////
//
// ConvertToBinary.cc
//
/////////////////////////////////////////////////////////////////////

#include "ConvertToBinary.h"
#include "ConvertBinary.h"
#include "Exception.h"

using namespace std;

/////////////////////////////////////////////////////////////////////

uint64_t ConvertToBinary::getLength( const DataMap & data )
{
	Buffer buf(1);
	return( toBinary( data, buf, false ) );
}

uint64_t ConvertToBinary::toBinary( const DataMap & data, Buffer & buf )
{
	return( toBinary( data, buf, true ) );
}

uint64_t ConvertToBinary::toBinary( const DataMap & data, Buffer & buf, bool write )
{
	vector<string> names = data.getNames();
	uint64_t numFields = names.size();
	uint64_t size = buf.size();

	if ( numFields == 0 ) return( 0 );

	if ( write && ( size < getLength(data) ) ) throw Primitives2Exception(
		"ConvertToBinary::toBinary() : Buffer is too small");

	BinaryHeader * pHeader = (BinaryHeader *)&buf[0];

	if (write) {
		pHeader->m_header[0] = 0x1836;
		pHeader->m_header[1] = 0x00ff;
		pHeader->m_header[2] = 0xe7ca;
		pHeader->m_version = 0x0001;
		pHeader->m_numFields = numFields;
	}

	BinaryField * pField = (BinaryField *)(pHeader+1);
	uint64_t * pDimArray;
	uint8_t * pName;

	for ( auto i : names ) {

		const DataField & f = data.find( i );
		const vector<uint64_t> & dim = f.m_dimensions;

		if (write) {
			pField->m_numItems = DataField::getDimensionProduct( dim );
			pField->m_nameLength = i.size();
			pField->m_numDimensions = dim.size();
			pField->m_type = f.m_type;
		}

		pDimArray = (uint64_t *)( pField + 1 );
		for ( auto j : dim ) {
			if (write) *pDimArray = j;
			pDimArray++;
		}

		pName = (uint8_t *)pDimArray;
		for ( auto j : i ) {
			if (write) *pName = j;
			pName++;
		}

		pName = valueToBinary( f, write, pName );

		pField = (BinaryField *)pName;
	}

	uint64_t len = pName - &buf[0];

	return( len );
}

/////////////////////////////////////////////////////////////////////

uint8_t * ConvertToBinary::valueToBinary(
			const DataField & f,
			bool write,
			uint8_t * pStart )
{
	switch ( f.m_type ) {
		case DataField::eUint8 :
		case DataField::eUint16 :
		case DataField::eUint32 :
		case DataField::eUint64 :
		case DataField::eInt8 :
		case DataField::eInt16 :
		case DataField::eInt32 :
		case DataField::eInt64 :
		case DataField::eDouble :
			return(valueToBinaryNumeric(f,write,pStart));
		case DataField::eString :
			return(valueToBinaryString(f,write,pStart));
	}

	throw Primitives2Exception(
		"ConvertToBinary::valueToBinary() : Invalid type");
}

/////////////////////////////////////////////////////////////////////

uint8_t * ConvertToBinary::valueToBinaryNumeric(
			const DataField & f,
			bool write,
			uint8_t * pStart )
{
	uint64_t size = f.m_numericValue.size();
	Buffer buf( pStart, size );
	if (write) buf = f.m_numericValue;
	return( pStart + size );
}

/////////////////////////////////////////////////////////////////////

uint8_t * ConvertToBinary::valueToBinaryString(
			const DataField & f,
			bool write,
			uint8_t * pStart )
{
	uint64_t size;
	uint8_t * pData;
	uint64_t * pLength = (uint64_t *)pStart;

	for ( auto & i : f.m_stringValue ) {
		size = i.size();
		if (write) *pLength = size;
		if ( size == 0 ) {
			pLength++;
			continue;
		}
		pData = (uint8_t *)(pLength+1);
		Buffer buf( pData, size );
		if (write) buf = i;
		pLength = (uint64_t *)(pData+size);
	}

	return( (uint8_t *)pLength );
}

/////////////////////////////////////////////////////////////////////

