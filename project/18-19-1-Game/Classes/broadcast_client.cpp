#include "broadcast_client.h"

boost::mutex mu_c;

void BroadcastClient::read()
{
	boost::asio::streambuf* buf = new boost::asio::streambuf();
	auto preBuf = buf->prepare(CLINET_BUFFER_SIZE);
	boost::asio::streambuf::mutable_buffers_type* bufs = &preBuf;//&(buf->prepare(CLINET_BUFFER_SIZE));
	m_sock->async_read_some(*bufs, boost::bind(&BroadcastClient::read_handler, this, _1, _2, buf));
}

void BroadcastClient::sync_read() 
{
	boost::asio::streambuf* buf = new boost::asio::streambuf();
	while (true)
	{
		try {
			size_t bt = m_sock->read_some(buf->prepare(CLINET_BUFFER_SIZE));
			buf->commit(bt);
			boost::archive::binary_iarchive ia(*buf);
			broadcast_message msg;
			ia >> msg;
			if (is_callback_setted)
			{
				m_on_status_change_func(msg.cs, this);
			}
			else
			{
				cout << "Callback func hasn't been setted, use BroadcastClient::on_client_status_change() to set." << endl;
			}
			buf->consume(bt);
		}
		catch (exception e)
		{
			continue;
		}
	}
}
BroadcastClient* BroadcastClient::instance = nullptr;

 void BroadcastClient::read_handler(error_code ec, std::size_t bytes_transferred, boost::asio::streambuf * buf)
{
	if (ec) return;
	do_read(buf, bytes_transferred);
	do_write();
}

 void BroadcastClient::do_read(boost::asio::streambuf * buf, std::size_t bytes_transferred) 
 {
	buf->commit(bytes_transferred);
	{
		boost::archive::binary_iarchive ia(*buf);
		broadcast_message msg;
		ia >> msg;
		if (is_callback_setted)
		{
			m_on_status_change_func(msg.cs, this);
		}
		else
		{
			cout << "Callback func hasn't been setted, use BroadcastClient::on_client_status_change() to set." << endl;
		}

	}

	delete buf;

	read();
}

 void BroadcastClient::connect_handler(error_code ec)
{
	if (ec) return;
	auto t = new boost::thread(boost::bind(&BroadcastClient::sync_read, this));
	auto t2 = new boost::thread(boost::bind(&BroadcastClient::sync_write, this));
}

 void setMsgMon(client_message msg, std::vector<boost::shared_ptr<st_Monster>> monsters)
 {
	 for (auto mon : monsters)
	 {
		 msg.monsters.push_back(*mon);
	 }
 }

 void BroadcastClient::sync_write()
 {
	 boost::asio::streambuf *buf = new boost::asio::streambuf();
	 while (true)
	 {
		 boost::archive::binary_oarchive oa(*buf);
		 client_message msg(m_p);
		 msg.collect_items = collect_items;
		 setMsgMon(msg, monsters);
		 msg.networkID = networkID;
		 oa << msg;
		 collect_items.clear();
		 monsters.clear();
		 monidset.clear();
		 boost::system::error_code ec;
		 m_sock->write_some(buf->data(),ec);
		 if (ec)
			 log("serialize failed");
		 buf->consume(buf->size());
		 boost::this_thread::sleep_for(CLIENT_CLOCK_TIME);
	 }
	 
 }



 void BroadcastClient::do_write()
{
	boost::asio::streambuf *buf = new boost::asio::streambuf();
	{
		boost::archive::binary_oarchive oa(*buf);
		client_message msg(m_p);
		oa << msg;
	}
	m_sock->async_write_some(buf->data(), boost::bind(&BroadcastClient::write_handler, this, buf, _1));
}

 void BroadcastClient::write_handler(boost::asio::streambuf * buf, error_code ec)
{
	delete buf;
	if (ec) return;
}

 void BroadcastClient::addSceneMonster(Monster& m,int id)
 {
	 boost::shared_ptr<st_Monster> sm(new st_Monster);
	 sm->monsterID = id;
	 sm->m_fDamage = m.getDamage();
	 sm->m_fDegree = m.getDegree();
	 sm->m_fHP = m.getHP();
	 sm->m_fSpeed = m.getSpeed();
	 sm->m_strNextDirection = m.getNextDirection();
	 sm->m_strNowDirection = m.getNowDirection();
	 sm->x = m.getMonster()->getPosition().x;
	 sm->y = m.getMonster()->getPosition().y;


	 auto it = monidset.find(id);
	 if (it == monidset.end())
	 {
		 monsters.push_back(sm);
		 monidset.insert(sm->monsterID);
	 }
	 else
	 {
		 for (auto mit = monsters.begin(); mit != monsters.end(); mit++)
		 {
			 if ((*mit)->monsterID == sm->monsterID)
			 {
				 if ((*mit)->m_fHP < sm->m_fHP)
				 {
					 monsters.erase(mit);
					 monsters.push_back(sm);
				 }
			 }
			 break;
		 }
	 }
 }

 void BroadcastClient::addCollectItem(int itemID)
 {
	 for (int i : collect_items)
	 {
		 if (i == itemID)
			 return;
	 }
	 collect_items.push_back(itemID);
 }

 BroadcastClient * BroadcastClient::getInstance()
 {
	 return instance;
 }

/*
** 构造函数
*/

 BroadcastClient::BroadcastClient(io_service & io_) :m_io(io_), 
	 m_sock(new ip::tcp::socket(m_io)),/* rng(time(0)),*/ is_callback_setted(false), is_sock_thread_start(false)
{
	 BroadcastClient::instance = this;
}

 void BroadcastClient::start(std::string ipv4)
{
	ip::tcp::endpoint ep(ip::address::from_string(ipv4), 16291);
	m_sock->async_connect(ep, boost::bind(&BroadcastClient::connect_handler, this, _1));
	m_io.run();
}

/*
** 设置客户端的st_Player状态，在下一帧发送给host
*/

 void BroadcastClient::set_client_status(st_Player st_Player)
{
		m_p = st_Player;
 }

/*
** 设置回调
*/

 void BroadcastClient::on_client_status_change(boost::function<void(st_CommonSetting cs, BroadcastClient*clientptr)> call_back)
{
	m_on_status_change_func = call_back;
	if (is_callback_setted == false)
		is_callback_setted = true;
}

void BroadcastClient::setnetworkID(int id)
{
	this->networkID = id;
}

/*
** client的启动方法
** 参数1：host的ipv4地址，参数2：回调函数
** 例如 start("127.0.0.1", callback_func)
** 回调的示例在下面
** 建议开一个新线程来执行本方法
*/
 void start_broadcast_client(std::string ipv4, boost::function<void(st_CommonSetting cs, BroadcastClient* clientptr)> m_on_status_change_func, int networkID)
 {
	auto io = new io_service();
	auto client = new BroadcastClient(*io);
	client->on_client_status_change(m_on_status_change_func);
	client->setnetworkID(networkID);
	client->start(ipv4);
	io->run();
}

/*
** 回调的示例
** cs: 广播的st_CommonSetting类; clientptr: 客户端的this指针
** 当每次收到host同步广播就会调用这个方法
** 建议在这个方法中读取st_CommonSetting信息
** 并使用	clientptr->set_client_status()来设置新状态，并在下一帧发给host
*/
void change_listener(st_CommonSetting cs, BroadcastClient * clientptr)
{

}
