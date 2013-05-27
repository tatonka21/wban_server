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
	{	//����һ��DTU���������Ч���ݣ�������

		if ( (pDataBuf[i]==0x41) && (pDataBuf[i+1]==0x53) && (pDataBuf[i+2]!=0) )
		{	//��������

			boost::shared_ptr<sqlData>	pSqlData(new sqlData());
			pSqlData->m_iKeyID		=	timeGetTime();//���ݱ�� ��key
			pSqlData->m_cUserID		=	pDataBuf[i+4];// �û�ID,ֱ�Ӵ洢
			cTmp	=	pDataBuf[i+2];//����������
			switch( cTmp )
			{
			case 0x20://Ѫѹ����
				strcpy(pSqlData->m_cSensorType,"Ѫѹ");	//warning unsafe
				break;
			case 0x30://Ѫ������
				strcpy(pSqlData->m_cSensorType,"Ѫ��");
				break;
			case 0x40://��������
				strcpy(pSqlData->m_cSensorType,"����");
				break;
			case 0x41://�ĵ�����
				strcpy(pSqlData->m_cSensorType,"�ĵ�");
				break;
			case 0x10://��������
				strcpy(pSqlData->m_cSensorType,"����");
				break;
			case 0x60://�Ʋ�����
				strcpy(pSqlData->m_cSensorType,"�Ʋ�");
				break;
			default:
				std::cout<<"Unknown sensor type !"<<std::endl;
				break;
			}
			uint16	uDataLen		=	pDataBuf[i+3];
			memcpy(pSqlData->m_sData,&pDataBuf[i+8],uDataLen);
			pDataBuf	=	m_pMsg->getRvTime();	//����ǰ��charָ�룬����ý���ʱ��
			memcpy(pSqlData->m_sRvTime,pDataBuf,20);	
			
			if ( kFlag==0||pDataBuf[i+5]!=kFlag )
			{
				m_pqSqlData.push(pSqlData);
				//std::cout<<"��������-"<<"���:"<<pSqlData->m_iKeyID<<"�û�ID:"<<pSqlData->m_cUserID<<"����������:"<<pSqlData->m_cSensorType<<"ֵ:"<<pSqlData->m_sData<<" "<<pSqlData->m_sRvTime<<std::endl;
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

	//����д����ֵ��
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