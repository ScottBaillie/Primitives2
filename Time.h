#ifndef PRIMITIVES2_TIME_H
#define PRIMITIVES2_TIME_H
/////////////////////////////////////////////////////////////////////
//
// Time.h
//
/////////////////////////////////////////////////////////////////////

#include <sys/time.h>
#include <memory>

/////////////////////////////////////////////////////////////////////

class Time {
public:
	Time();
	Time( const Time & t );
	Time( Time && tv );
	Time( double t );
	Time( const timeval & tv );

	void			now();

	double			get() const;
	const timeval &		get_timeval() const;

	void			set( double t );
	void			set( const timeval & tv );

	void			add( double seconds );
	void			sub( double seconds );

	Time &			operator = ( const Time & t );
	Time &			operator = ( Time && t );

	bool			operator < ( const Time & t ) const;
	bool			operator > ( const Time & t ) const;
	bool			operator == ( const Time & t ) const;
	bool			operator != ( const Time & t ) const;

				operator bool () const;

private:
	void			init();

private:
	std::shared_ptr<timeval>	m_timeval;
};


double operator - ( const Time & t0, const Time & t1 );


/////////////////////////////////////////////////////////////////////

#endif
