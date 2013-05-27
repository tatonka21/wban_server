#pragma once
#include <windows.h>
#include "wcomm_dll.h"
#include "head_info.h"
#include "Message.h"
#include "MyQueue.h"
#include <iostream>



class ProcessRvData
{
public:
	ProcessRvData(void);
	ProcessRvData(data_record *dr,user_info *ui,MyQueue* dataQueue);
	~ProcessRvData(void);
	ULONG		ReceiveData( int * terminated );	  

protected:
	void	refreshUserTable();
	data_record		*m_dr;
	user_info		*m_ui;
	MyQueue			*m_myQueue;
	FILE *			m_pFileHandle;
};
