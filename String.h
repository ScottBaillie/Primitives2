#ifndef PRIMITIVES2_STRING_H
#define PRIMITIVES2_STRING_H
/////////////////////////////////////////////////////////////////////
//
// String.h
//
/////////////////////////////////////////////////////////////////////

#include <string>
#include <vector>

/////////////////////////////////////////////////////////////////////

class StrParam {
public:
	StrParam();
	StrParam( const std::string & line );

	template<class T> StrParam & operator()( const T & val )
	{
		if (!m_enabled) return( *this );
		m_line += std::to_string(val);
		return( *this );
	}

	StrParam & operator()( const char * val );
	StrParam & operator()( const std::string & val );

	operator std::string () const;

protected:
	bool		m_enabled;
	std::string	m_line;
};

/////////////////////////////////////////////////////////////////////

class Str {
public:

	static std::vector<std::string>	getToken(
					const std::string & line,
					const std::string & sep_pat = "\\s",
					const std::string & token_pat = "\\S+" );

	static std::vector<std::string>	getMatch(
					const std::string & line,
					const std::string & pat );

	static bool match( const std::string & line, const std::string & pat );

};

/////////////////////////////////////////////////////////////////////

#endif
