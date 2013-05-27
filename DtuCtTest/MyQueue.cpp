#include "MyQueue.h"


MyQueue::MyQueue(void)
{
}

MyQueue::~MyQueue(void)
{
}

void MyQueue::pushBack( boost::shared_ptr<Message> ptrMsg )
{
	m_oMutex.lock();
	msgQueue.push(ptrMsg);
	m_oMutex.unlock();

	s.V();
}

boost::shared_ptr<Message> MyQueue::popFront()
{
	s.P();
	boost::shared_ptr<Message>	p;

	m_oMutex.lock();
	p	=	msgQueue.front();
	msgQueue.pop();
	m_oMutex.unlock();
	return p;
}

bool MyQueue::empty()
{
	m_oMutex.lock();
	bool flag =	msgQueue.empty();
	m_oMutex.unlock();
	return flag;
}

size_t MyQueue::size() const
{
	m_oMutex.lock();
	QueueType_t::size_type s = msgQueue.size();
	m_oMutex.unlock();
	return static_cast<size_t>(s);
}