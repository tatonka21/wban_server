#pragma once
#include <boost/asio.hpp>
#include <boost/bind.hpp>
#include <boost/shared_ptr.hpp>
#include <boost/enable_shared_from_this.hpp>
#include <iostream>
#include <string>
#include "head_info.h"
#include "MyQueue.h"
#define max_len 1024
using boost::asio::ip::tcp;


class clientSession
	:public boost::enable_shared_from_this<clientSession>
{
public:
	clientSession(boost::asio::io_service& ioservice,MyQueue *queue);
	~clientSession(void);

	tcp::socket& socket();
	void start();

private:
	void handle_write(const boost::system::error_code& error);
	void handle_read(const boost::system::error_code& error, std::size_t bytesTransferred);

private:
	tcp::socket m_socket;
	char data_[max_len];
	FILE *	m_pFileHandle;
	MyQueue			*m_myQueue;
};
