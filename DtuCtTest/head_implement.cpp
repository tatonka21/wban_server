#pragma once
#include <WinSock2.h>
#include <stdlib.h>
#include <iostream>
#include <Windows.h>
#include <ctime>
#include <string>
#include "wcomm_dll.h"
#include "head_info.h"
#pragma comment(lib,"Ws2_32.lib")

void ShowMess( char *mess )       
{
	//throw std::exception(mess);
	std::cout<<mess<<std::endl;
}
void initialize()
{
	char mess[100];
	int iResult;
	//DWORD	thid;         

	//选择非阻塞模式
	SetWorkMode(1);
	//选择UDP协议
	SelectProtocol(0);
	//设置IP地址,当计算机只有一个IP地址时可以不执行
	SetCustomIP(inet_addr("202.114.14.134"/*"192.168.0.122"*/));	
	//启动服务
	iResult=start_net_service(NULL, 0, 4000, mess);	
	//显示启动过程的信息
	ShowMess(mess);
	if(iResult == 0)
	{
		std::cout<<"start_net_service success!"<<std::endl;
	}
	else{
		std::cout<<"start_net_service failed!"<<std::endl;
	}

}


void stopService()
{
	char	mess[512];
	int		iRes;

	iRes	=	stop_net_service(mess);
	if ( iRes==0 )
	{
		std::cout<<"stop_net_service seccess!"<<std::endl;
	}
	else if ( iRes==-1 )
	{
		std::cout<<"stop_net_service failed or there isn't a net service started !"<<std::endl;
	}
}

void ProcessLogMessages(
						SQLSMALLINT plm_handle_type,
						//出现错误时所使用的ODBC句柄类型，取值为：SQL_HANDLE_ENV ，SQL_HANDLE_DBC ，SQL_HANDLE_STMT
						SQLHANDLE plm_handle, //出现错误时所使用的ODBC句柄
						char *logstring, //标题字符串
						int ConnInd //指明句柄是否为DBC句柄
						)
{
	RETCODE plm_retcode = SQL_SUCCESS;
	UCHAR plm_szSqlState[MAXBUFLEN] = "",
		plm_szErrorMsg[MAXBUFLEN] = "";
	SDWORD plm_pfNativeError = 0L;
	SWORD plm_pcbErrorMsg = 0;
	SQLSMALLINT plm_cRecNmbr = 1;
	SDWORD plm_SS_MsgState = 0, plm_SS_Severity = 0;
	SQLINTEGER plm_Rownumber = 0;
	USHORT plm_SS_Line;
	SQLSMALLINT plm_cbSS_Procname, plm_cbSS_Srvname;
	SQLCHAR plm_SS_Procname[MAXNAME], plm_SS_Srvname[MAXNAME];
	printf(logstring);
	while (plm_retcode != SQL_NO_DATA_FOUND) {
		plm_retcode = SQLGetDiagRec(plm_handle_type, plm_handle,
			plm_cRecNmbr, plm_szSqlState, &plm_pfNativeError,
			plm_szErrorMsg, MAXBUFLEN - 1, &plm_pcbErrorMsg);
		// Note that if the application has not yet made a
		// successful connection, the SQLGetDiagField
		// information has not yet been cached by ODBC
		// Driver Manager and these calls to SQLGetDiagField
		// will fail.
		if (plm_retcode != SQL_NO_DATA_FOUND) {
			if (ConnInd) {
				plm_retcode = SQLGetDiagField(
					plm_handle_type, plm_handle, plm_cRecNmbr,
					SQL_DIAG_ROW_NUMBER, &plm_Rownumber,
					SQL_IS_INTEGER,
					NULL);
				plm_retcode = SQLGetDiagField(
					plm_handle_type, plm_handle, plm_cRecNmbr,
					SQL_DIAG_SS_LINE, &plm_SS_Line,
					SQL_IS_INTEGER,
					NULL);
				plm_retcode = SQLGetDiagField(
					plm_handle_type, plm_handle, plm_cRecNmbr,
					SQL_DIAG_SS_MSGSTATE, &plm_SS_MsgState,
					SQL_IS_INTEGER,
					NULL);
				plm_retcode = SQLGetDiagField(
					plm_handle_type, plm_handle, plm_cRecNmbr,
					SQL_DIAG_SS_SEVERITY, &plm_SS_Severity,
					SQL_IS_INTEGER,
					NULL);
				plm_retcode = SQLGetDiagField(
					plm_handle_type, plm_handle, plm_cRecNmbr,
					SQL_DIAG_SS_PROCNAME, &plm_SS_Procname,
					sizeof(plm_SS_Procname),
					&plm_cbSS_Procname);
				plm_retcode = SQLGetDiagField(
					plm_handle_type, plm_handle, plm_cRecNmbr,
					SQL_DIAG_SS_SRVNAME, &plm_SS_Srvname,
					sizeof(plm_SS_Srvname),
					&plm_cbSS_Srvname);
			}
			printf("szSqlState = %s\n",plm_szSqlState);
			printf("pfNativeError = %d\n",plm_pfNativeError);
			printf("szErrorMsg = %s\n",plm_szErrorMsg);
			printf("pcbErrorMsg = %d\n\n",plm_pcbErrorMsg);
			if (ConnInd) {
				printf("ODBCRowNumber = %d\n", plm_Rownumber);
				printf("SSrvrLine = %d\n", plm_Rownumber);
				printf("SSrvrMsgState = %d\n",plm_SS_MsgState);
				printf("SSrvrSeverity = %d\n",plm_SS_Severity);
				printf("SSrvrProcname = %s\n",plm_SS_Procname);
				printf("SSrvrSrvname = %s\n\n",plm_SS_Srvname);
			}
		}
		plm_cRecNmbr++; //Increment to next diagnostic record.
	} // End while.
}

void sendToDB(unsigned char *str)
{

	SQLHANDLE	hEnv,hDBC,hstmt1;

	SQLAllocHandle(SQL_HANDLE_ENV,NULL,&hEnv);
	SQLSetEnvAttr(hEnv, SQL_ATTR_ODBC_VERSION,(SQLPOINTER) SQL_OV_ODBC3, SQL_IS_INTEGER);
	SQLAllocHandle(SQL_HANDLE_DBC,hEnv,&hDBC);
	//SQLAllocHandle(SQL_HANDLE_STMT,hDBC,&hSTMT);

	SQLCHAR		szInput[20];
	SQLINTEGER	iInput=0,iRowCount=0;
	SQLCHAR	*	szSQL2 = str/*"insert into Table_1 values(1,1,'血氧','63','2011-7-16 10:04')"*/;
	SQLINTEGER	cb1=0,cb2=SQL_NTS;

	SQLRETURN	retcode;
	retcode =	SQLConnect(hDBC, (SQLCHAR*) "WBANDSN", SQL_NTS,(SQLCHAR*) "", SQL_NTS, (SQLCHAR*) "", SQL_NTS);
	
	// insert
	retcode = SQLAllocHandle(SQL_HANDLE_STMT, hDBC, &hstmt1);
	retcode = SQLPrepare(hstmt1,szSQL2,SQL_NTS);
	if ( (retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO) )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt1,"Insert SQLPrepare() Failed\n\n", TRUE);
	}
	retcode = SQLBindParameter(hstmt1, 1, SQL_PARAM_INPUT, SQL_C_LONG,SQL_INTEGER, 0, 0, &iInput, 0, &cb1);
	if ( (retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO) )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt1,"bind para1() Failed\n\n", TRUE);
	}
	retcode = SQLBindParameter(hstmt1, 2, SQL_PARAM_INPUT, SQL_C_CHAR,SQL_VARCHAR, 20, 0, szInput, 20, &cb2);
	if ( (retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO) )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt1,"bind para2() Failed\n\n", TRUE);
	}
	retcode = SQLExecute(hstmt1);
	if ( (retcode != SQL_SUCCESS) && (retcode != SQL_SUCCESS_WITH_INFO) )
	{
		ProcessLogMessages(SQL_HANDLE_STMT, hstmt1,"insert SQLExecute() Failed\n\n", TRUE);
	}
	SQLRowCount(hstmt1,&iRowCount);
	printf("insert effect row count = %d\n", iRowCount);
	SQLFreeHandle(SQL_HANDLE_STMT,hstmt1);
}

void getNowTime(char (&res)[20])
{
	std::time_t	now	=	std::time(0);
	std::string	strTime	=	std::ctime(&now);
	const char	*	time_now	=	strTime.c_str();

	memcpy(res,&time_now[20],4);
	memcpy(&res[8],&time_now[8],11);
	res[4]=res[7]='-';
	switch( time_now[4] )
	{
	case 'F':
		res[5]='0';
		res[6]='2';break;
	case 'S':
		res[5]='0';
		res[6]='9';break;
	case 'O':
		res[5]='1';
		res[6]='0';break;
	case 'N':
		res[5]='1';
		res[6]='1';break;
	case 'D':
		res[5]='1';
		res[6]='2';break;
	case 'J':
		res[5]='0';
		if ( time_now[5]=='a' )
			res[6]='1';
		else if ( time_now[6]=='n' )
			res[6]='6';
		else
			res[6]='7';
		break;
	case 'M':
		res[5]='0';
		if ( time_now[6]=='r' )
			res[6]='3';
		else
			res[6]='5';
		break;
	case 'A':
		res[5]='0';
		if ( time_now[5]=='p' )
			res[6]='4';
		else 
			res[6]='8';
		break;
	}
}
