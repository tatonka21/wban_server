#pragma once
#include <stdlib.h>
#include "head_info.h"


///这个文件的命名太不好了

void ShowMess( char *mess )       //这个函数。。。不要抛出异常， 而且缺少相应的头文件
{
	throw std::exception(mess);
}
void initialize()
{
	char mess[512];
	int iResult;
	DWORD	thid;         ///这个类型，需要windows头文件！！

	//选择非阻塞模式
	SetWorkMode(1);
	//选择UDP协议
	SelectProtocol(0);
	//设置IP地址
	SetCustomIP(inet_addr("192.168.0.1"));	
	//启动服务
	iResult=start_net_service(NULL, 0, 5002, mess);	
	//显示启动过程的信息
	ShowMess(mess);

}