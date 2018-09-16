//FinishScene.cpp
#include "FinishScene.h"
#include "CommonSetting.h"
#include "hallTools.h"
#include "WelcomeScene.h"
//#include "hallScene.h"
//#include "AppDelegate.h"

FinishScene * FinishScene::instance;

FinishScene::FinishScene()
{
	FinishScene::instance = this;
	reader = creator::CreatorReader::createWithFilename("creator/Scene/endScene.ccreator");
	reader->setup();
	m_scene = reader->getSceneGraph();
	for (int i = 0; i < Hall::playerNumber; i++)
	{
		m_nameField[i] = (cocos2d::Label *)m_scene->getChildByName("Canvas")->getChildByName("roomBackground")
			->getChildByName("head-" + int2String(i))->getChildByName("PlayName");
		m_GPAField[i] = (cocos2d::Label *)m_scene->getChildByName("Canvas")->getChildByName("roomBackground")
			->getChildByName("head-" + int2String(i))->getChildByName("Info");
		//m_nameField[i]->setString(Hall::getInstance()->getNameList()[i]);

		//m_nameField[i]->setString(CSET->player[i].getName());
		//m_GPAField[i]->setString(float2String(CSET->player[i].getScore()));		
		//test
		m_nameField[i]->setString(int2String(i));
		m_GPAField[i]->setString(int2String(i));
	}
	m_closeButton = cocos2d::ui::Button::create("close.png","close0.png","close1.png");
	m_closeButton->setAnchorPoint(cocos2d::Vec2(1, 1));
	m_closeButton->setPosition(cocos2d::Vec2(Hall::m_size.width * 9 / 10, Hall::m_size.height * 9 / 10));
	m_closeButton->addTouchEventListener([&](Ref* sneder, ui::Widget::TouchEventType type)
	{			
		switch (type)
		{
		case cocos2d::ui::Widget::TouchEventType::BEGAN:
			break;
		case cocos2d::ui::Widget::TouchEventType::MOVED:
			break;
		case cocos2d::ui::Widget::TouchEventType::ENDED:
			CCDirector::sharedDirector()->end();
			break;
		case cocos2d::ui::Widget::TouchEventType::CANCELED:
			break;
		default:
			break;
		}
	});
	m_scene->addChild(m_closeButton);
}

FinishScene * FinishScene::getInstance()
{
	return instance;
}

cocos2d::Scene * FinishScene::createScene()
{
	return m_scene;
}

FinishScene::~FinishScene()
{

}