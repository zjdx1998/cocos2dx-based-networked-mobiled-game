/*
    Description : 血槽类(bloodprogress design)
    Author : 张建东(Zhang Jiandong)
    Date : 2018.9.5
*/

#include "BloodBar.h"
const float eps = 1e-6;
void BloodBar::setBloodBar(float per) 
{ 
    log("the boss % f", per);
    if (per <= 0 + eps) return; //判断血量是否小于0
    if (per + eps > 100) per = 100;
    m_ptimePtrFill->setPercentage(per); 
}
//填充
void BloodBar::setFill(ProgressTimer* fill) 
{ 
    m_ptimePtrFill = fill; 
}
//预加载，血条为100%
bool BloodBar::init(const char* background, const char * fillname) 
{
    this->initWithSpriteFrameName(background);
    ProgressTimer* fill = ProgressTimer::create(Sprite::createWithSpriteFrameName(fillname));
    this->setFill(fill);
    this->addChild(fill);

    fill->setType(ProgressTimer::Type::BAR);//采用ProgressBar类
    fill->setMidpoint(Point(0, 0.5));
    fill->setBarChangeRate(Point(1.0, 0));//设置血条减少比率
    fill->setPosition(this->getContentSize() / 2);
    fill->setPercentage(100);
    return true;
}

BloodBar* BloodBar::create(const char* background, const char* fillname) 
{
    BloodBar* bloodBar = new BloodBar();
    if (bloodBar && bloodBar->init(background, fillname)) 
    {
        bloodBar->autorelease();
        return bloodBar;
    }
    delete bloodBar;
    bloodBar = NULL;
    return NULL;
}