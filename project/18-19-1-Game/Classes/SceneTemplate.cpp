/*
    Description : 模板界面,用于GUI管理(template scene manager)
    Author : 张建东(Zhang Jiandong)
    Date : 2018/8/20
*/
#include "SceneTemplate.h"
//#include "MenuScene.h"

USING_NS_CC;

bool SceneTemplate::init()
{
    if (!Scene::init())
    {
        return false;
    }

    m_gRoot = GRoot::create(this);
    m_gRoot->retain();

    continueInit();

    return true;
}

void SceneTemplate::continueInit()
{
}

void SceneTemplate::onClose(EventContext* context)
{
    Director::getInstance()->end();
}

SceneTemplate::SceneTemplate() :
    m_gRoot(nullptr)
{
}

SceneTemplate::~SceneTemplate()
{
    CC_SAFE_RELEASE(m_gRoot);
}
