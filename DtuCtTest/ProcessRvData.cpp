#include "ProcessRvData.h"


ProcessRvData::ProcessRvData(void)
{
}

ProcessRvData::ProcessRvData( data_record *dr,user_info *ui,MyQueue *dataQueue )
{
	m_pFileHandle = fopen("file", "wb");
	if(m_pFileHandle == NULL)
	{
		std::cout<<"file cannot be created!"<<std::endl;
		exit(0);
	}
	m_dr		=	dr;
	m_myQueue	=	dataQueue;
	m_ui		=	ui;
}
ProcessRvData::~ProcessRvData(void)
{
	if(m_pFileHandle)
		fclose(m_pFileHandle);
}

ULONG ProcessRvData::ReceiveData( int * terminated )
{

	char		mess[512];
	int			iResFlag;


	while (!(*terminated))	//�����Χ����Ҫ����߳̽���������ѭ��
	{
		if (do_read_proc(m_dr,NULL,false)>=0)
		{	
			fwrite(m_dr, sizeof(data_record), 1, m_pFileHandle);
			
			switch(m_dr->m_data_type)
			{
				case 0x01:	//DTUע�����������
					std::cout<<m_dr->m_userid<<" ��½"<<std::endl;
					//����ˢ��DTU��Ϣ
					refreshUserTable();
					break;
				case 0x02:
					printf("%s  ע��\n",m_dr->m_userid);
					refreshUserTable();
					break;
				case 0x04:	//DTU��һ���յ�DSC�Ĵ������Ŀǰ�����û���˵���յ����ֻ����ζ�Ų������ó���
					std::cout<<"��Ч�����ݰ�"<<std::endl;
							//����Բ�������
					break;
				case 0x06:	//DTU�յ�do_disconnect_ppp_link������
					std::cout<<m_dr->m_userid<<" �Ͽ�PPP���ӳɹ�"<<std::endl;
					iResFlag	=	do_disconnect_ppp_link((uint8 *)m_ui->m_userid,mess);
					if (iResFlag)
					{
						ShowMess(mess);
					}
					break;
				case 0x07:	//DTU�յ�do_stop_send_data������
					std::cout<<m_dr->m_userid<<" ֹͣ��DSC��������"<<std::endl;
					iResFlag	=	do_stop_send_data((uint8 *)m_ui->m_userid,mess);
					if (iResFlag)
					{
						ShowMess(mess);
					}
					break;
				case 0x08:	//DTU�յ�do_start_send_data������
					std::cout<<m_dr->m_userid<<" ������DSC��������"<<std::endl;
					iResFlag	=	do_start_send_data((uint8 *)m_ui->m_userid,mess);
					if (iResFlag)
					{
						ShowMess(mess);
					}
					break;
				case 0x09:	//DTU���ݰ�
					printf("�û�ID��%s, ����ʱ�䣺%s, ���ݳ��ȣ�%d\n",m_dr->m_userid,m_dr->m_recv_date,m_dr->m_data_len);
					{
					
					  //��������
					  boost::shared_ptr<Message>	pMsg(new Message(*m_dr));
					  m_myQueue->pushBack(pMsg);
					  break;
					}
				case 0x0a:	//DTU�յ�do_flush_cache_data������
					break;
				case 0x0b:	//DTU���ز�����ѯ���
					std::cout<<"DTU���ز�����ѯ���"<<std::endl;
					//����GetParam��ȡ����
					//GetParam()�����һ������nParamType ����ʲô��˼��
					break;
				case 0x0d:	//DTU�������óɹ�
					std::cout<<"DTU�������óɹ�"<<std::endl;
					break;
				default :
					std::cout<<"unknown data_type"<<std::endl;
					break;
			}//end switch
			
		}//end if
		Sleep(80);
	}
	//ExitThread(100);  // ??
	return 0;


}

void ProcessRvData::refreshUserTable()
{
	int i,iMaxDTUAmount;
	iMaxDTUAmount	=	get_max_user_amount();
	for( i=0;i<iMaxDTUAmount;++i )
	{
		get_user_at( i,m_ui );
		get_user_info((uint8 *)m_ui->m_userid,m_ui);
	}
}