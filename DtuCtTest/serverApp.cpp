#include "serverApp.h"


serverApp::serverApp( boost::asio::io_service& ioservice,tcp::endpoint & endpoint,MyQueue *queue )
:m_ioservice(ioservice),
acceptor_(ioservice,endpoint)
{
	m_myQueue	=	queue;
	session_ptr new_session(new clientSession(ioservice,m_myQueue));
	acceptor_.async_accept(new_session->socket(),
		boost::bind(&serverApp::handle_accept,this,boost::asio::placeholders::error,
		new_session));
}

serverApp::~serverApp(void)
{
}

void serverApp::handle_accept( const boost::system::error_code& error,session_ptr& session )
{
	if(!error)
	{
		std::cout << "get a new client! "  << std::endl;
		//ʵ�ֶ�ÿ���ͻ��˵����ݴ���

		session->start();//ÿһ��session����һ���ͻ���

		session_ptr new_session(new clientSession(m_ioservice,m_myQueue));
		acceptor_.async_accept(new_session->socket(),
			boost::bind(&serverApp::handle_accept,this,boost::asio::placeholders::error,
			new_session));
	}
}