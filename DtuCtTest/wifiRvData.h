#pragma once

#include <boost/asio.hpp>
#include "MyQueue.h"

class wifiRvData
{
public:
	wifiRvData(MyQueue *queue);
	~wifiRvData(void);

	ULONG	ReceiveData(void);
  void  stop();

protected:
	boost::asio::io_service  m_myIoService;
	MyQueue		*			m_myQueue;
};
