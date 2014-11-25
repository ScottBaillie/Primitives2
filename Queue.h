#ifndef PRIMITIVES2_QUEUE_H
#define PRIMITIVES2_QUEUE_H
/////////////////////////////////////////////////////////////////////
//
// Queue.h
//
/////////////////////////////////////////////////////////////////////

#include <list>
#include <mutex>
#include <memory>
#include <condition_variable>

/////////////////////////////////////////////////////////////////////


template<class T> class Queue {

public:
	Queue() : m_stopped(false) {}

	void	add( std::shared_ptr<T> item )
	{
		std::unique_lock<std::mutex> lock1(m_mutex);
		m_queue.push_back( item );
		if (m_queue.size()==1)
			if (m_notEmptyCond) m_notEmptyCond->notify_all();
	}

	bool	get( std::shared_ptr<T> & item )
	{
		std::unique_lock<std::mutex> lock1(m_mutex);
		while (true) {
			if ( m_queue.size() != 0 ) {
				m_notEmptyCond.reset();
				break;
			}
			if ( m_stopped ) {
				m_notEmptyCond.reset();
				return(false);
			}
			if (!m_notEmptyCond) m_notEmptyCond.reset(new std::condition_variable());
			m_notEmptyCond->wait_for(lock1,std::chrono::milliseconds(500));
		}
		item = m_queue.front();
		m_queue.pop_front();
		return(true);
	}

	void	stop()
	{
		m_stopped = true;
	}

private:
	std::list<std::shared_ptr<T> >			m_queue;
	std::mutex					m_mutex;
	std::shared_ptr<std::condition_variable>	m_notEmptyCond;
	bool						m_stopped;
};


/////////////////////////////////////////////////////////////////////

#endif
