#pragma once
#include <stdlib.h>
#include "head_info.h"


///����ļ�������̫������

void ShowMess( char *mess )       //���������������Ҫ�׳��쳣�� ����ȱ����Ӧ��ͷ�ļ�
{
	throw std::exception(mess);
}
void initialize()
{
	char mess[512];
	int iResult;
	DWORD	thid;         ///������ͣ���Ҫwindowsͷ�ļ�����

	//ѡ�������ģʽ
	SetWorkMode(1);
	//ѡ��UDPЭ��
	SelectProtocol(0);
	//����IP��ַ
	SetCustomIP(inet_addr("192.168.0.1"));	
	//��������
	iResult=start_net_service(NULL, 0, 5002, mess);	
	//��ʾ�������̵���Ϣ
	ShowMess(mess);

}