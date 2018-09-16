#pragma once
/*
    Description : 血槽类(blood bar)
    Author : 张建东(zhang jiandong)
    Date : 2018.9.5
*/
#include "cocos2d.h"
USING_NS_CC;

class BloodBar : public Sprite {
public:
    //加载
    bool init(const char* background, const char* fillname);
    static BloodBar* create(const char* background, const char* fill);
    //填充
    void setFill(ProgressTimer* fill);
    //设置血条
    void setBloodBar(float per);

private:
    ProgressTimer* m_ptimePtrFill;
};
