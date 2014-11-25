/////////////////////////////////////////////////////////////////////
//
// Logger.cc
//
/////////////////////////////////////////////////////////////////////

#include "Logger.h"
#include "Exception.h"

using namespace std;

/////////////////////////////////////////////////////////////////////

std::shared_ptr<std::ofstream>	Logger::m_logFile;
LogLevel Logger::m_logLevel = eLogInfo;

/////////////////////////////////////////////////////////////////////

void Logger::setLogFile( const string & fileName )
{
	if ( fileName.size() == 0 ) {
		m_logFile.reset();
		return;
	}
	m_logFile.reset( new ofstream(fileName) );
}

void Logger::setLogLevel( LogLevel level )
{
	m_logLevel = level;
}

LogParam Logger::log( LogLevel level, const string & line )
{
	return( LogParam(level,line) );
}

LogParam Logger::logInfo( const string & line )
{
	if ( (!m_logFile) || ( m_logLevel > eLogInfo ) )
		return( LogParam() );

	return(log(eLogInfo,line));
}

LogParam Logger::logNotice( const string & line )
{
	if ( (!m_logFile) || ( m_logLevel > eLogNotice ) )
		return( LogParam() );

	return(log(eLogNotice,line));
}

LogParam Logger::logWarning( const string & line )
{
	if ( (!m_logFile) || ( m_logLevel > eLogWarning ) )
		return( LogParam() );

	return(log(eLogWarning,line));
}

LogParam Logger::logError( const string & line )
{
	if (!m_logFile)
		return( LogParam() );

	return(log(eLogError,line));
}

void Logger::writeLine( const string & line )
{
	if (!m_logFile) return;
	*m_logFile << line << "\n";
}

/////////////////////////////////////////////////////////////////////

LogParam::LogParam()
{
}

LogParam::LogParam( LogLevel logLevel, const std::string & line ) : StrParam(line)
{
	m_logLevel = logLevel;
}

LogParam::~LogParam()
{
	if (!m_enabled) return;
	Logger::writeLine( m_line );
}

LogParam & LogParam::operator()( const char * val )
{
	StrParam::operator()(val);
	return( *this );
}

LogParam & LogParam::operator()( const string & val )
{
	StrParam::operator()(val);
	return( *this );
}

/////////////////////////////////////////////////////////////////////

