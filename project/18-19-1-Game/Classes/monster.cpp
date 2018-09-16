/*
    Description:����ϵͳ
    Author : �Ž���
    Date : 2018/8/24
*/
#include "CommonSetting.h"
#include "cocos2d.h"
#include "monster.h"
#include "GameScene.h"

void Monster::addBasicInfo() 
{

    float scaleXY = Director::getInstance()->getContentScaleFactor();
    m_bloodProgress = BloodBar::create("bg.png", "blood.png");
    m_bloodProgress->setScale(0.1);
    m_bloodProgress->setPosition(64/scaleXY, 110/scaleXY);
    m_bloodProgress->setBloodBar(100);
    getMonster()->addChild(m_bloodProgress);
    setName(getKind() ? "Zhizhu" : "PipiXia");
    if (getKind() == 3) setName("Dragon");
    m_proShow = LabelTTF::create(m_strName , "fonts/Marker Felt", 20);
    m_proShow->setPosition(64/scaleXY, 140/scaleXY);
    getMonster()->addChild(m_proShow);
}

float Monster::getDamage()const 
{
    float MaxLevel = 0;
    for (int i = 1; i <= CSET->getNetworkCnt(); i++) 
        MaxLevel = max(MaxLevel, CSET->player[i].getLevel());
    return m_fDamage + MaxLevel * 5;
}

void Monster::createMonster() 
{
    auto monster1_texture = Director::getInstance()->getTextureCache()->addImage("monster1_move.png");
    auto monster2_texture = Director::getInstance()->getTextureCache()->addImage("monster2_move.png");
    auto boss_texture = Director::getInstance()->getTextureCache()->addImage("boss_move.png");
}

void Monster::setKind(const int &x) { iKind = x; }
int Monster::getKind()const { return iKind; }

Monster::Monster(int id) :Player() 
{
    setDamage(8);
    auto dirIns = Director::getInstance();
    if (id%2)
    {
        setMonster(Sprite::createWithTexture(dirIns->getTextureCache()
            ->getTextureForKey("monster1_move.png")
            , CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 128, 128)))
        );
        setKind(1);
        getMonster()->setAnchorPoint(Vec2(0.5, 0.2));
        setSpeed(0.6);
        setDamage(10);
    }
    else 
    {
        setMonster(Sprite::createWithTexture(dirIns->getTextureCache()
            ->getTextureForKey("monster2_move.png")
            , CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 128, 128)))
        );
        setKind(0);
        getMonster()->setAnchorPoint(Vec2(0.5, 0.5));
        setSpeed(0.3);
        setDamage(20);
    }
}

Sprite* Monster::getMonster() 
{
    return m_spPtrMonster;
}
void Monster::setMonster(Sprite* x) 
{
    m_spPtrMonster = x;
}

float Monster::getMaxHP()const
{
    if(getKind()!=3) return 100;
    return 3000;
}


void Monster::UpdateAnim() 
{
    stringstream t_tostring;  t_tostring<< (!getKind() + 1);
    string prename = "monster" + t_tostring.str() + "_move_";
    if (getNextDirection() == "Stop")
    {
        setAnimDone(true);
        m_spPtrMonster->stopAllActions();
        return;
    }
    if (isAttack()) 
    {
        m_spPtrMonster->stopAllActions();
        string name = "monster1_attack_" + getNextDirection();
        Animate * animate = Animate::create(AnimationCache::getInstance()->getAnimation(name));
        auto setDoneToTrue = CallFunc::create([this]() {
            setAnimDone(true);
        });
        auto sequence = Sequence::create(animate, setDoneToTrue, NULL);
        m_spPtrMonster->runAction(sequence);
        setAnimDone(false);
        return;
    }
    if (getNextDirection() == getNowDirection() && getAnimDone()) 
    {
        string name = prename + getNextDirection();
        Animate* animate = Animate::create(AnimationCache::getInstance()->getAnimation(name));
        auto setDoneToTrue = CallFunc::create([this]() {
            setAnimDone(true);
        });
        auto sequence = Sequence::create(animate, setDoneToTrue, NULL);
        m_spPtrMonster->runAction(sequence);
        setAnimDone(false);
    }
    if (getNextDirection() != getNowDirection()) 
    {
        string name = prename + getNextDirection();
        m_spPtrMonster->stopAllActions();
        Animate* animate = Animate::create(AnimationCache::getInstance()->getAnimation(name));
        auto setDoneToTrue = CallFunc::create([this]() 
        {
            setAnimDone(true);
        });
        auto sequence = Sequence::create(animate, setDoneToTrue, NULL);

        m_spPtrMonster->runAction(sequence);
        setNowDirection(getNextDirection());
        setAnimDone(false);
    }
}

Monster::Monster():Player()
{

}

Boss::Boss()
{
    auto dirIns = Director::getInstance();
    setName("Dragon");
    setSpeed(1.0f);
    setDamage(50);
    setMonster(Sprite::createWithTexture(dirIns->getTextureCache()
        ->getTextureForKey("boss_move.png")
        , CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 176, 176))));
    getMonster()->setAnchorPoint(Vec2(0.5, 0.2));
    setHP(3000);
    setLevel(3.0);
    setNextDirection("Stop");
    setNowDirection("West");
    setAnimDone(true);
    setIsAttack(false);
    setDegree(0);
    setKind(3);
}

void Boss::UpdateAnim()
{
    string prename = "boss_move_";
    if (getNextDirection() == "Stop")
    {
        setAnimDone(true);
        m_spPtrMonster->stopAllActions();
        return;
    }
    if (isAttack())
    {
        m_spPtrMonster->stopAllActions();
        string name = "boss_attack_" + getNextDirection();
        Animate * animate = Animate::create(AnimationCache::getInstance()->getAnimation(name));
        auto setDoneToTrue = CallFunc::create([this]() {
            setAnimDone(true);
        });
        auto sequence = Sequence::create(animate, setDoneToTrue, NULL);
        m_spPtrMonster->runAction(sequence);
        setAnimDone(false);
        return;
    }
    if (getNextDirection() == getNowDirection() && getAnimDone())
    {
        string name = prename + getNextDirection();
        Animate* animate = Animate::create(AnimationCache::getInstance()->getAnimation(name));
        auto setDoneToTrue = CallFunc::create([this]() {
            setAnimDone(true);
        });
        auto sequence = Sequence::create(animate, setDoneToTrue, NULL);
        m_spPtrMonster->runAction(sequence);
        setAnimDone(false);
    }
    if (getNextDirection() != getNowDirection())
    {
        string name = prename + getNextDirection();
        m_spPtrMonster->stopAllActions();
        Animate* animate = Animate::create(AnimationCache::getInstance()->getAnimation(name));
        auto setDoneToTrue = CallFunc::create([this]()
        {
            setAnimDone(true);
        });
        auto sequence = Sequence::create(animate, setDoneToTrue, NULL);

        m_spPtrMonster->runAction(sequence);
        setNowDirection(getNextDirection());
        setAnimDone(false);
    }
}