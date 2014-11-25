/////////////////////////////////////////////////////////////////////
//
// Event.cc
//
/////////////////////////////////////////////////////////////////////

#include "Event.h"
#include "Exception.h"

using namespace std;

/////////////////////////////////////////////////////////////////////

EventDispatcher::EventDispatcher()
{
	m_id = 0;
}

void EventDispatcher::clear()
{
	m_hlrMap.clear();
}

uint64_t EventDispatcher::addHandler( EventHandler & hlr )
{
	m_id++;
	m_hlrMap[m_id] = &hlr;
	return(m_id);
}

void EventDispatcher::removeHandler( uint64_t hlr_id )
{
	auto i = m_hlrMap.find(hlr_id);
	if ( i == m_hlrMap.end() )
		throw Primitives2Exception("Handler not found");

	m_hlrMap.erase(i);
}

bool EventDispatcher::invoke( EventData & data )
{
	bool ret = true;
	bool ret2;

	for ( auto i : m_hlrMap ) {
		ret2 = i.second->event(data);
		if (!ret2) ret = false;
	}

	return(ret);
}


