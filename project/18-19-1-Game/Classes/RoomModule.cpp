//RoomModule.cpp

#include "RoomModule.h"
#include "Hall.h"

void joinCallback(bool success, std::string IPv4);
//RoomModule* RoomModule::instance;

RoomModule::RoomModule()
{
	m_selected = -1;

	//RoomModule::instance = this;
	//do
	//{
	//	CC_BREAK_IF(!CCLayer::init());
	//	this->schedule(schedule_selector(RoomModule::updateInfo), 5.0f);
	//} while (false);
}
RoomModule::~RoomModule()
{
	;
}
//RoomModule* RoomModule::getInstace()
//{
//	return RoomModule::instance;
//}
void RoomModule::updateRoomList()
{

	for (unsigned short i = 0; i < 4; i++)
	{
		m_roomButton[i]->setVisible(false);
		if ((i + 4 * m_currentPageNumber) < m_roomList.size())
		{
			m_roomButton[i]->setTitleText(m_roomList.at(i + 4 * m_currentPageNumber).tag_playerName.at(0).tag_playerName);
			m_roomButton[i]->setVisible(true);
			if (m_selected != -1)
			{
				if (i == m_selected % 4)
				{
					m_roomButton[i]->setTouchEnabled(false);
					m_roomButton[i]->setBright(false);
				}
				else
				{
					m_roomButton[i]->setTouchEnabled(true);
					m_roomButton[i]->setBright(true);
				}
			}
		}
	}
}

void RoomModule::recall(bool success)
{

}

void RoomModule::roomModuleInitialize()
{
	m_roomBox = cocos2d::Sprite::create("roomBackground.png");
	//m_roomBox = cocos2d::ui::Button::create("roomBackground.png");
	m_roomBox->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
	m_roomBox->setPosition(cocos2d::Vec2(Hall::m_size.width / 2, Hall::m_size.height / 2));
	//m_roomBox->setScale(0.7);
	//m_roomBox->setOpacity(150);
	m_roomBox->setZOrder(101);
	m_roomBox->setVisible(false);
	//scene->addChild(m_roomBox);
	Hall::m_scene->addChild(m_roomBox);
	m_roomBoxSize = m_roomBox->getContentSize();

	for (unsigned i = 0; i < 4; i++)
	{
		m_roomButton[i] = cocos2d::ui::Button::create("blue.png");
		m_roomButton[i]->setAnchorPoint(cocos2d::Vec2(0.5, 0.5));
		m_roomButton[i]->setPosition(cocos2d::Vec2(m_roomBoxSize.width / 2, m_roomBoxSize.height * 3 / 4 - m_roomButton[i]->getContentSize().height * 11 / 10 * i));
		m_roomButton[i]->setTitleText("Null" + int2String(i));
		m_roomButton[i]->setTitleFontSize(30);
		//m_roomButton[i]->setOpacity(150);
		m_roomButton[i]->_ID;
		m_roomButton[i]->addTouchEventListener([&](cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type)
		{
			std::string IPv4;
			switch (type)
			{
			case cocos2d::ui::Widget::TouchEventType::BEGAN:
				break;
			case cocos2d::ui::Widget::TouchEventType::MOVED:
				break;
			case cocos2d::ui::Widget::TouchEventType::ENDED:
				unsigned id;
				for (unsigned j = 0; j < 4; j++)
				{
					if (m_roomButton[j]->_ID == sender->_ID)
					{
						id = j;
						break;
					}
				}
				m_selected = 4 * m_currentPageNumber + id;
				sender->_ID;
				m_selected = 4 * m_currentPageNumber + id;
				m_selectedIPv4 = m_roomList.at(m_selected).tag_ipv4;
				for (int j = 0; j < 4; j++)
				{
					if (id==j)
					{
						m_roomButton[j]->setTouchEnabled(false);
						m_roomButton[j]->setBright(false);
					}
					else
					{
						m_roomButton[j]->setTouchEnabled(true);
						m_roomButton[j]->setBright(true);
					}
				}
				break;
			case cocos2d::ui::Widget::TouchEventType::CANCELED:
				break;
			default:
				break;
			}
		});
		m_roomBox->addChild(m_roomButton[i]);

		m_roomListPrev = cocos2d::ui::Button::create("prev.png", "prev0.png", "prev1.png");
		m_roomListPrev->setAnchorPoint(Vec2(0, 1));
		m_roomListPrev->setPosition(Vec2(m_roomBoxSize.width * 1 / 20, m_roomBoxSize.height - m_roomBoxSize.height * 1 / 20));
		//m_roomListPrev->setOpacity(100);
		//m_roomListPrev->setScale(0.8);
		m_roomListPrev->setZOrder(90);
		m_roomListPrev->addTouchEventListener([&](cocos2d::Ref *sender, ui::Widget::TouchEventType type)
		{
			turnPages(false);
		});
		m_roomBox->addChild(m_roomListPrev);

		m_roomListNext = cocos2d::ui::Button::create("next.png", "next0.png", "next1.png");
		m_roomListNext->setAnchorPoint(Vec2(1, 1));
		m_roomListNext->setPosition(Vec2(m_roomBoxSize.width - m_roomBoxSize.width * 1 / 20, m_roomBoxSize.height - m_roomBoxSize.height * 1 / 20));
		//m_roomListNext->setOpacity(100);
		//m_roomListNext->setScale(0.8);
		m_roomListNext->setZOrder(90);
		m_roomListNext->addTouchEventListener([&](cocos2d::Ref *sender, ui::Widget::TouchEventType type)
		{
			turnPages(true);
		});
		m_roomBox->addChild(m_roomListNext);
	}
}

void joinCallback(bool success, std::string IPv4)
{
	/*
	if (success)
	{
		if (!Hall::isHost())
		{
			client_start(IPv4, boost::bind(&Hall::messageListener, Hall::getInstance(), _1));
		}else
		{
			server_start(boost::bind(&Hall::messageListener, Hall::getInstance(), _1));
		}
	}
	else
	{
		setConfirmButtonTrue();
	}
	*/
}

void RoomModule::setRoomModuleVisible(bool isVisible)
{
	m_roomBox->setVisible(isVisible);
}

std::string RoomModule::getHostName()
{
	return m_roomList.at(m_selected).tag_playerName.at(0).tag_playerName;
}
std::string RoomModule::getHostIP()
{
	if (m_selected > -1)
	{
		return m_roomList.at(m_selected).tag_ipv4;
	}
	else
	{
		return "";
	}
}
int RoomModule::getPlayerNumber()
{
	return m_roomList.at(m_selected).tag_playerNum;
}
void RoomModule::getRoomCallback(room_list_type* rl)
{
	m_roomList.clear();
	for (int i = 0; i < m_roomPlayerNumber; i++)
	{
		auto sr = rl->at(i);
		st_RoomInfo room;
		room.tag_ipv4 = sr->ipv4;
		room.tag_playerNum = sr->playernum;
		for (int j = 0; j < room.tag_playerNum; j++)
		{
			room.tag_playerName.at(j).tag_ID = j;
			room.tag_playerName.at(j).tag_playerName = sr->nameList.at(j);
		}
		m_roomList.push_back(room);
	}
	updateRoomList();
	hanyuuLog("timer");
}
void RoomModule::turnPages(bool isFront)
{
	if (isFront)
	{
		if ((int)m_roomList.size() - (int)(m_currentPageNumber + 1) * 4 > 0)
		{
			++m_currentPageNumber;
		}
	}
	else
	{
		if (m_currentPageNumber > 0)
		{
			--m_currentPageNumber;
		}
	}
	updateRoomList();
}