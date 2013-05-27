#pragma once
#include "clientSession.h"
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <iostream>
#include "MyQueue.h"
using boost::asio::ip::tcp;


class serverApp
{
	typedef boost::shared_ptr<clientSession> session_ptr;
public:
	serverApp(boost::asio::io_service& ioservice,tcp::endpoint & endpoint,MyQueue *queue);
	~serverApp(void);

private:
	void handle_accept(const boost::system::error_code& error,session_ptr& session);

	boost::asio::io_service& m_ioservice;
	tcp::acceptor acceptor_;
	MyQueue			*m_myQueue;
};
