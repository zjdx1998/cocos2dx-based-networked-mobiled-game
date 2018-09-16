/*
    Description : 背包实现 (bag)
    Author : 张建东(Zhang Jiandong)
    Date : 2018.9.5
*/

#include "BagWindow.h"
#include "CommonSetting.h"

GList* BagWindow::m_listPtrList = nullptr;

void BagWindow::onInit()
{
    //从摇杆模块读取背包
    setContentPane(UIPackage::createObject("Joystick", "BagWin")->as<GComponent>());
    //center();
    setModal(true);//设置背包跟随
    //预加载背包框
    m_listPtrList = _contentPane->getChild("list")->as<GList>();
    m_listPtrList->addEventListener(UIEventType::ClickItem, CC_CALLBACK_1(BagWindow::onClickItem, this));
    m_listPtrList->itemRenderer = CC_CALLBACK_2(BagWindow::renderListItem, this);
    m_listPtrList->setNumItems(45);

}

void BagWindow::renderListItem(int index, GObject* obj)
{
    //obj->setIcon("icons/i" + Value((int)(rand_0_1() * 10)).asString() + ".png");
    //obj->setText(Value((int)(rand_0_1() * 100)).asString());
}

void BagWindow::doShowAnimation()
{
    setScale(0.1f, 0.1f);
    setPivot(0.5f, 0.5f);

    GTween::to(getScale(), Vec2::ONE, 0.3f)->setTarget(this, TweenPropType::Scale)->onComplete(CC_CALLBACK_0(BagWindow::onShown, this));
}

void BagWindow::doHideAnimation()
{
    GTween::to(getScale(), Vec2(0.1f, 0.1f), 0.3f)->setTarget(this, TweenPropType::Scale)->onComplete(CC_CALLBACK_0(BagWindow::hideImmediately, this));
}
//使用物品
void BagWindow::onClickItem(EventContext* context)
{
	GObject* item = (GObject*)context->getData();
	_contentPane->getChild("n11")->setIcon(item->getIcon());
	_contentPane->getChild("n13")->setText(item->getText());
	
	if (item->name == "") {
		return;
	}
	int itemp = string2Int(item->name);
    if (itemp > 0 && itemp < 45) {
        auto myRecord = CSET->player[CSETN].iHashID.at(itemp);
        CSET->item[myRecord.second]->useItem();

        CSET->player[CSETN].iHashID.erase(CSET->player[CSETN].iHashID.begin() + myRecord.first);
        for (int i = 0; i < CSET->player[CSETN].iHashID.size(); ++i) {
            CSET->player[CSETN].iHashID[i].first = i;
        }
        for (int i = 1; i < CSET->player[CSETN].iHashID.size(); ++i) {
            auto myObj = BagWindow::m_listPtrList->getChildAt(i - 1);

            myObj->setIcon(CSET->item[CSET->player[CSETN].iHashID[i].second]->getitem_Icon());

            myObj->setText("1");
            myObj->name = int2String(i);
        }
        auto myObj = BagWindow::m_listPtrList->getChildAt(CSET->player[CSETN].iHashID.size() - 1);

        myObj->setIcon("space.png");
        myObj->setText("");
        myObj->name = int2String(0);
    }
}