//hall,h
/*
Description:大厅类头文件
Author:hanyuu
Date:18/08/24
Updated:18/09/12
*/

#ifndef HALL_H
#define HALL_H

#include <list>
#include <exception>
#include <boost/thread/thread.hpp>
#include "reader/CreatorReader.h"
#include "ui/UIWidget.h"
#include "hallPlayer.h"
#include "hallTools.h"
#include "chatroom_server.h"
#include"chatroom_client.h"
#include "GameScene.h"
#include "RoomModule.h"
#include "WelcomeScene.h"
#include "chatroom_client.h"
//#include"ui/CocosGUI.h"
//#include <string>

using namespace cocos2d;

/*
Function name:launchGame
Description:启动游戏
Created:18/09/10
*/
void launchGame();

class Hall : public RoomModule
{
public:

    std::string * getNameList()
    {
        return m_playerNameList;
    }

	//设备尺寸
	static Size m_size;
	//场景指针
	static Scene * m_scene;
	//大厅状态
	static int statusHall;
	//玩家数量
	const static int playerNumber = 4;

	Hall();

	virtual ~Hall();

	/*
	Function name:getInstance
	Description:获取Hall指针
	Created:18/09/03
	Parameter：VOID
	Return Code:static Hall*
	Author:Hanyuu
	*/
	static Hall* getInstance();

	/*
	Function name:getGameRoomInfo
	Description:获取游戏房间信息
	Created:18/09/08
	Parameter:VOID
	Return Code:st_GameRoomInfo:房间信息
	Authro:Hanyuu
	*/
	static st_GameRoomInfo getGameRoomInfo();

	/*
	Function name:initializationHall
	Description:以数组形式注册玩家信息
	Created:18/08/24
	Parameter：
		Scene:传入导出的场景指针
	Return Code:VOID
	Authro:Hanyuu
	*/
	void initializationHall(Scene * scene);

	/*
	Function name:setConfirmButtonTrue
	Description:发送信息
	Created:18/09/06
	Parameter:bool:是否显示
	Return:VOID
	Author:Hanyuu
	*/
	void setConfirmButtonTrue(bool);

	/*
	Function name : postMessage
	Description : 发送信息
	Created : 18/08/29
	Recent modified:18/09/09
	Parameter:
		int:用户ID
		string:发送者用户名
	Return : VOID
	Author : Hanyuu
	*/
	void postMessage(int, std::string);

	/*
	Function name:getEditMessage
	Description:获取信息编辑框中文本
	Created:18/08/28
	Parameter:VOID
	Return:String:返回的文本
	Author:Hanyuu
	*/
	std::string getEditMessage() const;

	/*
	Function name:clearEditBox
	Description:清除信息编辑框中的文本
	Created:18/08/29
	Parameter:VOID
	Return:VOID
	Author:Hanyuu
	*/
	void clearEditBox();

	/*
	Function name:addMessageRecord
	Description:添加新聊天记录
	Created:18/08/29
	Parameter:
		string:要上传的信息
		string:要上传的信息，int:用户ID
	Return:VOID
	Author:Hanyuu
	*/
	void addMessageRecord(std::string);

	/*
	Function name:addMessageRecord
	Description:添加新聊天记录
	Created:18/08/29
	Parameter:
		string:要上传的信息
		string:要上传的信息，int:用户ID
	Return:VOID
	Author:Hanyuu
	*/
	void addMessageRecord(std::string, int);

	/*
	Function name:getMeaasgeRecord
	Description:获取聊天记录信息
	Created:18/09/02
	Parameter:VOID
	return:string 字符串格式的聊天记录信息
	Author:Hanyuu
	*/
	std::string getMessageRecord();

	/*
	Function name:getChoiceIsHost
	Description:判断是否是Host
	Created:18/08/29
	Parameter:VOID
	Return:bool:返回是否是Host
	Author:Hanyuu
	*/
	bool getChoiceIsHost() const;

	/*
	Function name:isHost
	Description:判断是否是Host
	Created:18/08/29
	Parameter:VOID
	Return:bool:返回是否是Host
	Author:Hanyuu
	*/
	bool isHost() const;

	/*
	Function name:getMyPlayerUserName
	Description:返回当前用户名
	Created:18/08/29
	Parameter:VOID
	Return:string:用户名字符串
	Author:Hanyuu
	*/
	std::string getMyPlayerUserName() const;

	/*
	Function name:connectMessage
	Description:通讯线程
	Created:20180829
	Parameter:VOID
	Return:VOID
	Author:Hanyuu 
	*/
	//void connectMessage();

	/*
	Function name:	messageListener
	Description:监听信息
	Created:18/08/30
	Parameter:shared_ptr:传入的指针
	Return:string:VOID
	Author:Hanyuu
	*/
	void messageListener(boost::shared_ptr<ChatMessage>);

	/*
	Function name:getIPaddress
	Description:从EditBox获取IP地址
	Created:18/09/01
	Parameter:VOID
	Return:string:IP地址
	Author:Hanyuu
	*/
	std::string getIPAdress();

	/*
	Function name:setMyPlayerID
	Description:设置当前用户ID
	Created:18/09/07
	Update date:18/09/09
	Parameter:int:目标ID
	Return:VOID
	Author:Hanyuu
	*/
	void setMyPlayerID(int);

	/*
	Function name:getMyPlayerID
	Description:获取当前用户ID
	Created:18/09/07
	Update date:18/09/09
	Parameter:VOID
	Return:int::用户ID
	Author:Hanyuu
	*/
	int getMyPlayerID();

	/*
	Function name:sendMessage
	Description:发送聊天信息
	Created:18/08/26
	Parameter:String:待发送的信息变量
	Return Code:bool:发送成功判断
	Author:Hanyuu
	*/
	//bool sendChatMessage(std::string) const;

	/*
	Function name:reeateRoom
	Description:
	Created:18/09/06
	Parameter:room_list_type
	Return:VOID
	Author:Hanyuu
	*/

	void createRoom();
	/*
	Function name:setHallStatus
	Description:设置大厅界面状态 [0:未选择][1:Host][2:Client]
	Created:18/09/07
	Parameter:int:目标状态
	Return:VOID
	Author:Hanyuu
	*/
	void setHallStatus(int);

	/*
	Function name:getHallStatus
	Description:获取大厅界面状态 [0:未选择][1:Host][2:ClientHall][3:Client]
	Created:18/09/07
	Parameter:VOID
	Return:int:目标状态
	Author:Hanyuu
	*/
	int getHallStatus() const;

	/*
	Function name:hostCallBack
	Description:Host回调
	Created:18/09/07
	Updated:18/09/09
	Parameter:bool:返回成功状态
	Return:VOID
	Author:Hanyuu
	*/
	void hostCallBack(bool);

	/*
	Function name:clientCallBack
	Description:Client回调
	Created:18/09/07
	Updated:18/09/09
	Parameter:bool:返回房间列表
	Return:VOID
	Author:Hanyuu
	*/
	void clientCallBack(room_list_type *);

	/*
	Function name:hostCallBack
	Description:Host回调
	Created:18/09/07
	Updated:18/09/09
	Parameter:bool:返回成功状态
	Return:VOID
	Author:Hanyuu
	 */
	void setChoiceIsHost(bool);

	/*
	Function name messageFiltrator
	Description:信息过滤器
	Created:18/09/11
	Parameter:shared_ptr<CharMessage>传入信息
	Return:bool:过滤判定
	Author:Hanyuu
	*/
	bool messageFiltrator(boost::shared_ptr<ChatMessage>);

	/*
	Function name messageFiltrator
	Description:信息过滤器
	Created:18/09/11
	Parameter:shared_ptr<CharMessage>传入信息
	Return:bool:过滤判定
	Author:Hanyuu
	*/
	std::string getCachedName();

	/*
	Function name:updateUINameList
	Description:更新UI姓名列表
	Created:18/08/29
	Updated:18/09/12
	Parameter:VOID
	Return:bool:返回是否是Host
	Author:Hanyuu
	*/
	void updateUINameList();

	/*
	Function name:getGameRandSeed
	Description:获取同步游戏更新种子
	Created:18/09/07
	Parameter:VOID
	Return:VOID
	Author:Hanyuu
	*/
	int getGameRandSeed();
	/*
	Function name:loadingFrame
	Description:游戏启动前等待界面
	Created:18/09/11
	Parameter:VOID
	Return:VOID
	Author:Hanyuu
	*/
	void loadingFrame();
protected:
private:
	//默认用户名列表
	const st_HallPlayer m_defeatHallPlayer[playerNumber] = { {0,"[None]:0"},{1,"[None]:1"},{2,"[None]:2"},{3,"[None]:3"} };
	//Host状态表
	const bool m_hostStatus[playerNumber] = { true,false,false,false };
	//Hall instance
	static Hall * instance;
	cocos2d::Sprite * m_choiceBackground;
	cocos2d::Sprite * m_girl;
	cocos2d::Sprite * m_bubble;
	cocos2d::Sprite * m_load;
	cocos2d::ui::Button * m_play;
	cocos2d::ui::Button * m_sendButton;
	cocos2d::ui::Button * m_changeisHost;
	cocos2d::ui::Button * m_confirmStatus;
	cocos2d::ui::Button * m_closeSession;
    cocos2d::ui::Button * m_soloModal;
	cocos2d::ui::Button * m_back;
	cocos2d::ui::TextField * m_editBox;
	cocos2d::Label * m_record;
	cocos2d::Label * m_bubbleLabel;
	cocos2d::Label * m_playerNameField[playerNumber];
	//聊天记录消息链表
	std::list<st_MessageRecord> * m_messageRecord;
	//房间信息表
	room_list_type * m_hallRoom;
	//用户名表
	std::string	m_playerNameList[playerNumber];
	//从大厅获得的姓名
	std::string m_cachedName;
	//当前用户ID
	int m_myPlayerID = 0;
	//大厅界面状态:[0:未选择][1:Host][2:ClientHall][3:Client]
	int m_status = 0;
	//开局前选择是否为Host
	bool m_choiceIsHost{ false };
	//随机数种子
	int m_randSeed;

};
#endif