#ifndef __DEMO_SCENE_H__
#define __DEMO_SCENE_H__

#include "cocos2d.h"
#include "FairyGUI.h"

USING_NS_FGUI;

class SceneTemplate : public cocos2d::Scene
{
public:
    bool init();//¼ÓÔØ

    SceneTemplate();//³¡¾°Ä£°å
    virtual ~SceneTemplate();

protected:
    virtual void continueInit();

    GRoot* m_gRoot;

private:
    void onClose(EventContext* context);
};

#endif
