#include "clientSession.h"

clientSession::clientSession( boost::asio::io_service& ioservice,MyQueue *queue )
:m_socket(ioservice)
{
	memset(data_,'\0',sizeof(data_));
	m_myQueue	=	queue;
	m_pFileHandle = fopen("file-wifi", "wb");
	if(m_pFileHandle == NULL)
	{
		std::cout<<"file cannot be created!"<<std::endl;
		exit(0);
	}
}

clientSession::~clientSession(void)
{
	if(m_pFileHandle)
		fclose(m_pFileHandle);
}

tcp::socket& clientSession::socket()
{
	return m_socket;
}

void clientSession::start()
{
	boost::asio::async_write(m_socket,
		boost::asio::buffer("Link successed!"),
		boost::bind(&clientSession::handle_write,shared_from_this(),
		boost::asio::placeholders::error));

	m_socket.async_read_some(boost::asio::buffer(data_,max_len),
		boost::bind(&clientSession::handle_read,shared_from_this(),
		boost::asio::placeholders::error,
		boost::asio::placeholders::bytes_transferred));
	tcp::socket & t =m_socket;
	//std::cout<<"IP:"<<t.local_endpoint().address()<<" port:"<<t.local_endpoint().port()<<std::endl;

}

void clientSession::handle_write( const boost::system::error_code& error )
{
	if(error)
	{
		std::cout<<error.message()<<std::endl;
		m_socket.close();
	}
}

void clientSession::handle_read( const boost::system::error_code& error, std::size_t bytesTransferred )
{
	if(!error)
	{
		char	time_now[20]	=	{0};
		getNowTime( time_now );

		data_record	*	dataRecTmp	=	new	data_record();
		memcpy(dataRecTmp->m_data_buf,data_,bytesTransferred);
		memcpy(dataRecTmp->m_recv_date,time_now,20);
		//memcpy(dataRecTmp->m_userid,0,12);
		dataRecTmp->m_data_type	=	0x0e;
		dataRecTmp->m_data_len	=	bytesTransferred;


		std::cout <<"Total len:" <<bytesTransferred <<" Time:"<<dataRecTmp->m_recv_date<<" Data:"<<data_<< std::endl;

		//写到文件
		fwrite(dataRecTmp,sizeof(data_record),1,m_pFileHandle);
		fflush(m_pFileHandle);

		//存到队列
		boost::shared_ptr<Message>	pMsg(new Message(*dataRecTmp));
		m_myQueue->pushBack(pMsg);

		m_socket.async_read_some(boost::asio::buffer(data_,max_len),
			boost::bind(&clientSession::handle_read,shared_from_this(),
			boost::asio::placeholders::error,
			boost::asio::placeholders::bytes_transferred));
	}
	else
	{
		std::cout<<error.message()<<std::endl;
		m_socket.close();
	}
}