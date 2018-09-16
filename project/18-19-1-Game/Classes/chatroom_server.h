#pragma once
#include<iostream>
using std::cout;
using std::endl;

#define BOOST_ASIO_DISABLE_STD_CHRONO
#define CC_BUFFER_SIZE 4096
#define CS_MAX_SOCKET_AMOUNT 3
#define CLOCK_TIME boost::posix_time::milliseconds(100)
#include<boost/thread.hpp>
#include<boost/asio.hpp>
#include<boost/smart_ptr.hpp>
#include<boost/ref.hpp>
#include<boost/bind.hpp>
#include<iostream>
#include<boost/asio/steady_timer.hpp>
#include<boost/asio/system_timer.hpp>
#include<boost/asio/high_resolution_timer.hpp>
#include<string>
#include<boost/function.hpp>
#include<vector>
#include<boost/atomic.hpp>
#include <sstream>
#include <boost/function.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include <boost/archive/text_oarchive.hpp>
#include <boost/archive/text_iarchive.hpp>
#include <sstream>
#include "chat_message.h"
#include <list>
#include "cocos2d.h"

using std::list;
using namespace boost::asio;
using boost::system::error_code;
using ip::tcp;

class ChatroomServer
{
	typedef std::vector<boost::shared_ptr<ip::tcp::socket>> sptrvec;
	typedef boost::shared_ptr<ip::tcp::socket> sockptr;
	typedef boost::shared_array<char> buffers;
	typedef list<boost::shared_ptr<ChatMessage>> msglist;
	typedef boost::shared_ptr<ChatMessage> msg_ptr;
	typedef boost::shared_ptr<boost::asio::streambuf> buffer;

private:

	boost::thread* threadpool[4];
	static ChatroomServer* instance;
	sptrvec socks;
	io_service& io;
	boost::atomic<int> current_sock_amount;
	ip::tcp::acceptor ac;
	boost::atomic<int> current_send_time;
	msglist ml;
	boost::asio::streambuf buf;
	boost::function<void(msg_ptr)> on_recieve;
	bool is_on_recieve_setted;

public:
	/*
	** ���캯����
	** ����1��io_servce
	*/
	ChatroomServer(io_service &io_);
	/*
	** ��������tcp����
	** ֮����Ҫio_serce.poll()
	*/
	~ChatroomServer();

	void start_accept();

	void read_thread(int sockid);

	void read_thread();

	void timing_thread_func();

	void timer_handler(boost::system::error_code ec, deadline_timer * timer);


	/*
	** ����һ����Ϣ
	*/
	void post(ChatMessage msg);

	/*
	** ���ûص�����
	*/
	void set_on_recieve(boost::function<void(msg_ptr)> call_back_func);

	void do_read(int sockid);

	void post_networkID(int sockid);

	void read_handler(int sockid, boost::shared_array<char> charbuf, boost::system::error_code ec, std::size_t bytes_transferred);

	static ChatroomServer* get_instance();

	void close_sock(int playerid);

	void close_all_sock();


private:

	void broadcast();
	void write_handler(boost::system::error_code ec, buffer nbuf);

	void accept_handler(int sockid, boost::system::error_code ec);
};
/*
** ����ǻص�������ʾ��
** ÿ��������Ϣ����ʱ�ͻ�����������
** mp����Ϣ��ChatMessage��ָ�룬
*/
void server_message_listener(boost::shared_ptr<ChatMessage> mp);

/*
** �������������
** ����1��ip��ַ������2���ص�����
** ���鿪һ�����߳���ִ�б�����
*/
ChatroomServer* server_start(boost::function<void(boost::shared_ptr<ChatMessage>)> on_recieve);
