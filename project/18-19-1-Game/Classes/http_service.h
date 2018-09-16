#pragma once

#include "network/HttpClient.h"
#include "cocos2d.h"
#include <sstream>
#include "json/rapidjson.h"
#include "json/document.h"
#include "boost/function.hpp"
#include "st_Room.h"
#include "boost/smart_ptr.hpp"
#include <vector>
#include "boost/bind.hpp"

using namespace cocos2d::network;

typedef std::vector<boost::shared_ptr<st_room>> room_list_type;
typedef boost::shared_ptr<st_room> room_ptr_type;

class HttpService
{
private:
	room_list_type *m_cached_list;
	boost::function<void(room_list_type *)> m_getrooms_callback;
	bool is_gc_setted;
	boost::function<void(bool)> m_join_room_callback;
	bool is_jr_setted;
	boost::function<void(room_ptr_type)> m_getoneroom_callback;
	bool is_goc_setted;
	static HttpService* instance;
public:
	static HttpService* getInstance();
	//加入房间,需要调用set_joinroom_cb设置回调
	void joinRoom(std::string ipv4, std::string playerName);
	//创建房间， 弃用
	void openRoom(std::string ipv4, std::string playername);
	//真正的创建房间，需要调用set_joinroom_cb设置回调
	void createRoom(std::string playername);				//9-9
	//Client离开房间, 无回调
	void leaveRoom(std::string ipv4);

	void get_my_room(boost::function<void(room_ptr_type)> callback);

	//Host关闭房间， 无回调
	void closeRoom();
	//获取房间列表，需要调用set_getrooms_cb设置回调
	void getRoomsViaHttp();
	//获取单个房间信息,需要调用set_getoneroom_cb设置回调
	void getOneRoom(std::string ipv4);						//9-9
	//设置获得单个房间回调
	void set_getoneroom_cb(boost::function<void(room_ptr_type)> callback);
	//获取房间列表回调
	void set_getrooms_cb(boost::function<void(room_list_type *)> callback);
	//创建房间回调
	void set_joinroom_cb(boost::function<void(bool)> callback);
private:

	HttpService();
	room_list_type * parseRoomListJson(const char * json);
	const room_ptr_type parseRoomJson(const char * json);
	void getroom_callback(HttpClient *sender, HttpResponse *response);
	void joinroom_callback(HttpClient *sender, HttpResponse *response);
	void leaveroom_callback(HttpClient *sender, HttpResponse *response);

};
