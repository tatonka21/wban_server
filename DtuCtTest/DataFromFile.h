#pragma once
#include <windows.h>
#include "wcomm_dll.h"
#include "head_info.h"
#include "Message.h"
#include "MyQueue.h"
#include <stdio.h>


class DataFromFile
{
public:
	DataFromFile(data_record *dr,user_info *ui,MyQueue* dataQueue);

	ULONG		ReceiveData( int * terminated );
public:
	~DataFromFile(void);

protected:
	void	refreshUserTable();

	data_record		*m_dr;
	user_info		*m_ui;
	MyQueue			*m_myQueue;
	FILE *			m_pFileHandle;
};
