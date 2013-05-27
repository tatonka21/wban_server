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
	//�����õ���inet4

	tcp::endpoint endPoint(tcp::v4(),port);
	//�նˣ����Կ���sockaddr_in����ɺ󣬾�Ҫaccept��

	serverApp sa(m_myIoService,endPoint,m_myQueue);
	//�����շ��߼�

	m_myIoService.run();
	return 0;
}

void wifiRvData::stop()
{
  m_myIoService.stop();
}

