#ifndef PRIMITIVES2_LOGGER_H
#define PRIMITIVES2_LOGGER_H
/////////////////////////////////////////////////////////////////////
//
// Logger.h
//
/////////////////////////////////////////////////////////////////////

#include <memory>
#include <fstream>

#include "String.h"

/////////////////////////////////////////////////////////////////////

enum LogLevel {
	eLogInfo,
	eLogNotice,
	eLogWarning,
	eLogError
};

/////////////////////////////////////////////////////////////////////

class LogParam : public StrParam {
public:
	LogParam();
	LogParam( LogLevel logLevel, const std::string & line );
	~LogParam();

	template<class T> LogParam & operator()( const T & val )
	{
		StrParam::operator()(val);
		return( *this );
	}

	LogParam & operator()( const char * val );
	LogParam & operator()( const std::string & val );

private:
	LogLevel	m_logLevel;
};

/////////////////////////////////////////////////////////////////////

class Logger {
public:

	friend class LogParam;

	static void setLogFile( const std::string & fileName );

	static void setLogLevel( LogLevel level );

	static LogParam logInfo( const std::string & line );
	static LogParam logNotice( const std::string & line );
	static LogParam logWarning( const std::string & line );
	static LogParam logError( const std::string & line );

private:
	static LogParam log( LogLevel level, const std::string & line );
	static void writeLine( const std::string & line );

private:
	static LogLevel				m_logLevel;
	static std::shared_ptr<std::ofstream>	m_logFile;
};

/////////////////////////////////////////////////////////////////////

#endif
