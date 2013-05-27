#include "ProcessStData.h"
#include "translateMsg.h"
#include "head_info.h"


ProcessStData::ProcessStData(void)
{
}

ProcessStData::ProcessStData( MyQueue * que )
{
	m_pQueue	=	que;
}
ProcessStData::~ProcessStData(void)
{
}

void ProcessStData::readData( int *	terminated )
{
	while( !(*terminated) ){
		boost::shared_ptr<Message>	pMsgTmp	=	m_pQueue->popFront();
		
		translateMsg	transData(pMsgTmp);
		std::queue< boost::shared_ptr<sqlData> >	sqlDataQue	=	transData.getSqlData();
		while ( sqlDataQue.size()!=0 )
		{
			boost::shared_ptr<sqlData>	sqlDataTmp	=	sqlDataQue.front();
			sqlDataQue.pop();
			const	char * insertSQL	=	transData.transInsertSentence(sqlDataTmp);
			sendToDB((unsigned char *)insertSQL);
		}
	
	}//end while
	
}