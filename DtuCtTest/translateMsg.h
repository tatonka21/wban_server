#pragma once
#include <boost/shared_ptr.hpp>
#include <queue>
#include "Message.h"
#pragma comment(lib,"winmm.lib")

typedef	struct  
{
	DWORD		m_iKeyID;
	int			m_cUserID;
	char		m_cSensorType[8];
	char		m_sData[256];
	char		m_sRvTime[20];
}sqlData;

class translateMsg
{
public:
	translateMsg(void);
	translateMsg( boost::shared_ptr<Message> pMsg );
	~translateMsg(void);
	
	std::queue< boost::shared_ptr<sqlData> >		getSqlData();
	const char	*		transInsertSentence(boost::shared_ptr<sqlData> pSqlData);
	

protected:
	boost::shared_ptr<Message>	m_pMsg;
	std::queue< boost::shared_ptr<sqlData> >	m_pqSqlData;
	std::string					m_strSqlRes;
};
