#ifndef PRIMITIVES2_CONVERTBINARY_H
#define PRIMITIVES2_CONVERTBINARY_H
/////////////////////////////////////////////////////////////////////
//
// ConvertBinary.h
//
/////////////////////////////////////////////////////////////////////

#include "DataField.h"

/////////////////////////////////////////////////////////////////////
//
// Header Definition
// -----------------
// Header       : 0x1836 0x00ff 0xe7ca
// Version      : 0x0001
// NumFields    : 64 bits
//
// Field Definition
// ----------------
// NumItems     : 64 bits
// NameLength   : 16 bits
// NumDimension : 32 bits
// Type         : ?
// Dimensions   : Array of 64 bit values
// Name         : 1 -> 65535 bytes
// Items        : 
//
// Numeric Item Definition
// -----------------------
// uint8_t      : 8 bits
//
// String Item Definition
// ----------------------
// Length       : 64 bits
// Data         : 
//
/////////////////////////////////////////////////////////////////////

struct BinaryHeader {
	uint16_t	m_header[3];
	uint16_t	m_version;
	uint64_t	m_numFields;
};

struct BinaryField {
	uint64_t	m_numItems;
	uint16_t	m_nameLength;
	uint32_t	m_numDimensions;
	DataField::Type	m_type;
};

/////////////////////////////////////////////////////////////////////

#endif
