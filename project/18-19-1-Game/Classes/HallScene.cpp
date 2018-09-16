//HallScene.cpp
#include "HallScene.h"

//Hall HallScene::m_hall;

CocosDenshion::SimpleAudioEngine* HallScene::audio;
HallScene * HallScene::instance;
bool HallScene::init()
{
	if (!Scene::init())
	{
		return false;
	}
	return true;
}

HallScene::HallScene()
{
	instance = this;
	audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("HallMusicR.mp3", true);

	reader = creator::CreatorReader::createWithFilename("creator/Scene/Hall.ccreator");
	reader->setup();
	//Hall::m_scene = reader->getSceneGraph();
	Hall::Hall::m_scene = reader->getSceneGraph();
	initializationHall(Hall::m_scene);
	scheduleUpdate();
}

void HallScene::stopBackgroundMusic()
{
	audio->stopBackgroundMusic();
}

cocos2d::Scene * HallScene::createScene()
{
	return Hall::m_scene;
}

HallScene::~HallScene()
{
	//delete Hall::m_scene;
}

HallScene* HallScene::getInstance()
{
	return instance;
}
