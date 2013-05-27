#pragma once
#include "MyQueue.h"


class ProcessStData
{
public:
	ProcessStData(void);
	ProcessStData(MyQueue * que);
	~ProcessStData(void);

	void	readData( int * terminated );

protected:
	MyQueue	*	m_pQueue;
};
