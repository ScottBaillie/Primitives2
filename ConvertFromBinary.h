#ifndef PRIMITIVES2_CONVERTFROMBINARY_H
#define PRIMITIVES2_CONVERTFROMBINARY_H
/////////////////////////////////////////////////////////////////////
//
// ConvertFromBinary.h
//
/////////////////////////////////////////////////////////////////////

#include "DataMap.h"
#include "ConvertBinary.h"

/////////////////////////////////////////////////////////////////////

class ConvertFromBinary {
public:

	static DataMap fromBinary( const Buffer & buf );

private:

	static uint8_t * valueFromBinary( DataMap & dm,
					const std::string & name,
					const std::vector<uint64_t> & dim,
					DataField::Type type,
					uint8_t * pData );

	static uint8_t * valueFromBinaryNumeric( DataField & f, uint8_t * pData );
	static uint8_t * valueFromBinaryString( DataField & f, uint8_t * pData );
};

/////////////////////////////////////////////////////////////////////

#endif
