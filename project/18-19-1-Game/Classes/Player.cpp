/*
    Description : 玩家类实现(Player)
    Author : 张建东(Zhang Jiandong)
    Date : 2018-8-21
*/

#include "GameScene.h"
#include "CommonSetting.h"
#include "Player.h"
using namespace std;
using namespace cocos2d;
#define refCSETP(i) &CommonSetting::getInstance()->player[i]

bool Player::getIsScheduing() const
{
    return m_bIsScheduing;
}

void Player::setIsScheduing(const bool& x)
{
    m_bIsScheduing = x;
}

void Player::setDegree(const float& x) { m_fDegree = x; }
float Player::getDegree()const { return m_fDegree; }

bool Player::isAttack()const { return m_bIsAttack; }
void Player::setIsAttack(const bool &x) { m_bIsAttack = x; }

float Player::getLevel()const { return m_fLevel; }
void Player::setLevel(const float &x) { m_fLevel = x; }

void Player::setScore(const float&x) { m_fScore = x; }
float Player::getScore()const { return m_fScore; }

void Player::UpdateAnim()
{

    string prename = "player_move_";
    if (isAttack() && getAnimDone())
    {
        // log("%s", ("mnfyjplayer_attack_" + getNowDirection()).c_str());
        Animate* animate = Animate::create(AnimationCache::getInstance()->getAnimation("player_attack_" + getNowDirection()));
        auto setDoneToTrue = CallFunc::create([this]() {
            
            setAnimDone(true);
            //  log("woc,setIsAttackFalseHere!");
        });
        getPlayer()->stopAllActions();
        auto sequence = Sequence::create(animate, setDoneToTrue, NULL);
        setAnimDone(false);
        setIsAttack(false);
        //log("I am truly using the attack animation!");
        getPlayer()->runAction(sequence);
        if (getNextDirection() == "Stop") return;
    }
    if (getNextDirection() == "Stop")
    {
        setAnimDone(true);
        getPlayer()->stopAllActions();
        return;
    }
    if (getNextDirection() == getNowDirection() && getAnimDone())
    {
        string name = prename + m_strNextDirection;
        Animate* animate = Animate::create(AnimationCache::getInstance()->getAnimation(name));
        auto setDoneToTrue = CallFunc::create([this]() {
            setAnimDone(true);
        });
        auto sequence = Sequence::create(animate, setDoneToTrue, NULL);
        setAnimDone(false);
        getPlayer()->runAction(sequence);
    }
    if (getNextDirection() != getNowDirection())
    {
        string name = prename + m_strNextDirection;
        getPlayer()->stopAllActions();
        // log("Lod %s", name.c_str());
        Animate* animate = Animate::create(AnimationCache::getInstance()->getAnimation(name));
        auto setDoneToTrue = CallFunc::create([this]() {
            setAnimDone(true);
        });
        auto sequence = Sequence::create(animate, setDoneToTrue, NULL);
        setAnimDone(false);
        getPlayer()->runAction(sequence);
        setNowDirection(getNextDirection());

    }
}

Point Player::tileCoordToPosition(cocos2d::TMXTiledMap* tileMap, Vec2 pos)
{
    double x = pos.x / (tileMap->getTileSize().width*tileMap->getScale());
    double y = ((tileMap->getMapSize().height * tileMap->getTileSize().height*tileMap->getScale()) - pos.y*Director::getInstance()->getContentScaleFactor()) /
        tileMap->getTileSize().height / tileMap->getScale();
    return Point(x* Director::getInstance()->getContentScaleFactor(), y);//+Vec2(-1,1);
}

Vec2 positionForTileCoord(TMXTiledMap* tiledMap, const Vec2& tileCoord)
{
    Size mapSize = tiledMap->getMapSize();
    Size tileSize = tiledMap->getTileSize();
    int x = tileCoord.x * tileSize.width + tileSize.width / 2;
    int y = (mapSize.height - tileCoord.y)*tileSize.height - tileSize.height / 2;
    return Vec2(x, y);
}

Vec2 Player::getDestination() const 
{
    return m_pDestination;
}

void Player::setDestination(const Vec2& x)
{
    m_pDestination = x;
}

void Player::Move(int id,Sprite* player, cocos2d::TMXTiledMap* tileMap, cocos2d::TMXLayer* block, TMXLayer* block2)
{
    //log("%f  testhahahahah", Director::getInstance()->getContentScaleFactor());
    //Vec2 test = positionForTileCoord(m_tmxPtrTileMap, Vec2(9, 236));
    //log("test : %f %f", test.x, test.y);
 /*   for (int i = 0; i < 8; i++)
        if (m_strNextDirection == CommonSetting::getInstance()->Direction[i])
        {

        }
        */
    //log("the degree is %f", getDegree());
    float myAngle = getDegree() / 180 * const_pi;
    float offsetX = -10 * getSpeed()*cos(myAngle);
    float offsetY = 10 * getSpeed()*sin(myAngle);
    auto pointDestination = Vec2(offsetX * 6, offsetY * 6) + player->getPosition();
    auto pointTDestination = Vec2(offsetX, offsetY) + player->getPosition();
    //log("location : %f %f",pointDestination.x,pointDestination.y);
    //auto pointDestination = Vec2(directionScale[i].first*m_fSpeed * 6, directionScale[i].second*m_fSpeed * 6) + player->getPosition();
    //auto pointTDestination = Vec2(directionScale[i].first*m_fSpeed, directionScale[i].second*m_fSpeed) + player->getPosition();
    auto tileCoord = tileCoordToPosition(tileMap, pointDestination);
    auto playertileCoord = tileCoordToPosition(tileMap, player->getPosition());
    //  log("%f %f %f %f", player->getPosition().x,m_tmxPtrTileMap->getMapSize().width, m_tmxPtrTileMap->getTileSize().width, m_tmxPtrTileMap->getScale());
    //  log("%f %f ----- %f %f", playertileCoord.x,playertileCoord.y, tileCoord.x, tileCoord.y);
    int tileGid = block->getTileGIDAt(tileCoord);
    if (tileGid)
    {
        auto properties = tileMap->getPropertiesForGID(tileGid).asValueMap();
        if (!properties.empty())
        {
            auto collision = properties["block"].asString();
            if ("true" == collision) return;
        }
    }
    tileGid = block2->getTileGIDAt(tileCoord);
    if (tileGid)
    {
        auto properties = tileMap->getPropertiesForGID(tileGid).asValueMap();
        if (!properties.empty())
        {
            auto collision = properties["block"].asString();
            if ("true" == collision) return;
        }
    }
    for (int i = 1; i <= CSET->getNetworkCnt(); i++)
    {
        if (i == id) continue;
        float disCheck = GameScene::getDis(CSET->player[i].getPlayer()->getPosition(), pointDestination);
     //   log("wokule 150 %d %f %f",i,disCheck,dis_collider);
        if (disCheck < dis_collider) return;
    }
    // log("the now count of closeMonster is %d", closeMonster.size());
    for (int i = 0; i < closeMonster.size(); i++)
    {
        //     log("---%d----%d----", i, closeMonster.size());
        int id = closeMonster.at(i);
        float disCheck = GameScene::getDis(CSET->monster[id]->getMonster()->getPosition(), pointDestination);
        //  log("the checkDis is %f", disCheck);
       // log("wokule 160  %d %f %f",i,disCheck,dis_collider);
        if (disCheck < dis_collider) return;
        else if (disCheck > dis_distant * 5)
        {
            closeMonster.erase(closeMonster.begin() + i);
            i--;
        }
        //   log("---%d----%d----", i, closeMonster.size());
    }
    //if (CSET->getNetworkID() != CSETN) pointTDestination += Vec2(offsetX*3, offsetY*3);
    auto moveby = MoveTo::create(0.1, pointTDestination);
    player->runAction(moveby);
   // log("not the player %d 's position is -----%f %f-----",CSET->getNetworkID(),player->getPosition().x,player->getPosition().y);
}

bool Player::isDie() const{
    return (getHP() < 0 + eps);
}

void Player::setPlayer(Sprite* x) { m_spPtrPlayer = x; }

Sprite* Player::getPlayer() { return m_spPtrPlayer; }

void Player::CreatePlayer(int id)
{
    auto player_texture = Director::getInstance()->getTextureCache()->addImage("player_move.png");
    CommonSetting::getInstance()->player[id].setPlayer(Sprite::createWithTexture(player_texture, CC_RECT_PIXELS_TO_POINTS(Rect(0, 0, 128, 128))));
    // CommonSetting::getInstance()->setNetworkID(id);
}

float Player::getMaxHP()const {
    return 100 + getLevel() * 100;
} 

void Player::PlayerDie() {
    auto die_born_animation = AnimationCache::getInstance()->getAnimation("dieborn");
    die_born_animation->setDelayPerUnit(getLevel()+0.1);
    die_born_animation->setLoops(-1);
    auto myfadeOut = FadeOut::create(0.3f);
    auto myfadeIn = FadeIn::create(0.3f);
    Animate* animate = Animate::create(die_born_animation);
    getPlayer()->stopAllActions();
    auto mySpawn = Spawn::create(myfadeOut, animate, myfadeIn, nullptr);
    getPlayer()->runAction(mySpawn);
}

Player::Player()
{
    //预加载plist
    setDamage(10);
    setHP(100);
    setLevel(0);
    setSpeed(0.3);
    setName("SEU");
    setNextDirection("Stop");
    setNowDirection("West");
    setAnimDone(true);
    setIsAttack(false);
    setDegree(0);
    setIsScheduing(false);
    while (!closeMonster.empty())  closeMonster.erase(closeMonster.begin());
}

void Player::addBasicInfo()
{
    float scaleXY = Director::getInstance()->getContentScaleFactor();
    m_bloodProgress = BloodBar::create("bg.png", "blood.png");
    m_bloodProgress->setScale(0.15);
    m_bloodProgress->setPosition(64 / scaleXY, 110 / scaleXY);
    m_bloodProgress->setBloodBar(100);
    getPlayer()->addChild(m_bloodProgress);
    stringstream os; os << getLevel();
    m_proShow = LabelTTF::create(m_strName + " GPA : " + os.str(), "fonts/Marker Felt", 20);
    m_proShow->setPosition(64 / scaleXY, 140 / scaleXY);
    getPlayer()->addChild(m_proShow);
}

void Player::LevelUpAnim() {
    if (getScore() < ExpLevel[(int)(getLevel() * 10)]) return;
    auto level_up_animation = AnimationCache::getInstance()->getAnimation("levelup");
    level_up_animation->setDelayPerUnit(0.1);   
    auto myfadeOut = FadeOut::create(0.3f);
    auto myfadeIn = FadeIn::create(0.3f);
    Animate* animate = Animate::create(level_up_animation);
    getPlayer()->stopAllActions();
    auto mySpawn = Spawn::create(myfadeOut, animate, myfadeIn, nullptr);
    getPlayer()->runAction(mySpawn);
    setLevel(getLevel() + 0.1);
}

//速度
float Player::getSpeed() const { return m_fSpeed; }
void Player::setSpeed(const float& x) { m_fSpeed = x; }
//血量
float Player::getHP() const { return m_fHP; }
void Player::setHP(const float& x) { m_fHP = x; }
//伤害
float Player::getDamage() const { return m_fDamage + getLevel()*10; }
void Player::setDamage(const float& x) { m_fDamage = x; }
//名称
string Player::getName() const { return m_strName; }
void Player::setName(const string& x) { m_strName = x; }
//下一步方向
string Player::getNextDirection() const { return m_strNextDirection; }
void Player::setNextDirection(const string& x) { m_strNextDirection = x; }
//当前方向
string Player::getNowDirection() const { return m_strNowDirection; }
void Player::setNowDirection(const string& x) { m_strNowDirection = x; }
bool Player::getAnimDone()const { return m_bIsAnimDone; }
void Player::setAnimDone(const bool& x) { m_bIsAnimDone = x; }

void Player::setBulletType(const string& str) { bulletType = str; }
string Player::getBulletType() const { return bulletType; }