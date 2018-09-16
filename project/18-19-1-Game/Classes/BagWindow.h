/*
    Description : 背包实现
    Author : 张建东
    Date : 2018.9.5
*/

#ifndef __BAG_WINDOW_H__
#define __BAG_WINDOW_H__

#include "FairyGUI.h"
#include "HallTools.h"
USING_NS_FGUI;

class BagWindow : public Window
{
public:
    CREATE_FUNC(BagWindow);
    static GList* m_listPtrList;
protected:
    //加载、父类虚函数
    virtual void onInit() override;
    virtual void doShowAnimation() override;//打开背包动画设置
    virtual void doHideAnimation() override;//关闭背包动画设置

private:
    void onClickItem(EventContext* context);
    void renderListItem(int index, GObject* obj);


};

#endif