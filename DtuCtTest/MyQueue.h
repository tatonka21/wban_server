#pragma once
#include <queue>
#include "Message.h"
#include "windows_semaphore.h"
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>

/// 队列，把message存到队列中，以便处理数据的线程使用。
class MyQueue 
{
protected:
	//MyQueue 不能被复制
	MyQueue(const MyQueue &){}
	MyQueue & operator= (const MyQueue &) {return *this;}
public:
	MyQueue(void);
	~MyQueue(void);

	void							pushBack(boost::shared_ptr<Message> ptrMsg);
	boost::shared_ptr<Message>		popFront();		//出队，并返回退出的那个对象
	bool							empty();
	size_t							size() const;

protected:
	typedef std::queue< boost::shared_ptr<Message> > QueueType_t;
	QueueType_t			msgQueue;     
	utl::semaphore		s;       
	mutable boost::mutex		m_oMutex;
};
