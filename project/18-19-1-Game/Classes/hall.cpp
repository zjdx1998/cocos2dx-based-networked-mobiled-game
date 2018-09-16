//hallFunction.cpp
//Author:hanyuu
//Commit:
//	提供控制大厅的类

#include "hall.h"
#include "WelcomeScene.h"
#include "chatroom_client.h"
#include "HallScene.h"

Hall* Hall::instance;

cocos2d::Size Hall::m_size;
cocos2d::Scene* Hall::m_scene;

////互斥锁
//boost::mutex lock;
//Hall * Hall::instance = nullptr;

Hall::Hall()
{
	m_cachedName = WelcomeScene::getInstance()->getPlayerName();
	hanyuuLog("-**Start log**-\nObject Hall generated.");
	//Hall::instance = this;
}

Hall::~Hall()
{
	//delete m_record;
	//delete m_sendButton;
	//delete m_changeisHost;
	//delete m_editBox;
	//delete m_IPConfig;
	//delete ptr;
	//delete cptr;
	//delete m_room;
	delete m_messageRecord;

	hanyuuLog("Object Hall destroyed.\n-**End log**-\n");
}

//Hall * Hall::getInstance()
//{
//	return instance;
//}

void ontouch(Ref * sender, ui::Widget::TouchEventType type)
{

}

void Hall::initializationHall(Scene * scene)
{
	//建立聊天记录
	m_messageRecord = new std::list<st_MessageRecord>;

	//获取可视尺寸
	m_size = Director::getInstance()->getVisibleSize();

	//导入场景
	if (scene != nullptr)
	{
		m_scene = scene;
		//初始化发送按钮
		m_sendButton = (ui::Button*) m_scene->getChildByName("hallCanvas")->getChildByName("shadowVertcial")->getChildByName("hallChatMessageSendButton");
		m_sendButton->setTitleText("Send");
		m_sendButton->setTitleFontSize(30);
		m_sendButton->addTouchEventListener([&](Ref* sneder, ui::Widget::TouchEventType type)
		{
			std::string editMessage = getEditMessage();
			switch (type)
			{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				//hanyuuLog("Send button touch began.");
				break;
			case cocos2d::ui::Widget::TouchEventType::MOVED:
				//hanyuuLog("Send button touch moved.");
				break;
			case cocos2d::ui::Widget::TouchEventType::ENDED:
				//sendChatMessage(getEditMessage());
				//addMessageRecord(getEditMessage());
				if (getEditMessage() == "")
				{
					break;
				}
				postMessage(getMyPlayerID(), getEditMessage());
				clearEditBox();
				//hanyuuLog("Send button touch ended.");
				break;
			case cocos2d::ui::Widget::TouchEventType::CANCELED:
				hanyuuLog("Send button touch canceled.");
				break;
			default:
				hanyuuLog("Send button touch defalult.");
				break;
			}
		});
       

		//进入游戏按钮
		m_play = cocos2d::ui::Button::create("button0.png", "button1.png", "button2.png");
		m_play->setTitleText("Play");
		//m_changeisHost->setTitleFontName("微软雅黑");
		m_play->setTitleFontSize(50);
		m_play->setAnchorPoint(Vec2(0.5, 0.5));
		//m_play->setPosition(Vec2(30.0f, m_size.height - 200.0f));
		m_play->addTouchEventListener([&](Ref* sneder, ui::Widget::TouchEventType type)
		{
			std::string editMessage = getEditMessage();
			switch (type)
			{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				//hanyuuLog("Send button touch began.");
				Hall::getInstance()->loadingFrame();
				break;
			case cocos2d::ui::Widget::TouchEventType::MOVED:
				//hanyuuLog("Send button touch moved.");
				break;
			case cocos2d::ui::Widget::TouchEventType::ENDED:
				//gameScene
			{
				//auto gameScene = GameScene::create();
				//gameScene->retain();
				//Director::getInstance()->replaceScene(TransitionFade::create(0.5, gameScene, Color3B(0, 255, 255)));
				if (isHost())
				{
					ChatMessage msg;
					msg.message = "-1";
					msg.playerID = -1;
					ChatroomServer::get_instance()->post(msg);
					//发送房主已进入房间消息
					HttpService::getInstance()->get_my_room(
						[this](room_ptr_type roomptr)
					{
						m_roomPlayerNumber = roomptr->playernum;
						launchGame();
					});
				}
				else {
					HttpService::getInstance()->set_getoneroom_cb(
						[this](room_ptr_type roomptr)
					{
						m_roomPlayerNumber = roomptr->playernum;
						launchGame();
					});
					string hostip = Hall::getInstance()->getHostIP();
					HttpService::getInstance()->getOneRoom(Hall::getInstance()->getHostIP());
				}
				
			}
			break;
			case cocos2d::ui::Widget::TouchEventType::CANCELED:
				hanyuuLog("Send button touch canceled.");
				break;
			default:
				hanyuuLog("Send button touch defalult.");
				break;
			}
		});
		m_scene->addChild(m_play);

		//初始化信息输入窗口
		m_editBox = cocos2d::ui::TextField::create("Click here and input message", "Arial", 30);
		m_editBox->setFontSize(50);
		m_editBox->setPlaceHolder("Click here and input message");
		m_editBox->setAnchorPoint(Vec2(0, 0));
		m_editBox->setPosition(Vec2(m_size.width / 2, 0));
		m_editBox->setMaxLength(20);
		m_editBox->setZOrder(51);
		m_scene->addChild(m_editBox);

		//初始化聊天记录框
		m_record = cocos2d::Label::create("Let's talk!", "Arial", 30);
		//m_record = cocos2d::Label::create("Let's talk!","Arial",30);
		//m_record = cocos2d::ui::Text::create("Let's talk!","Arial",30);
		m_record->setAnchorPoint(Vec2(0, 0));
		m_record->setPosition(Vec2(m_size.width/2, m_editBox->getContentSize().height));
		m_record->setZOrder(50);
		m_scene->addChild(m_record);

		//ClientIP输入框
		//m_IPConfig = cocos2d::ui::TextField::create("127.0.0.1", "Arial", 30);
		//m_IPConfig->setFontSize(30);
		//m_IPConfig->setPlaceHolder("Input service's IP here");
		//m_IPConfig->setAnchorPoint(Vec2(0, 1));
		//m_IPConfig->setPosition(Vec2(50, m_size.height - 150));
		//m_IPConfig->setMaxLength(20);
		//m_IPConfig->setZOrder(101);
		//m_IPConfig->setVisible(true);
		//m_scene->addChild(m_IPConfig);

		//选择是否是Host
		m_changeisHost = cocos2d::ui::Button::create("button0.png", "button1.png", "button2.png");
		m_changeisHost->setTitleText("Join");
		//m_changeisHost->setScale(2);
		m_changeisHost->setZOrder(102);
		m_changeisHost->setTitleFontSize(50);
		//m_changeisHost->setAnchorPoint(Vec2(0.5, 0.5));
		//m_changeisHost->setPosition(Vec2(m_size.width / 2, m_size.height / 2 + 100));
		m_changeisHost->addTouchEventListener([&](Ref * sender, ui::Widget::TouchEventType type)
		{

			switch (type)
			{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				hanyuuLog("HOST TOUCHED");
				break;
			case cocos2d::ui::Widget::TouchEventType::MOVED:
				break;
			case cocos2d::ui::Widget::TouchEventType::ENDED:
				setChoiceIsHost(!getChoiceIsHost());
				if (getChoiceIsHost())
				{
					hanyuuLog("Turn to host now");
					m_changeisHost->setTitleText("Create");
					//RoomModule::setRoomModuleVisible(false);
				}
				else
				{
					hanyuuLog("Turn to client now");
					m_changeisHost->setTitleText("Join");
					//RoomModule::setRoomModuleVisible(true);
				}
				break;
			case cocos2d::ui::Widget::TouchEventType::CANCELED:
				break;
			default:
				break;
			}
		}
		);
		m_scene->addChild(m_changeisHost);
		//Host装填确认/房间加入按钮
		m_confirmStatus = cocos2d::ui::Button::create("button0a.png", "button1a.png", "button2a.png");
		m_confirmStatus->setTitleText("Okey");
		//m_confirmStatus->setScale(2);
		m_confirmStatus->setZOrder(103);
		m_confirmStatus->setTitleFontSize(50);
		//m_confirmStatus->setAnchorPoint(Vec2(0.5, 0.5));
		//m_confirmStatus->setPosition(Vec2(m_size.width / 2, m_size.height / 2 - 100));
		m_confirmStatus->addTouchEventListener([&](Ref * sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				hanyuuLog("HOST TOUCHED");
				break;
			case cocos2d::ui::Widget::TouchEventType::MOVED:
				break;
			case cocos2d::ui::Widget::TouchEventType::ENDED:
				////聊天室收发
				//m_changeisHost->setTouchEnabled(false);
				//m_changeisHost->setBright(false);
				//m_confirmStatus->setTouchEnabled(false);
				//m_confirmStatus->setBright(false);
				////m_IPConfig->setVisible(true);
				//if (isHost())
				//{
				//	HttpService::getInstance()->set_joinroom_cb(boost::bind(joinCallback, _1, IP));
				//	HttpService::getInstance()->openRoom(IP, "qe");
				//}
				//按钮复用
				//Host/Client选择界面
				if (0 == getHallStatus())
				{
					if (getChoiceIsHost())
					{
						//Host模式
						HttpService::getInstance()->set_joinroom_cb(boost::bind(&Hall::hostCallBack, this, _1));
						HttpService::getInstance()->createRoom(m_cachedName);
						//setHallStatus(1);
					}
					else
					{
						//Client模式
						setHallStatus(2);
					}
				}
				//Client房间列表
				else if (2 == getHallStatus())
				{
					if (-1 != m_selected || "" == getIPAdress())
					{
						HttpService::getInstance()->joinRoom(RoomModule::getHostIP(), m_cachedName);
						client_start(RoomModule::getHostIP(), boost::bind(&Hall::messageListener, this, _1));

						for (int i = 0; i < m_roomList.at(m_selected).tag_playerNum; i++)
						{
							m_playerNameList[i]=m_roomList.at(m_selected).tag_playerName.at(i).tag_playerName;
						}
						m_roomPlayerNumber = m_roomList.at(m_selected).tag_playerNum;	//获得当前房间的人数给
						updateUINameList();
						setHallStatus(3);
					}
				}
				break;
			case cocos2d::ui::Widget::TouchEventType::CANCELED:
				break;
			default:
				break;
			}
		});
		m_scene->addChild(m_confirmStatus);

		//返回state1页面
		m_back = cocos2d::ui::Button::create("button0a.png", "button1a.png", "button2a.png");
		m_back->setTitleText("Back");
		m_back->setZOrder(104);
		//m_back->setAnchorPoint(Vec2(0.5, 0.5));
		//m_back->setScale(2);
		m_back->setVisible(false);

		m_back->addTouchEventListener([&](cocos2d::Ref *sender, ui::Widget::TouchEventType type)
		{
			switch (type)
			{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				break;
			case cocos2d::ui::Widget::TouchEventType::MOVED:
				break;
			case cocos2d::ui::Widget::TouchEventType::ENDED:
				if (1 == getHallStatus())
				{
					//Host
					//HttpService::getInstance()->closeRoom();
					postMessage(-1, int2String(-2));
				}
				else if (3 == getHallStatus())
				{
					//client
					ChatroomClient::get_instance()->close();
				}
				setHallStatus(0);
				break;
			case cocos2d::ui::Widget::TouchEventType::CANCELED:
				break;
			default:
				break;
			}
		});
		m_scene->addChild(m_back);

		hanyuuLog("Class hall get scene sucess.");
	}
	else
	{
		hanyuuLog("Class Hall can't touch scene");
		return;
	}

	//精灵人物
	m_girl = (cocos2d::Sprite*) m_scene->getChildByName("hallCanvas")->getChildByName("shadowVertcial")->getChildByName("sc");
	m_girl->setZOrder(200);
	//精灵对话气泡
	m_bubble = (cocos2d::Sprite*) m_scene->getChildByName("hallCanvas")->getChildByName("bubble");
	m_bubble->setZOrder(201);
	m_bubbleLabel = (cocos2d::Label*)m_scene->getChildByName("hallCanvas")->getChildByName("bubble")->getChildByName("label");
	m_bubbleLabel->setZOrder(202);
	m_bubbleLabel->setString("Wait Host to start.");
	//待选择界面
	m_choiceBackground = cocos2d::Sprite::create("background.jpg");
	m_choiceBackground->setPosition(cocos2d::Vec2(Hall::m_size.width / 2, Hall::m_size.height / 2));
	m_choiceBackground->setZOrder(100);
	//m_choiceBackground->setScale(3);
	m_scene->addChild(m_choiceBackground);

	std::string strHead = "bleb";

	hanyuuLog("[Player information]:");
	for (unsigned i = 0; i < Hall::playerNumber; i++)
	{
		std::string strName = strHead + int2String(i);
		m_playerNameField[i] = (cocos2d::Label *) m_scene->getChildByName("hallCanvas")->getChildByName(strName);
		//player->setZOrder(100);
		m_playerNameList[i] = m_defeatHallPlayer[i].tag_playerName;
		m_playerNameField[i]->setString(m_defeatHallPlayer[i].tag_playerName);
		hanyuuLog(m_defeatHallPlayer[i].tag_playerName);
	}
	m_load = cocos2d::Sprite::create("load.png");
	m_load->setVisible(false);
	m_scene->addChild(m_load);
	m_roomPlayerNumber = 1;
	roomModuleInitialize();
	instance = this;
	setHallStatus(0);
}

//void Hall::setConfirmButtonTrue(bool isVisible)
//{
//	m_changeisHost->setTouchEnabled(isVisible);
//	m_changeisHost->setBright(isVisible);
//	m_confirmStatus->setTouchEnabled(isVisible);
//	m_confirmStatus->setBright(isVisible);
//}

void launchGame()
{
	Hall::getInstance()->loadingFrame();
	auto gameScene = GameScene::create();
	gameScene->retain();
	Director::getInstance()->replaceScene(TransitionFade::create(0.5, gameScene, Color3B(0, 255, 255)));
}

std::string Hall::getEditMessage() const
{
	if (m_editBox)
	{
		return m_editBox->getString();
	}
	else
	{
		return std::string("No message");
	}
}

void Hall::clearEditBox()
{
	if (m_editBox)
	{
		m_editBox->setString("");
	}
}

void Hall::addMessageRecord(std::string str)
{
	m_messageRecord->push_back(st_MessageRecord{ m_myPlayerID, str });
	m_record->setString(getMessageRecord());
	hanyuuLog("[Chat record]\n" + getMessageRecord() + "\n");
}

void Hall::addMessageRecord(std::string str, int ID)
{
	m_messageRecord->push_back(st_MessageRecord{ ID,str });
	m_record->setString(getMessageRecord());
	hanyuuLog("[Chat record]\n" + getMessageRecord() + "\n");
}

std::string Hall::getMessageRecord()
{
	std::string temp;
	int i{ 0 };

	for (std::list<st_MessageRecord>::reverse_iterator ptr = m_messageRecord->rbegin(); i < 10 && ptr != m_messageRecord->rend(); ++ptr)
	{
		if (ptr->tag_ID >= 0 && ptr->tag_ID < 4)
		{
			temp = "\n[" + m_playerNameList[ptr->tag_ID] + "]" + ptr->tag_message + temp;
		}
		else
		{
			temp = "\nSystem message" + int2String(ptr->tag_ID) + ":" + ptr->tag_message + temp;
		}
		i++;
	}
	return temp;
}

bool Hall::isHost() const
{
	return m_hostStatus[m_myPlayerID];
}

bool Hall::getChoiceIsHost() const
{
	return m_choiceIsHost;
}

std::string Hall::getMyPlayerUserName() const
{
	return m_playerNameList[m_myPlayerID];
}

//void connectMessage()
//{
//	hanyuuLog("Connect message thread lunched.");
//	client_start("127.0.0.1", clientGetMessage, cptr);
//}

void Hall::messageListener(boost::shared_ptr<ChatMessage> mp)
{
	if (messageFiltrator(mp))
	{
		addMessageRecord(mp->message, mp->playerID);
	}
}
bool Hall::messageFiltrator(boost::shared_ptr<ChatMessage> mp)
{
	switch (mp->playerID)
	{
	case -1:
		switch (string2Int(mp->message))
		{
			//房主已退房
		case -2:
			if (isHost())
			{
				//HttpService::getInstance()->closeRoom();
			}
			else
			{
				HttpService::getInstance()->leaveRoom(getHostIP());
			}
			return false;
			break;
			//房主已进入游戏
		case -1:
			if (!isHost())
			{
				//scheduleOnce(schedule_selector(Hall::launchGame), 1.0f);
				//launchGame();
				m_play->setVisible(true);
			}
			return false;
			break;
		default:
			//加入房间时分配的ID
			if (string2Int(mp->message) < 4 && string2Int(mp->message) > 0)
			{
				hanyuuLog("get my ID:" + mp->message);
				setMyPlayerID(string2Int(mp->message));
				//postMessage(getMyPlayerID() - 8, m_cachedName);
				m_playerNameList[getMyPlayerID()] = m_cachedName;

				//广播新加入房间的玩家名
				ChatMessage msg;
				msg.playerID = -8 + getMyPlayerID();
				msg.message = getMyPlayerUserName();
				ChatroomClient::get_instance()->post(msg);

				updateUINameList();
			}
			//退出房间的ID
			else if (string2Int(mp->message) < 8 && string2Int(mp->message) > 3)
			{
				if (isHost())
				{
					ChatroomServer::get_instance()->close_sock(string2Int(mp->message) - 4);
				}
				m_playerNameList[string2Int(mp->message) - 4] = m_defeatHallPlayer[string2Int(mp->message) - 4].tag_playerName;
				updateUINameList();
			}
			//未定义的数据格式
			else
			{
				hanyuuLog("Error:[PlayerID]" + int2String(mp->playerID) + "[Message]" + mp->message);
			}
			return false;
			break;
		}
		return false;
		break;
		//新玩家加入,已确认无需break；
	case -8:	//ID=0
	case -7:	//ID=1
	case -6:	//ID=2
	case -5:	//ID=3
		++m_roomPlayerNumber;
		m_playerNameList[mp->playerID + 8] = mp->message;
		updateUINameList();
		return false;
		break;
	case -3:
		//设置随机数种子
		m_randSeed = string2Int(mp->message);
		return false;
		break;
	default:
		break;
	}
	return true;
}
void  Hall::postMessage(int ID, std::string strMessage)
{
	hanyuuLog("[" + int2String(ID) + " Post message]\n    " + strMessage);
	ChatMessage  CMMessageFrame;
	CMMessageFrame.playerID = ID;
	CMMessageFrame.message = strMessage;
	if (isHost())
	{
		if (ChatroomServer::get_instance() != nullptr)
			ChatroomServer::get_instance()->post(CMMessageFrame);
	}
	else
	{
		ChatroomClient::get_instance()->post(CMMessageFrame);
	}
}


std::string Hall::getIPAdress()
{
	return "hello world";
}
void Hall::setMyPlayerID(int target)
{
	m_myPlayerID = target;
	for (unsigned i = 0; i < Hall::playerNumber; i++)
	{
		if (i == target)
		{
			m_playerNameField[i]->setTextColor(cocos2d::Color4B(105, 176, 172, 255));
		}
		else
		{
			m_playerNameField[i]->setTextColor(cocos2d::Color4B(255, 255, 255, 255));
		}
	}
}
int Hall::getMyPlayerID()
{
	return m_myPlayerID;
}

void Hall::hostCallBack(bool success)
{
	if (success)
	{
		m_playerNameList[0] = m_cachedName;
		if (ChatroomServer::get_instance() != nullptr)
		{
			//delete ChatroomServer::get_instance();
		}
		server_start(boost::bind(&Hall::messageListener, this, _1));
		//++m_roomPlayerNumber;
		updateUINameList();
		setHallStatus(1);
	}
	else
	{
		//创建房间失败,回到带选择界面
		setHallStatus(0);
	}
}
void Hall::clientCallBack(room_list_type * roomList)
{
	m_roomList.clear();
	if (0 != roomList->size())
	{
		for (unsigned i = 0; i < roomList->size(); i++)
		{
			st_RoomInfo temp;
			temp.tag_ipv4 = roomList->at(i)->ipv4;
			temp.tag_playerNum = roomList->at(i)->playernum;
			for (int j = 0; j < temp.tag_playerNum; j++)
			{
				temp.tag_playerName.push_back(st_HallPlayer{ j,roomList->at(i)->nameList.at(j) });
			}
			m_roomList.push_back(temp);
		}
	}
	updateRoomList();
}

int Hall::getHallStatus() const
{
	return m_status;
}

void Hall::setChoiceIsHost(bool target)
{
	m_choiceIsHost = target;
}

void Hall::setHallStatus(int status)
{
	switch (status)
	{
		//未选择模式
	case 0:
		m_status = 0;
		m_choiceBackground->setVisible(true);
		m_changeisHost->setVisible(true);
		m_changeisHost->setPosition(Vec2(m_size.width * 1 / 2, m_size.height * 3 / 5));
		m_confirmStatus->setVisible(true);
		m_play->setVisible(false);
		setRoomModuleVisible(false);
		m_back->setVisible(false);
		setRoomModuleVisible(false);
		m_confirmStatus->setTitleText("confirm");
		m_confirmStatus->setPosition(Vec2(m_size.width * 1 / 2, m_size.height * 2 / 5));
		break;
		//Host模式
	case 1:
		m_status = 1;
		m_choiceBackground->setVisible(false);
		m_changeisHost->setVisible(false);
		m_confirmStatus->setVisible(false);
		setRoomModuleVisible(false);
		m_play->setVisible(true);
		m_play->setPosition(Vec2(m_size.width * 1 / 10, m_size.height * 8 / 10));
		m_back->setTitleFontSize(50);
		m_back->setPosition(Vec2(m_size.width * 1 / 10, m_size.height * 9 / 10));
		m_back->setVisible(true);
		setMyPlayerID(0);
		break;
		//Client大厅模式
	case 2:
		m_status = 2;
		m_choiceBackground->setVisible(true);
		m_changeisHost->setVisible(false);
		m_confirmStatus->setVisible(true);
		setRoomModuleVisible(true);
		m_confirmStatus->setPosition(Vec2(m_size.width * 3 / 5, m_size.height / 20));
		m_confirmStatus->setTitleText("join");
		m_play->setVisible(false);
		m_back->setVisible(true);
		m_back->setPosition(Vec2(m_size.width * 2 / 5, m_size.height / 20));
		m_back->setTitleFontSize(50);

		//m_back->setVisible(false);
		HttpService::getInstance()->set_getrooms_cb(boost::bind(&Hall::clientCallBack, this, _1));
		HttpService::getInstance()->getRoomsViaHttp();
		updateRoomList();
		break;
		//Client模式
	case 3:
		m_status = 3;
		m_choiceBackground->setVisible(false);
		m_changeisHost->setVisible(false);
		m_confirmStatus->setVisible(false);
		setRoomModuleVisible(false);
		m_confirmStatus->setPosition(Vec2(m_size.width / 10, m_size.height * 1 / 10));
		m_confirmStatus->setTitleText("Okey");
		m_play->setPosition(Vec2(m_size.width * 1 / 10, m_size.height * 8 / 10));
		m_play->setVisible(false);
		//m_play->setVisible(true);
		m_back->setTitleFontSize(50);
		m_back->setPosition(Vec2(m_size.width * 1 / 10, m_size.height * 9 / 10));
		m_back->setVisible(true);
		//HttpService::getInstance()->set_joinroom_cb(boost::bind(joinCallback, _1, IP));
		//HttpService::getInstance()->joinRoom(IP, "TestUserClient");
		//server_start(boost::bind(&Hall::messageListener, this, _1));
		break;
	default:
		break;
	}
}

Hall* Hall::getInstance()
{
	return instance;
}
st_GameRoomInfo Hall::getGameRoomInfo()
{
	return st_GameRoomInfo
	{
		Hall::getInstance()->m_myPlayerID,
		2,
		//Hall::getInstance()->getPlayerNumber(),
		//Hall::getInstance()->getHostIP(),
		"127.0.0.1"
	};
}
std::string Hall::getCachedName()
{
	return m_cachedName;
}
void Hall::updateUINameList()
{
	hanyuuLog("[Player information]:");
	std::string strHead = "bleb";
	for (unsigned i = 0; i < Hall::playerNumber; i++)
	{
		std::string strName = strHead + int2String(i);
		m_playerNameField[i] = (cocos2d::Label *) m_scene->getChildByName("hallCanvas")->getChildByName(strName);
		m_playerNameField[i]->setString(m_playerNameList[i]);
		hanyuuLog(m_defeatHallPlayer[i].tag_playerName);
	}
}
int Hall::getGameRandSeed()
{
	return m_randSeed;
}
void Hall::loadingFrame()
{
	HallScene::stopBackgroundMusic();
	m_load->setVisible(true);
	m_load->setPosition(Vec2(m_size.width / 2, m_size.height / 2));
	m_load->setZOrder(200);
}