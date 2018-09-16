#pragma once
#include "chatroom_server.h"

/*
** 构造函数。
** 参数1：io_servce
*/

ChatroomServer::ChatroomServer(io_service & io_) : io(io_), ac(io, tcp::endpoint(tcp::v4(), 16667)), socks(CS_MAX_SOCKET_AMOUNT), is_on_recieve_setted(false)
{
	current_sock_amount.store(0);
	current_send_time.store(0);
	ChatroomServer::instance = this;
}

/*
** 开启接收tcp连接
** 之后需要io_serce.poll()
*/

ChatroomServer::~ChatroomServer()
{
	ChatroomServer::get_instance()->close_all_sock();
	for (int i = 0; i < 4; i++)
	{
		threadpool[i]->interrupt();
        delete threadpool[i];
	}
	io.stop();
	delete &io;
	instance = nullptr;
}

void ChatroomServer::start_accept()
{
	//sockptr s(new ip::tcp::socket(io));
	//socks[current_sock_amount] = s;
	//cout << current_sock_amount.load() << endl;
	//ac.async_accept(*socks[current_sock_amount], boost::bind(&ChatroomServer::accept_handler, this, this->current_sock_amount.load(), _1));
	//boost::thread timingThread(boost::bind(&ChatroomServer::timing_thread_func, this));
	//++current_sock_amount;
	//io.poll();
	for (int i = 0; i < 3; i++)
	{
		threadpool[i] = new boost::thread(boost::bind(&ChatroomServer::read_thread, this, i));
	}
	threadpool[3] = new boost::thread(boost::bind(&ChatroomServer::broadcast, this));

}

void ChatroomServer::read_thread(int sockid)
{
	try {
		sockptr s(new ip::tcp::socket(io));
		socks[sockid] = s;
		ac.accept(*socks[sockid]);
	}
	catch (std::exception e)
	{
		return;
	}
	current_sock_amount++;

	ChatMessage msg;
	std::stringstream ss;
	ss << current_sock_amount;
	msg.message = ss.str();
	msg.playerID = -1;
	buffer nbuf(new streambuf());
	boost::archive::binary_oarchive oa(*nbuf);
	oa << msg;
	socks[sockid]->write_some(nbuf->data());
	buffer nbuf2(new streambuf());

	while (true)
	{
		boost::system::error_code ec;
		auto bt = socks[sockid]->read_some(nbuf2->prepare(CC_BUFFER_SIZE), ec);
		nbuf2->commit(bt);
		boost::archive::binary_iarchive ia(*nbuf2);
		boost::shared_ptr<ChatMessage> msg(new ChatMessage);
		ia >> *msg;
		nbuf2->consume(bt);
		ml.push_back(msg);
	}
}

void ChatroomServer::timing_thread_func()
{
	auto timer = new deadline_timer(io, CLOCK_TIME);	 timer->async_wait(boost::bind(&ChatroomServer::timer_handler, this, _1, timer));
	io.run();
}

void ChatroomServer::timer_handler(boost::system::error_code ec, deadline_timer* timer)
{
	// cout << "start one timer" << endl;
	timer->expires_at(timer->expires_at() + CLOCK_TIME);
	timer->async_wait(boost::bind(&ChatroomServer::timer_handler, this, _1, timer));
	//io.run();
}

void ChatroomServer::close_sock(int playerid)
{
	socks[playerid - 1]->close();
	ac.async_accept(*socks[playerid - 1], boost::bind(&ChatroomServer::accept_handler, this, playerid - 1, _1));

}

void ChatroomServer::close_all_sock()
{
	for (int i = 0; i < CS_MAX_SOCKET_AMOUNT; i++)
	{
		socks[i]->close();
	}
	
}

void ChatroomServer::post(ChatMessage msg)
{
	msg_ptr mp(new ChatMessage(msg));
	ml.push_back(mp);
}

/*
** 设置回调函数
*/

void ChatroomServer::set_on_recieve(boost::function<void(msg_ptr)> call_back_func)
{
	if (!is_on_recieve_setted)
		is_on_recieve_setted = true;
	on_recieve = call_back_func;
}

void ChatroomServer::broadcast()
{
	buffer nbuf(new streambuf());
	while (true)
	{
		if (ml.size() != 0)
		{
			msg_ptr msgp = ml.front();
			ChatMessage msg = *msgp;
			ml.pop_front();
			if (is_on_recieve_setted)
				on_recieve(msgp);
			else
				cout << "on_recieve_func hasn't been setted, call ChatroomServer::set_on_recieve() to set" << endl;

			for (int i = 0; i < current_sock_amount; ++i)
			{
				{
					boost::system::error_code ec;
					boost::archive::binary_oarchive oa(*nbuf);
					oa << msg;
					socks[i]->write_some(nbuf->data(), ec);
					if (ec)
					{
						continue;
					}
				}
			}
			nbuf->consume(nbuf->size());
			cout << "broadcast one msg, left : " << ml.size() << " msg(s). " << endl;
		}
		else
		{
			boost::this_thread::sleep_for(boost::chrono::milliseconds(500));
		}
	}
}

void ChatroomServer::write_handler(boost::system::error_code ec, buffer nbuf)
{
	//delete nbuf;
	if (ec)
		return;
}

void ChatroomServer::accept_handler(int sockid, error_code ec)
{

	cocos2d::log(" ChatroomServer::accept_handler accept connect");
	//cout << "a new player from " << socks[sockid]->remote_endpoint().address() << "  join chatroom" << endl;
	if (ec)
		return;

	post_networkID(sockid);

	do_read(sockid);
	if (current_sock_amount != 3)
		start_accept();
}


void ChatroomServer::do_read(int sockid)
{
	boost::shared_array<char> charbuf(new char[CC_BUFFER_SIZE]);
	socks[sockid]->async_read_some(boost::asio::buffer(charbuf.get(), CC_BUFFER_SIZE), boost::bind(&ChatroomServer::read_handler, this, sockid, charbuf, _1, _2));
	io.poll();
}

//用来分配NetworkId
void ChatroomServer::post_networkID(int sockid)
{
	//ChatMessage msg;
	//std::stringstream ss;
	//ss << sockid+1;
	//msg.message = ss.str();
	//msg.playerID = -1;
	//
	//buffer nbuf(new streambuf());
	//boost::archive::binary_oarchive oa(*nbuf);
	//oa << msg;
	//socks[sockid]->async_write_some(nbuf->data(), boost::bind(&ChatroomServer::write_handler, this, _1, nbuf));
}


void ChatroomServer::read_handler(int sockid, boost::shared_array<char> charbuf, boost::system::error_code ec, std::size_t bytes_transferred)
{
	if (ec)
		return;

	try {
		std::stringstream ss;
		ss << charbuf.get();
		boost::archive::text_iarchive ia(ss); // an exception here
		msg_ptr mp(new ChatMessage);
		ia >> *mp;
		//delete nbuf;
		ml.push_back(mp);
		broadcast();
		do_read(sockid);
	}
	catch (boost::archive::archive_exception e)
	{
		cout << "read message failed" << endl;
		do_read(sockid);
	}


}
ChatroomServer * ChatroomServer::instance = nullptr;

ChatroomServer * ChatroomServer::get_instance()
{
	return ChatroomServer::instance;
}

/*
** 这个是回调函数的示例
** 每次有新消息到达时就会调用这个函数
** mp：消息类ChatMessage的指针，
*/
void server_message_listener(boost::shared_ptr<ChatMessage> mp)
{
	cout << "[player]\t\t[message]" << endl;
	cout << mp->playerID << "\t\t" << mp->message << endl;
}

/*
** 这个是启动方法
** 参数1：ip地址，参数2：回调函数
** 建议开一个新线程来执行本方法
*/
ChatroomServer* server_start(boost::function<void(boost::shared_ptr<ChatMessage>)> on_recieve)
{
	boost::asio::io_service* io = new boost::asio::io_service;
	ChatroomServer* server = new ChatroomServer(*io);
	server->set_on_recieve(on_recieve);
	server->start_accept();
	io->poll();
	return server;
}

