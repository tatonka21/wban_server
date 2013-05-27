#pragma once
#include <queue>
#include "Message.h"
#include "windows_semaphore.h"
#include <boost/noncopyable.hpp>
#include <boost/thread.hpp>

/// ���У���message�浽�����У��Ա㴦�����ݵ��߳�ʹ�á�
class MyQueue 
{
protected:
	//MyQueue ���ܱ�����
	MyQueue(const MyQueue &){}
	MyQueue & operator= (const MyQueue &) {return *this;}
public:
	MyQueue(void);
	~MyQueue(void);

	void							pushBack(boost::shared_ptr<Message> ptrMsg);
	boost::shared_ptr<Message>		popFront();		//���ӣ��������˳����Ǹ�����
	bool							empty();
	size_t							size() const;

protected:
	typedef std::queue< boost::shared_ptr<Message> > QueueType_t;
	QueueType_t			msgQueue;     
	utl::semaphore		s;       
	mutable boost::mutex		m_oMutex;
};
