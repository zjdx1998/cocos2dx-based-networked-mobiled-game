#include "item.h"
#include "cocos2d.h"
#include "gun.h"
#include "consum.h"
#include "armor.h"
#include "CommonSetting.h"
#include <vector>
using namespace cocos2d;
using namespace std;
item::item()
    :item_Id(0), item_Name(""), item_Type(0), item_Icon(""), item_Introduction(""), item_Level(1), item_PlayerId(0)
{
}

item::item(int id, const string &name, int type, const string &icon, const string &intro, int level,int player)
	:item_Id(id),item_Name(name),item_Type(type),item_Icon(icon),item_Introduction(intro),item_Level(level),item_PlayerId(player)
{
}

item::item(item &x): item_Id(x.item_Id), item_Name(x.item_Name), item_Type(x.item_Type), item_Icon(x.item_Icon),
item_Introduction(x.item_Introduction), item_Level(x.item_Level), item_PlayerId(x.item_PlayerId)
{
}


item::~item()
{
}

int item::getitem_Id() const
{
	return item_Id;
}

int item::getitem_Type() const
{
	return item_Type;
}

string item::getitem_Name() const
{
	return item_Name;
}

string item::getitem_Icon() const
{
	return item_Icon;
}

string item::getitem_Introduction() const
{
	return item_Introduction;
}

int item::getitem_Level() const
{
	return item_Level;
}

int item::getitem_PlayerId() const
{
	return item_PlayerId;
}

void item::setitem_PlayerId(int player)
{
	item_PlayerId = player;
}

bool item::operator>(item &x)
{
	if (this->getitem_Id() > x.getitem_Id()) {
		return true;
	}
	return false;
}

bool item::operator=(item &x)
{
	if (this->getitem_Id() == x.getitem_Id()) {
		return true;
	}
	return false;
}

bool item::operator!=(item &x)
{
	return !(*this = x);
}

void item::init()
{
	auto item_Sprite = Sprite::create(getitem_Icon());
}

void item::setitem(vector<item *> &itemList, int id)
{
	item *myItem = itemList.at(id);
	CommonSetting *comSet = CommonSetting::getInstance();
	auto item_texture = Director::getInstance()->getTextureCache()->addImage(myItem->item_Icon);
	CommonSetting::getInstance()->player[id].setPlayer(Sprite::createWithTexture(item_texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 128, 128))));
	auto mySprite = myItem->getitemIcon();
	mySprite->setPosition(9000, 880);
}
vector<item *> item::inititemList()
{
	vector<item *> itemList;
	return itemList;
}

string item::pickup(int playerid)
{
	setitem_PlayerId(playerid);
	return item_Icon;
}

void item::setitemIcon(Sprite *x)
{

	itemIcon = x;
}

Sprite * item::getitemIcon()
{
	return itemIcon;
}

void item::creatItem(int x)
{
	string iconpath = getitem_Icon();
	auto itemicon_texture = Director::getInstance()->getTextureCache()->addImage(iconpath);
	CommonSetting::getInstance()->item[x]->setitemIcon(Sprite::createWithTexture(itemicon_texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 24, 24))));
	CommonSetting::getInstance()->item[x]->getitemIcon()->setScale(2);
}

Node* item::getPickUpMenu(int iid,int pid)
{
	
	auto pickUpMenu = Node::create();
	auto itemicon_texture = Director::getInstance()->getTextureCache()->addImage(CommonSetting::getInstance()->item[iid]->getitem_Icon());
	auto itemicon_texture2 = Director::getInstance()->getTextureCache()->addImage("pickUpBack.png");
	//setitemIcon(Sprite::createWithTexture(itemicon_texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 128, 128))));
	auto itemName = Label::create(item_Name,"Arial",20);
	itemName->setColor(Color3B(0, 0, 0));
	auto itemIntro = Label::create(item_Introduction, "Arial", 20);
	itemIntro->setColor(Color3B(0, 0, 0));
	auto itemMenuIcon = Sprite::createWithTexture(itemicon_texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 24, 24)));
	itemMenuIcon->setScale(4);
	auto itemMenuBack = Sprite::createWithTexture(itemicon_texture2, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 480, 128)));
//	itemMenuBack->setScaleX(0.5);


	pickUpMenu->addChild(itemMenuIcon, -100);
	pickUpMenu->addChild(itemName, -100);
	pickUpMenu->addChild(itemIntro, -100);
	pickUpMenu->addChild(itemMenuBack, -101,10);
	itemMenuIcon->setPosition(-70, 0);
	itemName->setPosition(70, 20);
	itemIntro->setPosition(70, -20);
	itemMenuBack->setPosition(130, 0);

	
	auto listener1 = EventListenerTouchOneByOne::create();
	listener1->setSwallowTouches(true);
	listener1->onTouchBegan = [iid, pid](Touch* touch, Event* event)->bool {


		auto target = event->getCurrentTarget();
		//auto scaleFactor = Director::getInstance()->getContentScaleFactor();
		Vec2 locationInNode = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		if (rect.containsPoint(locationInNode))
		{
			if (Hall::getInstance()->isHost())
			{
				BroadcastServer::getInstance()->addHostCollectItem(iid);
			}
			else
			{
				BroadcastClient::getInstance()->addCollectItem(iid);

			}
			CommonSetting::getInstance()->item[iid]->setitem_PlayerId(pid);
			//if (CommonSetting::getInstance()->item[iid]->getitemIcon()->getParent() == nullptr) {
			//	return 0;
			//}
			CommonSetting::getInstance()->item[iid]->getitemIcon()->getParent()->removeFromParent();
			//int logsize = CSET->player[pid].iHashID.size();
			//log("%d--------------", logsize);
			auto myRecord = CommonSetting::getInstance()->player[pid].iHashID.at(0);
			myRecord.first = CSET->player[pid].iHashID.size();
			myRecord.second = iid;
			CommonSetting::getInstance()->player[pid].iHashID.push_back(myRecord);
			//CommonSetting::getInstance()->player[pid].getItemPack().push_back(CommonSetting::getInstance()->item[iid]);
			target->getParent()->removeFromParent();
			
			auto myObj = BagWindow::m_listPtrList->getChildAt(CSET->player[pid].iHashID.size()-2); 

			myObj->setIcon(CSET->item[iid]->getitem_Icon());
			
			myObj->setText("1");
			//myObj->setHeight(myRecord.first*0.1+95);
			myObj->name = int2String(myRecord.first);
			return true;
		}
		return false;
	};
	auto _eventDispatcher = Director::getInstance()->getEventDispatcher();
	//_eventDispatcher->addEventListenerWithFixedPriority(listener1, 10000);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener1, itemMenuBack);
	

	return pickUpMenu;
}

void item::useItem()
{
}

  