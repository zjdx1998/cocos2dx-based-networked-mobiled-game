//RoomModule.h
/*
Description:房间模块头文件
Author:Hanyuu
Date:18/09/03
*/

#ifndef ROOM_MODULE_H
#define ROOM_MODULE_H

#include <string>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "http_service.h"
#include "hallTools.h"
#include "hallPlayer.h"
//#include "hall.h"
//#include "HallScene.h"

class RoomModule
{
	//friend Hall;
public:
	//当前房间内玩家人数
	int m_roomPlayerNumber;
	short m_selected;
	std::vector<st_RoomInfo> m_roomList;
	RoomModule();
	~RoomModule();
	/*
	Function name:updateRoomList
	Description:更新List信息
	Created:18/09/06
	Parameter:VOID:
	Return:VOID
	Author:Hanyuu
	*/
	void updateRoomList();
	/*
	Function name:addInfo
	Description:添加List信息
	Created:18/09/06
	Parameter:st_RoomInfo*:房间信息
	Return:VOID
	Author:Hanyuu
	*/
	void addInfo(st_RoomInfo *);
	/*
	Function name:recall
	Description:回调
	Created:18/09/06
	Parameter:bool:判断是否成功
	Return:VOID
	Author:Hanyuu
	*/
	void recall(bool);
	/*
	Function name:roomModuleInitialize
	Description:房间模块初始化信息
	Created:18/09/06
	Parameter:VOID:
	Return:VOID
	Author:Hanyuu
	*/
	void roomModuleInitialize();
	/*
	Function name:setRoomModuleVisible
	Description:设置房间是否可见
	Created:18/09/06
	Parameter:bool:设置值
	Return:VOID
	Author:Hanyuu
	*/
	void setRoomModuleVisible(bool);
	/*
	Function name:getHostName
	Description:获取Host名字
	Created:18/09/06
	Parameter:VOID
	Return:string:Host名字
	Author:Hanyuu
	*/
	std::string getHostName();
	/*
	Function name:getHostIP
	Description:获取IP地址
	Created:18/09/06
	Parameter:VOID
	Return:string：IP地址
	Author:Hanyuu
	*/
	std::string getHostIP();
    int getPlayerNumber();
	/*
	Function name:getRoomCallback
	Description:获取房间回调
	Created:18/09/06
	Parameter:room_list_type
	Return:VOID
	Author:Hanyuu
	*/
	void getRoomCallback(room_list_type * rl);
	void turnPages(bool);
	std::string getCachedName();
	void updateUINameList();
	int getGameRandSeed();
	int getRoomPlayerNumber()
	{
		return m_roomPlayerNumber;
	}
	void setRoomPlayerNumber(int num)
	{
		m_roomPlayerNumber = num;
	}
	short m_currentPageNumber = 0;

private:
	//static RoomModule* instance;
	std::string m_selectedIPv4;
	cocos2d::Size m_roomBoxSize;
	cocos2d::ui::Button * m_roomButton[4];
	cocos2d::ui::Button * m_roomListPrev;
	cocos2d::ui::Button * m_roomListNext;
	cocos2d::ui::Button * m_exitButton;
	cocos2d::ui::Button * m_update;
	cocos2d::ui::Button * m_freshList;
	cocos2d::Sprite * m_roomBox;

	//cocos2d::ui::Button * m_roomBox;
};

void joinCallback(bool success, std::string IPv4);

#endif // !ROOM_MODULE