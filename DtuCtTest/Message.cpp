#pragma once
#include <boost/shared_ptr.hpp>
#include <string.h>
#include "Message.h"

Message::Message(void)
{
	//m_sptrDataRecord	=	NULL;
}

Message::Message( data_record	dr )
{
	boost::shared_ptr<data_record>	p(new data_record());    
	memcpy(p.get(), &dr, sizeof(dr));

  //*p					=	dr;
	m_sptrDataRecord	=	p;
}
Message::~Message(void)
{
}

char	* Message::getDataBuf()
{
	return	m_sptrDataRecord->m_data_buf;
}

uint16 Message::getDataLen()
{
	return	m_sptrDataRecord->m_data_len;
}

char	* Message::getRvTime()
{
	return	m_sptrDataRecord->m_recv_date;
}