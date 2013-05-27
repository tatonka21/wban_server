#include "translateMsg.h"
#include <string>
#include <sstream>
#include <iostream>

#pragma warning(disable:4996)
//#include <afx.h>

translateMsg::translateMsg(void)
{
}

translateMsg::translateMsg( boost::shared_ptr<Message> pMsg )
{
	m_pMsg	=	pMsg;
}
translateMsg::~translateMsg(void)
{
}

std::queue< boost::shared_ptr<sqlData> >		translateMsg::getSqlData()
{
	char	*	pDataBuf	=	m_pMsg->getDataBuf();
	uint16		datalen		=	m_pMsg->getDataLen();
	char	cTmp;
	int   kFlag=0;

	for (uint16 i=0;i<datalen;)
	{	//查找一个DTU包里面的有效数据，并解析

		if ( (pDataBuf[i]==0x41) && (pDataBuf[i+1]==0x53) && (pDataBuf[i+2]!=0) )
		{	//解析数据

			boost::shared_ptr<sqlData>	pSqlData(new sqlData());
			pSqlData->m_iKeyID		=	timeGetTime();//数据编号 ，key
			pSqlData->m_cUserID		=	pDataBuf[i+4];// 用户ID,直接存储
			cTmp	=	pDataBuf[i+2];//传感器类型
			switch( cTmp )
			{
			case 0x20://血压数据
				strcpy(pSqlData->m_cSensorType,"血压");	//warning unsafe
				break;
			case 0x30://血氧数据
				strcpy(pSqlData->m_cSensorType,"血氧");
				break;
			case 0x40://心率数据
				strcpy(pSqlData->m_cSensorType,"心率");
				break;
			case 0x41://心电数据
				strcpy(pSqlData->m_cSensorType,"心电");
				break;
			case 0x10://体温数据
				strcpy(pSqlData->m_cSensorType,"体温");
				break;
			case 0x60://计步数据
				strcpy(pSqlData->m_cSensorType,"计步");
				break;
			default:
				std::cout<<"Unknown sensor type !"<<std::endl;
				break;
			}
			uint16	uDataLen		=	pDataBuf[i+3];
			memcpy(pSqlData->m_sData,&pDataBuf[i+8],uDataLen);
			pDataBuf	=	m_pMsg->getRvTime();	//借用前面char指针，来获得接收时间
			memcpy(pSqlData->m_sRvTime,pDataBuf,20);	
			
			if ( kFlag==0||pDataBuf[i+5]!=kFlag )
			{
				m_pqSqlData.push(pSqlData);
				//std::cout<<"解析数据-"<<"编号:"<<pSqlData->m_iKeyID<<"用户ID:"<<pSqlData->m_cUserID<<"传感器类型:"<<pSqlData->m_cSensorType<<"值:"<<pSqlData->m_sData<<" "<<pSqlData->m_sRvTime<<std::endl;
				Sleep(1);
				kFlag = pDataBuf[i+5];
			}
			i	=	i+uDataLen+8;
		}
		else
		{
			++i;
		}//end if
	
	}//end for

	//这里写返回值。
	return	m_pqSqlData;
	
}

const char	* translateMsg::transInsertSentence( boost::shared_ptr<sqlData> pSqlData )
{
	/*CString	strSqlInst;
	strSqlInst.Format("insert into Table_1 values(%d,%u,\'%s\',\'%s\',\'%s\')",pSqlData->m_iKeyID,pSqlData->m_cUserID,pSqlData->m_cSensorType,pSqlData->m_sData,pSqlData->m_sRvTime);
	return	(char *)strSqlInst;*/
	//error!!!!!
	std::stringstream str;
	str<<"insert into Table_1 values(" <<pSqlData->m_iKeyID << ","<<pSqlData->m_cUserID << ",\'"<<pSqlData->m_cSensorType<<"\',\'"<<pSqlData->m_sData << "\',\'" << pSqlData->m_sRvTime << "\')";
	m_strSqlRes = str.str();
	std::cout<<m_strSqlRes<<std::endl;

	return	m_strSqlRes.c_str();
}