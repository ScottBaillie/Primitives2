/////////////////////////////////////////////////////////////////////
//
// Process.cc
//
/////////////////////////////////////////////////////////////////////

#include "Process.h"
#include "Exception.h"

using namespace std;

/////////////////////////////////////////////////////////////////////

int Process::run( const string& command, vector<string>& outputLines )
{
	FILE *stream = 0;
	char *lineptr = 0;
	size_t n = 0;
	ssize_t numchar;
	int returnStatus;

	stream = ::popen( command.c_str(), "r" );
	if ( ! stream )
		throw Primitives2Exception("Error from popen()");

	while ( ! ::feof( stream ) ) {
		numchar = ::getline( &lineptr, &n, stream );
		if ( numchar < 1 ) continue;
		string s( lineptr, numchar - 1 );
		outputLines.push_back( s );
	}

	returnStatus = ::pclose( stream );

	return( returnStatus );
}

/////////////////////////////////////////////////////////////////////


