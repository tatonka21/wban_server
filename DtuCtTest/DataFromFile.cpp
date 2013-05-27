#include "DataFromFile.h"


DataFromFile::DataFromFile( data_record *dr,user_info *ui,MyQueue* dataQueue )
{
	m_pFileHandle = fopen("file-wifi", "rb");
	if(m_pFileHandle == NULL)
	{
		std::cout<<"file cannot be opened!"<<std::endl;
		exit(0);
	}
	m_dr		=	dr;
	m_myQueue	=	dataQueue;
	m_ui		=	ui;
}

DataFromFile::~DataFromFile(void)
{
	if(m_pFileHandle)
		fclose(m_pFileHandle);
}

ULONG DataFromFile::ReceiveData( int * terminated )
{


	char		mess[512];
	int			iResFlag;


	while (!(*terminated))	//如果外围程序要求读线程结束，则不再循环
	{
		if (fread(m_dr, sizeof(data_record), 1, m_pFileHandle)==1 && (!feof(m_pFileHandle)))
		{	
			
			switch(m_dr->m_data_type)
			{
			case 0x01:	//DTU注册包、心跳包
				std::cout<<m_dr->m_userid<<" 登陆"<<std::endl;
				//界面刷新DTU信息
				refreshUserTable();
				break;
			case 0x02:
				printf("%s  注销\n",m_dr->m_userid);
				refreshUserTable();
				break;
			case 0x04:	//DTU上一次收到DSC的错误包，目前对于用户来说，收到这个只是意味着参数配置出错
				std::cout<<"无效的数据包"<<std::endl;
				//请调试参数配置
				break;
			case 0x06:	//DTU收到do_disconnect_ppp_link的请求
				std::cout<<m_dr->m_userid<<" 断开PPP连接成功"<<std::endl;
				iResFlag	=	do_disconnect_ppp_link((uint8 *)m_ui->m_userid,mess);
				if (iResFlag)
				{
					ShowMess(mess);
				}
				break;
			case 0x07:	//DTU收到do_stop_send_data的请求
				std::cout<<m_dr->m_userid<<" 停止向DSC发送数据"<<std::endl;
				iResFlag	=	do_stop_send_data((uint8 *)m_ui->m_userid,mess);
				if (iResFlag)
				{
					ShowMess(mess);
				}
				break;
			case 0x08:	//DTU收到do_start_send_data的请求
				std::cout<<m_dr->m_userid<<" 允许向DSC发送数据"<<std::endl;
				iResFlag	=	do_start_send_data((uint8 *)m_ui->m_userid,mess);
				if (iResFlag)
				{
					ShowMess(mess);
				}
				break;
			case 0x09:	//DTU数据包
				//printf("用户ID：%s, 接收时间：%s, 数据长度：%d\n",m_dr->m_userid,m_dr->m_recv_date,m_dr->m_data_len);
				{
					
					//处理数据
					boost::shared_ptr<Message>	pMsg(new Message(*m_dr));
					m_myQueue->pushBack(pMsg);
					break;
				}
			case 0x0a:	//DTU收到do_flush_cache_data的请求
				break;
			case 0x0b:	//DTU返回参数查询结果
				std::cout<<"DTU返回参数查询结果"<<std::endl;
				//调用GetParam读取参数
				//GetParam()里面第一个参数nParamType ，是什么意思？
				break;
			case 0x0d:	//DTU参数配置成功
				std::cout<<"DTU参数配置成功"<<std::endl;
				break;
			case 0x0e:
				{
					boost::shared_ptr<Message>	pMsg1(new Message(*m_dr));
					m_myQueue->pushBack(pMsg1);
					break;
				}
			default :
				std::cout<<"unknown data_type"<<std::endl;
				break;
			}//end switch

		}
		else if ( feof(m_pFileHandle) )
		{
			std::cout<<"file end"<<std::endl;
			//*terminated	=	1;
		}
		else{
			std::cout<<"read file error"<<std::endl;
			exit(0);
		}//end if
		Sleep(80);
	}
	return 0;
}

void DataFromFile::refreshUserTable()
{
	int i,iMaxDTUAmount;
	iMaxDTUAmount	=	get_max_user_amount();
	for( i=0;i<iMaxDTUAmount;++i )
	{
		get_user_at( i,m_ui );
		get_user_info((uint8 *)m_ui->m_userid,m_ui);
	}

}
