#pragma once
#include <boost/shared_ptr.hpp>
#include <Windows.h>
#include "wcomm_dll.h"



/// message�����Ȱ�data_record��������ݴ�����
class Message
{
public:
	Message(void);
	~Message(void);
	Message(data_record	dr);
	char	*	getDataBuf();
	uint16		getDataLen();
	char	*	getRvTime();

protected:
	boost::shared_ptr<data_record>	m_sptrDataRecord;        
};
