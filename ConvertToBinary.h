#ifndef PRIMITIVES2_CONVERTTOBINARY_H
#define PRIMITIVES2_CONVERTTOBINARY_H
/////////////////////////////////////////////////////////////////////
//
// ConvertToBinary.h
//
/////////////////////////////////////////////////////////////////////

#include "DataMap.h"

/////////////////////////////////////////////////////////////////////

class ConvertToBinary {
public:

	static uint64_t getLength( const DataMap & data );

	// Returns the length of the binary data.
	static uint64_t toBinary( const DataMap & data, Buffer & buf );

private:

	static uint64_t toBinary( const DataMap & data, Buffer & buf, bool write );

	static uint8_t * valueToBinary(
				const DataField & f,
				bool write,
				uint8_t * pStart );

	static uint8_t * valueToBinaryNumeric(
				const DataField & f,
				bool write,
				uint8_t * pStart );

	static uint8_t * valueToBinaryString(
				const DataField & f,
				bool write,
				uint8_t * pStart );
};

/////////////////////////////////////////////////////////////////////

#endif
