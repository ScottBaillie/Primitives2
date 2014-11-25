/////////////////////////////////////////////////////////////////////
//
// String.cc
//
/////////////////////////////////////////////////////////////////////

#include <regex>

#include "String.h"
#include "Exception.h"

using namespace std;

/////////////////////////////////////////////////////////////////////

StrParam::StrParam()
{
	m_enabled = false;
}

StrParam::StrParam( const std::string & line )
{
	m_line = line;
	m_enabled = true;
}

StrParam & StrParam::operator()( const char * val )
{
	if (!m_enabled) return( *this );
	m_line += string(val);
	return( *this );
}

StrParam & StrParam::operator()( const string & val )
{
	if (!m_enabled) return( *this );
	m_line += val;
	return( *this );
}

StrParam::operator string () const
{
	return(m_line);
}

/////////////////////////////////////////////////////////////////////

vector<string> Str::getToken( const string & line,
				const string & sep_pat,
				const string & token_pat )
{
	string pat = "("+sep_pat+")*("+token_pat+")("+sep_pat+")*(.*)";
	regex e0(pat);
	vector<string> ret;
	string l = line;

	while ( true ) {
		smatch match0;
		if ( regex_match( l, match0, e0 ) ) {
			ret.push_back( match0[2] );
			l = match0[4];
			if ( l.size() == 0 ) break;
		} else {
			break;
		}
	}

	return( ret );
}

vector<string> Str::getMatch( const string & line, const string & pat )
{
	regex e0(pat);
	smatch match0;
	vector<string> ret;

	if ( regex_match( line, match0, e0 ) )
		for ( auto i : match0 ) ret.push_back( i );

	return( ret );
}

bool Str::match( const string & line, const string & pat )
{
	vector<string> ret = getMatch( line, pat );
	if ( ret.size() == 0 ) return( false );
	return( true );
}

/////////////////////////////////////////////////////////////////////


