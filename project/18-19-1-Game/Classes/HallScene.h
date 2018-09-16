//HallScene.h
/*
Description:大厅场景头文件
Author:Hanyuu
Date:18/09/04
*/

#ifndef  HALL_SCENE_H
#define HALL_SCENE_H

#include "Hall.h"
#include "SimpleAudioEngine.h"
//#include "cocos2d.h"
//#include "reader/CreatorReader.h"

class HallScene :public cocos2d::Scene,public Hall
{
public:
	HallScene();
	virtual bool init();
	virtual ~HallScene();
	static void stopBackgroundMusic();
	/*
	Function name:createScene
	Description:创建并返回场景
	Created:18/08/24
	Parameter:VOID
	Return:Scene*
	Author:Hanyuu
	*/
	static cocos2d::Scene * createScene();

	/*
	Function name:getInstance
	Description:getInstance
	Created:18/09/13
	Parameter:VOID
	Return:HallScene *
	Author:Hanyuu
	*/
	static HallScene * getInstance();

	//friend class Hall;
	//static Hall  m_hall;
	//void LoadScene();
protected:
	//virtual void continueinit();
private:
	creator::CreatorReader* reader;
	static  CocosDenshion::SimpleAudioEngine* audio;
	static HallScene * instance;
};
#endif // ! HALL_SCENE_H