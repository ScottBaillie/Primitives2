#ifndef PRIMITIVES2_EVENT_H
#define PRIMITIVES2_EVENT_H
/////////////////////////////////////////////////////////////////////
//
// Event.h
//
/////////////////////////////////////////////////////////////////////

#include <map>

/////////////////////////////////////////////////////////////////////

class EventData {
public:
	virtual ~EventData() {}
};

class EventHandler {
public:
	virtual ~EventHandler() {}

	virtual bool event( EventData & data ) = 0;
};

template<class T> class EventRelay : public EventHandler {
public:
	typedef bool (T::*FnPtr)( EventData & );

	EventRelay( T & obj, FnPtr fp ) : m_obj(obj),m_fnPtr(fp) {}

	virtual bool event( EventData & data )
	{
		return( (m_obj.*m_fnPtr)(data) );
	}

private:
	T &	m_obj;
	FnPtr	m_fnPtr;
};

class EventDispatcher {
public:

	EventDispatcher();

	void clear();

	uint64_t addHandler( EventHandler & hlr );
	void removeHandler( uint64_t hlr_id );

	bool invoke( EventData & data );

private:
	uint64_t				m_id;
	std::map<uint64_t,EventHandler*>	m_hlrMap;
};

/////////////////////////////////////////////////////////////////////

#endif
