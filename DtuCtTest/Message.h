#pragma once
#include <boost/shared_ptr.hpp>
#include <Windows.h>
#include "wcomm_dll.h"



/// message负责先把data_record里面的内容存下来
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
