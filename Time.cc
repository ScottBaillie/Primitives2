/////////////////////////////////////////////////////////////////////
//
// Time.cc
//
/////////////////////////////////////////////////////////////////////

#include "Time.h"
#include "Exception.h"

using namespace std;

/////////////////////////////////////////////////////////////////////

void Time::init()
{
	if ( ! m_timeval ) m_timeval.reset( new timeval );
}

Time::Time()
{
}

Time::Time( const Time & t )
{
	*this = t;
}

Time::Time( Time && t )
{
	*this = t;
}

Time::Time( double t )
{
	set(t);
}

Time::Time( const timeval & tv )
{
	set(tv);
}

void Time::now()
{
	timeval tv;
	int ret = ::gettimeofday( &tv, NULL );

	if ( ret != 0 )
		throw Primitives2Exception("Error from gettimeofday()");

	init();
	*m_timeval = tv;
}

double Time::get() const
{
	if ( ! m_timeval )
		throw Primitives2Exception("Not initialised");

	timeval & tv = *m_timeval;
	double t = tv.tv_sec;
	t += ( tv.tv_usec / 1000000.0 );

	return(t);
}

const timeval & Time::get_timeval() const
{
	if ( ! m_timeval )
		throw Primitives2Exception("Not initialised");
	return(*m_timeval);
}

void Time::set( double t )
{
	init();
	timeval & tv = *m_timeval;
	tv.tv_sec = t;
	tv.tv_usec = 1000000 * ( t - (double)((uint64_t)t) );
}

void Time::set( const timeval & tv )
{
	init();
	*m_timeval = tv;
}

void Time::add( double seconds )
{
	if ( ! m_timeval )
		throw Primitives2Exception("Not initialised");

	set( get() + seconds );
}

void Time::sub( double seconds )
{
	if ( ! m_timeval )
		throw Primitives2Exception("Not initialised");

	set( get() - seconds );
}

Time & Time::operator = ( const Time & t )
{
	if (!t.m_timeval) {
		m_timeval.reset();
		return( *this );
	}
	init();
	*m_timeval = *t.m_timeval;
	return( *this );
}

Time & Time::operator = ( Time && t )
{
	m_timeval.reset();
	m_timeval = std::move( t.m_timeval );
	return( *this );
}

bool Time::operator < ( const Time & t ) const
{
	if ( (!m_timeval) || (!t.m_timeval) )
		throw Primitives2Exception("Not initialised");

	return( get() < t.get() );
}

bool Time::operator > ( const Time & t ) const
{
	if ( (!m_timeval) || (!t.m_timeval) )
		throw Primitives2Exception("Not initialised");

	return( get() > t.get() );
}

bool Time::operator == ( const Time & t ) const
{
	if ( (!m_timeval) || (!t.m_timeval) )
		throw Primitives2Exception("Not initialised");

	bool ret = ( m_timeval->tv_sec == t.m_timeval->tv_sec ) &&
			( m_timeval->tv_usec == t.m_timeval->tv_usec );
	return( ret );
}

bool Time::operator != ( const Time & t ) const
{
	return( ! operator==(t) );
}

Time::operator bool () const
{
	return( (bool)m_timeval );
}


double operator - ( const Time & t0, const Time & t1 )
{
	return( t0.get() - t1.get() );
}


