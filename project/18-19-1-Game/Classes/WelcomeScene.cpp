/****************************************************************************
 描述：开始游戏页面
 作者：张睦婕
 时间：2018/8/27
 ****************************************************************************/

#include "WelcomeScene.h"
#include "SimpleAudioEngine.h"
#include "reader/CreatorReader.h"
#include "GameScene.h"
#include <stdlib.h>
#include "playerName.h"
#include "HallScene.h"
#include <time.h>
#define random(x) (rand()%x)

USING_NS_CC;
using namespace std;
//extern char * playerNameData[];

WelcomeScene* WelcomeScene::instance = nullptr;
CocosDenshion::SimpleAudioEngine* WelcomeScene::audio;

string WelcomeScene::getPlayerName()
{
	playerName = text->getString();
	if(playerName=="")
	{
		srand(time(0));
		int i = random(889);
		//playerName = string(playerNameData[i]);
		playerName = string("user"+int2String(i));
	}
	return playerName;
}

WelcomeScene * WelcomeScene::getInstance()
{
	return WelcomeScene::instance;
}


Scene* WelcomeScene::createScene()
{
	return WelcomeScene::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
	printf("Error while loading: %s\n", filename);
	printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool WelcomeScene::init()
{
	//////////////////////////////
	// 1. super init first
	if (!Scene::init())
	{
		return false;
	}
	loadScene();//加载场景
	audio = CocosDenshion::SimpleAudioEngine::getInstance();
	audio->playBackgroundMusic("WelcomeSceneMusic.mp3", true);

	return true;
}

void WelcomeScene::loadScene()
{

	//add a scene node test
	creator::CreatorReader* reader = creator::CreatorReader::createWithFilename("creator/start.ccreator");
	// will create the needed spritesheets + design resolution
	reader->setup();
	// get the scene graph
	Scene* scene = reader->getSceneGraph();
	if (scene == nullptr)
	{
		problemLoading("'creator/start.ccreator'");
	}
	else
	{
		instance = this;
		auto visibleSize = Director::getInstance()->getVisibleSize();
		auto origin = Director::getInstance()->getVisibleOrigin();


		// add the scene as a child to this layer
		this->addChild(scene);
		auto button = (cocos2d::ui::Button*)(scene->getChildByName("Canvas")->getChildByName("button"));
		auto sky = scene->getChildByName("Canvas")->getChildByName("bkg_start")->getChildByName("star");
		auto sprite = (Sprite*)(scene->getChildByName("Canvas")->getChildByName("sprite"));
		auto about_us = (cocos2d::ui::Button*)(scene->getChildByName("Canvas")->getChildByName("about"));
		

		//姓名输入框
		text = cocos2d::ui::TextField::create("Click here and input your name", "Arial", 30);
		Vec2 center = Vec2(visibleSize.width / 2 + origin.x, visibleSize.height / 2 + origin.y);
		text->setFontSize(30);
		//text->setPlaceHolder(playerName);
		text->setPosition(center);
		text->setMaxLength(20);
		text->setZOrder(51);
		this->addChild(text);

		auto input = Sprite::create("input.png");
		input->setPosition(center);
		//input->setScale(3.7f);
		this->addChild(input);

		//游戏开始按钮
		button->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				break;
			case ui::Widget::TouchEventType::ENDED:
				//go to hall scene
			{
				/*auto gamescene = GameScene::create();
				gamescene->retain();
				Director::getInstance()->replaceScene(gamescene);*/

				static HallScene hallScene;
				auto hall = hallScene.createScene();
				Director::getInstance()->replaceScene(hall);
			}
			break;
			default:
				break;
			}
		});
		
		aboutSprite = Sprite::create("about_us.png");
	    close = cocos2d::ui::Button::create("closeAbout.png", "closeAbout.png", "closeAbout.png");
		aboutSprite->setPosition(center);
		auto about_size = aboutSprite->getBoundingBox().size;
		close->setPosition(Vec2(center.x + about_size.width / 2.6, center.y + about_size.height / 2.4));
		aboutSprite->setVisible(false);
		close->setVisible(false);
		this->addChild(aboutSprite, 100);
		this->addChild(close, 110);

		about_us->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type){
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				break;
			case ui::Widget::TouchEventType::ENDED:
			{
				aboutSprite->setVisible(true);
				close->setVisible(true);
			}
			break;
			default:
				break;
			}
		});

		close->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type) {
			switch (type)
			{
			case ui::Widget::TouchEventType::BEGAN:
				break;
			case ui::Widget::TouchEventType::ENDED:
			{
				aboutSprite->setVisible(false);
				close->setVisible(false);
			}
			break;
			default:
				break;
			}
		});

		// add a princess walk on screen

		auto size = sprite->getBoundingBox().size;
		sprite->setPosition(Vec2(origin.x+size.width, origin.y+size.height));
		sprite->setScale(7);
		auto moveBy = MoveBy::create(10.0f, Vec2(1500, 0));
		auto moveBack = MoveBy::create(1.0f, Vec2(-1500, 0));
		// fades in the sprite in 1 seconds
		auto fadeIn = FadeIn::create(1.0f);
		// Move sprite 
		auto fadeOut = FadeOut::create(1.0f);

		// create a sequence with the actions and callbacks
		auto seq = Sequence::create(fadeIn, moveBy, fadeOut, moveBack, nullptr);

		// create the animation out of the frames

		Vector<SpriteFrame*> animFrames(8);
		char str[100] = { 0 };
		for (int i = 0; i < 8; i++)
		{
			sprintf(str, "princess_walk_00%d.png", (i + 1));
			auto frame = SpriteFrame::create(str, Rect(0, 0, 100, 100));
			animFrames.pushBack(frame);
		}

		auto animation = Animation::createWithSpriteFrames(animFrames, 0.15f);
		sprite->runAction(RepeatForever::create(Animate::create(animation)));
		sprite->runAction(RepeatForever::create(seq));

		auto starFadeOut = FadeOut::create(0.5f);
		auto starFadeIn = FadeIn::create(0.5f);
		auto bling = Sequence::create(starFadeOut, starFadeIn, nullptr);
		sky->runAction(RepeatForever::create(bling));
	}

}


void WelcomeScene::menuCloseCallback(Ref* pSender)
{
	//Close the cocos2d-x game scene and quit the application
	Director::getInstance()->end();

#if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
	exit(0);
#endif

	/*To navigate back to native iOS screen(if present) without quitting the application  ,do not use Director::getInstance()->end() and exit(0) as given above,instead trigger a custom event created in RootViewController.mm as below*/

	//EventCustom customEndEvent("game_scene_close_event");
	//_eventDispatcher->dispatchEvent(&customEndEvent);

}
