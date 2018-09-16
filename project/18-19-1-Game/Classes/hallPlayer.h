//hallPlayer.h
/*
Description:大厅类结构体
Author:hanyuu
Date:18/08/24
*/
#ifndef HALL_PLAYER_H
#define HALL_PLAYER_H

#include <string>

//玩家信息
struct st_HallPlayer
{
	//玩家ID
	int tag_ID;
	//玩家姓名
	std::string tag_playerName;
};

//信息
struct st_MessageRecord
{
	//发送者ID
	int tag_ID;
	//消息内容
	std::string tag_message;
};

//房间信息
struct st_RoomInfo
{
	std::string tag_ipv4;
	int tag_playerNum;
	std::vector<st_HallPlayer> tag_playerName;
};

//房间信息
struct st_GameRoomInfo
{
	int currentPlayerNumber;
	int playerNumber;
	std::string ipv4;
};

//返回战斗成功结构体
struct st_BattleFinishInfo
{
	std::string tag_name;
	float tag_GPA;
};
#endif // !HALL_PLAYER_H