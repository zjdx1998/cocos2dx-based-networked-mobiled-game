/*
    Description : 摇杆类设计(joystick design)
    Author: 张建东(Zhang Jiandong)
    Date : 2018/8/21
*/

#ifndef __JOYSTICK_MODULE_H__
#define __JOYSTICK_MODULE_H__

#include "cocos2d.h"
#include "FairyGUI.h"

USING_NS_FGUI;

class YGMoudle : public UIEventDispatcher
{
public:
    static const int MOVE = 100;
    static const int END = 101;

    static YGMoudle* create(GComponent* mainView);

private:
    bool init(GComponent* mainView);
    //触摸开始
    void onTouchBegin(EventContext* context);
    //移动摇杆
    void onTouchMove(EventContext* context);
    //移动结束
    void onTouchEnd(EventContext* context);

    float m_fInitX;//初始x
    float m_fInitY;//初始y
    float m_fstartStageX;//起始圆盘x
    float m_fstartStageY;//起始圆盘y
    float m_flastStageX;//结束X
    float m_flastStageY;//结束y
    int m_iRadius;//半径

    GButton* m_btnPtrButton;//按钮
    GObject* m_objPtrTouchArea;//触碰区
    GObject* m_objPtrThumb;//手指移动
    GObject* m_objPtrCenter;//显示中心
    GTweener* m_PtrTweener;//UI中间件
    int touchId;
};

#endif
