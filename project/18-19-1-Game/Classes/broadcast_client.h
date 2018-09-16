#pragma once
#include<iostream>
using std::cout;
using std::endl;

#define BOOST_ASIO_DISABLE_STD_CHRONO
#define CLINET_BUFFER_SIZE 10240
#define CLIENT_CLOCK_TIME boost::chrono::milliseconds(15)
#include<boost/thread.hpp>
#include<boost/asio.hpp>
#include<boost/smart_ptr.hpp>
#include<boost/bind.hpp>
#include<iostream>
#include<string>
#include<boost/function.hpp>
#include <boost/archive/binary_oarchive.hpp>
#include <boost/archive/binary_iarchive.hpp>
#include<vector>
#include<boost/atomic.hpp>
#include <sstream>
#include <boost/random.hpp>
#include <ctime>
#include <boost/ref.hpp>
#include "monster.h"
#include "st_monster.h"
#include "message.h"
#include "cocos2d.h"

using namespace boost::asio;
using boost::system::error_code;
using ip::tcp;
using std::cout; using std::endl;
using boost::system::error_code;


class BroadcastClient {
	typedef boost::shared_ptr<ip::tcp::socket> sockptr;
	typedef boost::shared_array<char> buffers;
	typedef boost::mt19937 rand;
	typedef boost::system::error_code error_code;

private:
	int networkID;
	io_service& m_io;
	sockptr m_sock;
	//rand rng;
	st_Player m_p;
	std::vector<int> collect_items;
	std::vector<boost::shared_ptr<st_Monster>> monsters;
	std::set<int> monidset;

	boost::function<void(st_CommonSetting cs, BroadcastClient* clientptr)> m_on_status_change_func;
	bool is_callback_setted;
	bool is_sock_thread_start;

	void read();

	void sync_read();

	void read_handler(error_code ec, std::size_t bytes_transferred, boost::asio::streambuf* buf);

	void do_read(boost::asio::streambuf* buf, std::size_t bytes_transferred);

	void connect_handler(error_code ec);

	void sync_write();

	void do_write();

	void write_handler(boost::asio::streambuf *buf, error_code ec);




public:
	static BroadcastClient* instance;

	static BroadcastClient* getInstance();
	/*
	** 构造函数
	*/
	BroadcastClient(io_service& io_);
	void start(std::string ipv4);;
	/*
	** 设置客户端的st_Player状态，在下一帧发送给host
	*/
	void set_client_status(st_Player st_Player);

	/*
	** 设置回调
	*/
	void on_client_status_change(boost::function<void(st_CommonSetting cs, BroadcastClient* clientptr)> call_back);

	void setnetworkID(int id);

	void addCollectItem(int itemID);

	//cm: Close Monster
	void addSceneMonster(Monster& m, int id);

};

/*
** client的启动方法
** 参数1：host的ipv4地址，参数2：回调函数
** 例如 start("127.0.0.1", callback_func)
** 回调的示例在下面
** 建议开一个新线程来执行本方法
*/
void start_broadcast_client(std::string ipv4, boost::function<void(st_CommonSetting cs, BroadcastClient* clientptr)> m_on_status_change_func, int networkID);

/*
** 回调的示例
** cs: 广播的st_CommonSetting类; clientptr: 客户端的this指针
** 当每次收到host同步广播就会调用这个方法
** 建议在这个方法中读取st_CommonSetting信息
** 并使用	clientptr->set_client_status()来设置新状态，并在下一帧发给host
*/
void change_listener(st_CommonSetting cs, BroadcastClient *clientptr);

