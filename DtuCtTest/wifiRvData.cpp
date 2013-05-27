#include "wifiRvData.h"
#include "clientSession.h"
#include "serverApp.h"

wifiRvData::wifiRvData( MyQueue *queue )
{
	m_myQueue	=	queue;
}

wifiRvData::~wifiRvData(void)
{
}

ULONG wifiRvData::ReceiveData( void )
{
	
	short port = 8100/*argv[1]*/;
	//我们用的是inet4

	tcp::endpoint endPoint(tcp::v4(),port);
	//终端（可以看作sockaddr_in）完成后，就要accept了

	serverApp sa(m_myIoService,endPoint,m_myQueue);
	//数据收发逻辑

	m_myIoService.run();
	return 0;
}

void wifiRvData::stop()
{
  m_myIoService.stop();
}

