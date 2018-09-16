//FinshScene.h
/*
Description:结束场景头文件
Author:Hanyuu
Date:18/09/12
*/

/*
//How to call this.
	FinishScene a;
	auto testScene = FinishScene::getInstance()->createScene();
	testScene->retain();
	Director::getInstance()->replaceScene(testScene);

*/

#ifndef FINISH_SCENE_H
#define FINISH_SCENE_H

#include "cocos2d.h"
//#include <string>
#include "reader/CreatorReader.h"
#include "hall.h"
//#include "FairyGUI.h"

//USING_NS_FGUI;

class FinishScene
{
public:
	FinishScene();
	~FinishScene();
	static FinishScene * getInstance();
	cocos2d::Scene * createScene();
	bool init();



protected:
	//virtual void continueInit();

	//GRoot* m_gRoot;

private:
	cocos2d::Scene * m_scene;
	static FinishScene * instance;
	creator::CreatorReader* reader;
	cocos2d::Label * m_nameField[Hall::playerNumber];
	cocos2d::Label * m_GPAField[Hall::playerNumber];
	cocos2d::ui::Button * m_closeButton;

	//void onClose(EventContext* context);
};

#endif
