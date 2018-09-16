#include "broadcast_server.h"
#include "hall.h"
mutex mu;

BroadcastServer* BroadcastServer::getInstance() {
	return instance;
}

BroadcastServer* BroadcastServer::instance = nullptr;


 void BroadcastServer::accept_handler(sockptr sp, error_code ec)
{
	if (ec)
		return;
	++m_current_sock_amount;
	cout << "a tcp connect from " << sp->remote_endpoint().address() << " established" << endl;
	//read(m_current_sock_amount.load() - 1);
	auto t = new boost::thread(boost::bind(&BroadcastServer::sync_read, this, m_current_sock_amount-1));
	if (m_current_sock_amount !=  Hall::getInstance()->getRoomPlayerNumber())
		start_accept();
}

 void BroadcastServer::sync_read(int sockid)
 {
	 while (true)
	 {
		 try {
			 buffer buf(new boost::asio::streambuf);
			 size_t bytes_transferred = m_socks[sockid]->read_some(buf->prepare(BUFFER_SIZE));
			 buf->commit(bytes_transferred);
			 boost::archive::binary_iarchive ia(*buf);
			 client_message msg;
			 ia >> msg;
			 /*		 if (msg.monsters.size() != 0)
					  {
						  stringstream ss;
						  ss << "read " << msg.monsters.size() << " monsters from client";
						  cocos2d::log(ss.str().data());
					  }*/
					  //st_Players[sockid + 1] = msg.Player;
			 st_Players[msg.networkID - 1] = msg.Player;
			 addCollectItems(msg.collect_items);
			 addClientMonster(msg.monsters);
		 }
		 catch (std::exception e)
		 {
			 continue;
		 }
	 }
 }

 void BroadcastServer::write_handler(error_code ec)
{
	++m_current_send_time;
	if (m_current_send_time == m_current_sock_amount)
	{
		m_current_send_time.store(0);
		//delete buf;
	}
}

 void BroadcastServer::do_broadcast(boost::asio::deadline_timer * t)
{
	 boost::thread* tb = new boost::thread(
		 [this]()
	 {
		 while (true)
		 {
			 if(m_current_sock_amount!=0)
				do_write();
			 boost::this_thread::sleep_for(CHRONO_TIME);
		 }

	 }
	 );
}

 void BroadcastServer::do_write()
{
	 merge_player_status();
	 {
		 //boost::archive::binary_oarchive oa(*buf);
		 broadcast_message msg(m_cs);
		 msg.cs.st_Player[1] = st_Players[0];
		 for (int i : collect_items)
		 {
			 msg.cs.collect_items.push_back(i);
		 }
		 collect_items.clear();

		 //cocos2d::log("BroadcastServer::do_write %d monsters in list", monsters.size());
		 for (auto mon : monsters)
		 {
			 msg.cs.monsters.push_back(mon);
		 }
		 monsters.clear();
		 monidset.clear();
		 mu.lock();
		 m_message_list.push_back(msg);
		 mu.unlock();
         cocos2d::log("msg player 2 level %f", msg.cs.st_Player[2].Level);

		 if (is_callback_setted)
		 {
			 m_on_status_change_func(msg.cs, this);
		 }
		 else
		 {
			 cout << "Callback func hasn't been setted, use BroadcastServer::on_server_status_change() to set." << endl;
		 }

		 //oa << msg;
	 }
	 for (int i = 0; i < m_current_sock_amount; ++i)
	{
		//buffer buf(new boost::asio::streambuf());

		//m_socks[i]->async_write_some(buf->data(), boost::bind(&BroadcastServer::write_handler, this, _1));
		//�첽���ͣ��������ӳ�
		if (!is_send_thread_start[i])
		{
			auto t = new thread(boost::bind(&BroadcastServer::sync_write_thread, this, m_socks[i]));
			is_send_thread_start[i] = true;
		}
	}
	
}

 void BroadcastServer::sync_write_thread(sockptr sp)
 {

	 while (true)
	 {
		 if (m_message_list.size() != 0)
		 {
				 buffer buf(new boost::asio::streambuf());
				 {
					 boost::archive::binary_oarchive oa(*buf);
					 mu.lock();
					 broadcast_message msg = m_message_list.front();
					 mu.unlock();
					 oa << msg;
					 m_current_send_time++;
				 }
				 sp->write_some(buf->data());
		
		 }
		 else
			 boost::this_thread::sleep_for(boost::chrono::milliseconds(2));
		 if (m_current_send_time == m_current_sock_amount)
		 {
			 mu.lock();
			 m_message_list.pop_front();
			 mu.unlock();
			 m_current_send_time = 0;
		 }
	 }
	 
	
			
	}

 void BroadcastServer::addCollectItems(vector<int> items)
 {
	 for (int i : items)
	 {
		 bool has_item = false;
		 for (int j : collect_items)
		 {
			 if (i == j)
			 {
				 has_item = true;
			 }
		 }
		 if (!has_item)
		 {
			 collect_items.push_back(i);
		 }
	 }
 }

 void BroadcastServer::read_handler(int sockid, error_code ec, std::size_t bytes_transferred, buffer buf)
{
	if (ec) return;
	buf->commit(bytes_transferred);
	boost::archive::binary_iarchive ia(*buf);
	client_message msg;
	ia >> msg;
	st_Players[sockid + 1] = msg.Player;
	
	buf->consume(bytes_transferred);

	read(sockid);
}

 void BroadcastServer::read(int i)
{
	buffer buf(new boost::asio::streambuf());
	m_socks[i]->async_read_some(buf->prepare(BUFFER_SIZE), boost::bind(&BroadcastServer::read_handler, this, i, _1, _2, buf));
}

 void BroadcastServer::merge_player_status()
{
	for (int i = 0; i < 4; i++)
	{
		m_cs.st_Player[i + 1] = st_Players[i];
	}
}


 

 BroadcastServer::BroadcastServer(io_service & io_) : m_io(io_), m_socks(MAX_SOCKET_AMOUNT), 
	 m_current_sock_amount(0), m_ac(m_io, tcp::endpoint(tcp::v4(), 16291)),
	 m_current_send_time(0), m_message_list(0)
{
	 BroadcastServer::instance = this;
	 for (int i = 0; i < 3; i++)
	 {
		 is_send_thread_start[i] = false;
	 }
 }

/*
** Main method, to start service. Use io_service::run when service start
*/

 void BroadcastServer::start_accept()
{
	sockptr s(new ip::tcp::socket(m_io));
	m_socks[m_current_sock_amount] = s;
	m_ac.async_accept(*m_socks[m_current_sock_amount], boost::bind(&BroadcastServer::accept_handler, this, m_socks[m_current_sock_amount], _1));
}

/*
** start broadcast
*/

 void BroadcastServer::start_broadcast(boost::asio::deadline_timer * t) {
	t->async_wait(boost::bind(&BroadcastServer::do_broadcast, this, t));
	m_io.run();
}

/*
** ����host server��st_CommonSetting״̬
*/

 void BroadcastServer::set_server_status(st_CommonSetting cs_)
{
		m_cs = cs_;
}

 void BroadcastServer::on_server_status_change(boost::function<void(st_CommonSetting cs, BroadcastServer*clientptr)> call_back)
{
	m_on_status_change_func = call_back;
	if (is_callback_setted == false)
		is_callback_setted = true;
}

 void BroadcastServer::set_host_player_status(st_Player p)
 {
	 st_Players[0] = p;//0����ҹ̶�Ϊhost
 }

 void BroadcastServer::addHostCollectItem(int id)
 {
	 for (int i : collect_items)
	 {
		 if (i == id)
			 return;
	 }
	 collect_items.push_back(id);
 }

 void BroadcastServer::addSceneMonster(Monster & m, int id)
 {
	 st_Monster sm;
	 sm.monsterID = id;
	 sm.m_fDamage = m.getDamage();
	 sm.m_fDegree = m.getDegree();
	 sm.m_fHP = m.getHP();
	 sm.m_fSpeed = m.getSpeed();
	 sm.m_strNextDirection = m.getNextDirection();
	 sm.m_strNowDirection = m.getNowDirection();
	 sm.x = m.getMonster()->getPosition().x;
	 sm.y = m.getMonster()->getPosition().y;

	 auto it = monidset.find(id);
	 if (it == monidset.end())
	 {
		 //cocos2d::log("BroadcastServer::addSceneMonster add 1 monster to list");
		 monsters.push_back(sm);
		 monidset.insert(sm.monsterID);
	 }
	 else
	 {
		 for (auto mit = monsters.begin(); mit != monsters.end(); mit++)
		 {
			 if ((*mit).monsterID == sm.monsterID)
			 {
				 if ((*mit).m_fHP < sm.m_fHP)
				 {
					 monsters.erase(mit);
					 monsters.push_back(sm);
				 }
				 else
				 {
					 //cocos2d::log("BroadcastServer::addSceneMonster give up 1 monster");
				 }
			 }
			 
			 break;
		 }
	 }
 }


 void BroadcastServer::addClientMonster(std::vector<st_Monster>& mons)
 {
	 int i = 0;
	 for (auto mon : mons)
	 {
		 st_Monster sm;
		 auto it = monidset.find(mon.monsterID);
		 if (it == monidset.end())
		 {
			 monsters.push_back(sm);
			 monidset.insert(mon.monsterID);
		 }
		 else
		 {
			 for (auto mit = monsters.begin(); mit != monsters.end(); mit++)
			 {
				 if ((*mit).monsterID == sm.monsterID)
				 {
					 if ((*mit).m_fHP < sm.m_fHP)
					 {
						 monsters.erase(mit);
						 monsters.push_back(sm);
					 }
				 }
				 break;
			 }
		 }

	 }
	 //cocos2d::log("BroadcastServer::addClientMonster finish, now mons: %d, now monidset: %d", monsters.size(), monidset.size());
 }

/*
** host server�������
** ����1���ص�����
** ���� start( callback_func)
** �ص���ʾ��������
** ���鿪һ�����߳���ִ�б�����
*/
/*
** host server�������
** ����1���ص�����
** ���� start( callback_func)
** �ص���ʾ��������
** ���鿪һ�����߳���ִ�б�����
*/
void start_broadcast_server(boost::function<void(st_CommonSetting cs, BroadcastServer*clientptr)> call_back_func)
{
	io_service io;
	BroadcastServer server(io);
	server.on_server_status_change(call_back_func);
	server.start_accept();

	boost::asio::deadline_timer t(io, boost::posix_time::milliseconds(15));
	server.start_broadcast(&t);

	io.run();


}

/*
** �ص���ʾ��
** cs: �㲥��st_CommonSetting��; serverptr: host��thisָ��
** ��ÿ���յ�hostͬ���㲥�ͻ�����������
** ��������������ж�ȡst_CommonSetting��Ϣ
** ��ʹ��	serverptr->set_server_status()��������ȫ��״̬��������һ֡�㲥
*/
void change_listener(st_CommonSetting cs, BroadcastServer * serverptr)
{

}
