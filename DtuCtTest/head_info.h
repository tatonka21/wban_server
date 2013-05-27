#pragma once
#include	<sql.h> 
#include	<sqlext.h> 
#include	<sqltypes.h> 
#include	<stdio.h>
#include	<Odbcss.h>
#define		MAXBUFLEN	512

#define	USERID	"13071218320"

void	initialize();


/// 显示错误信息或相关结果信息
void	ShowMess( char *mess ); 

void	stopService();

void	sendToDB(unsigned char * str);

void getNowTime(char (&str)[20]);
