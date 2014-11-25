/////////////////////////////////////////////////////////////////////
//
// Main.cc
//
/////////////////////////////////////////////////////////////////////

#include <iostream>

#include "TestMessageRouter.h"
#include "Logger.h"


using namespace std;

/////////////////////////////////////////////////////////////////////

#if 0

    TestServerSocket tss;
    tss.runTest();

    /////////////

    TestServerSocket2 tss;
    tss.runTest();

    /////////////

	vector<string> outputLines;
	int ret = Process::run( "ls -al /etc", outputLines );

	for ( auto i : outputLines ) cout << i << "\n";

	Time t0;
	Time t1;
	t0.now();
	t1 = t0;
	t1.add( 10.0 );
	double diff = t1 - t0;
	cout << "diff=" << diff << "\n";

	Logger::setLogFile( "logger.log" );

	Logger::setLogLevel( eLogInfo );

	Logger::logError("test string1");

	uint64_t val1 = 12;
	int64_t val2 = -34;
	string s0 = "abcd";
	double d0 = 1.2345;
	Logger::logError("test string2 : val1=")(val1)(" : val2=")(val2)(" : s0=")(s0)(" : d0=")(d0);

	string str0 = StrParam("StrParam : val1=")(val1)(" : val2=")(val2);
	Logger::logError(str0);

	Message m0;

	DataMap dm0;

	dm0.createUint8( "uint8value", 0, 4 );
	dm0.setUint8( "uint8value", 12, 0 );
	dm0.setUint8( "uint8value", 34, 1 );
	dm0.setUint8( "uint8value", 56, 2 );
	dm0.setUint8( "uint8value", 78, 3 );

	dm0.createString( "stringvalue", "", 4 );
	dm0.setString( "stringvalue", "str0", 0 );
	dm0.setString( "stringvalue", "str1", 1 );
	dm0.setString( "stringvalue", "str2", 2 );
	dm0.setString( "stringvalue", "str3", 3 );

	uint64_t length = ConvertToBinary::getLength( dm0 );

	Buffer buf( length );

	ConvertToBinary::toBinary( dm0, buf );

	DataMap dm1 = ConvertFromBinary::fromBinary( buf );

	cout << "value : " << dm1.getString( "stringvalue", 0 ) << "\n";
	cout << "value : " << dm1.getString( "stringvalue", 1 ) << "\n";
	cout << "value : " << dm1.getString( "stringvalue", 2 ) << "\n";
	cout << "value : " << dm1.getString( "stringvalue", 3 ) << "\n";

	cout << "value : " << (uint64_t)dm1.getUint8( "uint8value", 0 ) << "\n";
	cout << "value : " << (uint64_t)dm1.getUint8( "uint8value", 1 ) << "\n";
	cout << "value : " << (uint64_t)dm1.getUint8( "uint8value", 2 ) << "\n";
	cout << "value : " << (uint64_t)dm1.getUint8( "uint8value", 3 ) << "\n";

    /////////////

#endif

int main( int argc, char* argv[] )
{
  int ret = 0;

  try {

    bool client = false;

#ifdef MSG_CLIENT
    client = true;
#endif

    if (client) {
        Logger::setLogFile( "Test-client.log" );
        TestMessageRouter tmr;
        tmr.runClient();
    } else {
        Logger::setLogFile( "Test-server.log" );
        TestMessageRouter tmr;
        tmr.runServer();
    }

  }
  catch (const std::exception &e)
  {
    ret = 2;
    cerr << "std exception: " << e.what() << endl;
  }
  catch (...)
  {
    ret = 1;
    cerr << "Unhandled exception" << endl;
  }
  if ( ret != 0 ) {
    cout << "Exited with an error.\n";
  } else {
    cout << "OK.\n";
  }
  return(ret);
} 

/////////////////////////////////////////////////////////////////////


