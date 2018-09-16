/*
    Description ： 游戏场景设置(Game Scene)
    Author : 张建东（Zhang Jiandong)
    Date : 2018-8-26
*/
#include "CommonSetting.h"
#include "Player.h"
#include "GameScene.h"
#include "hall.h"
USING_NS_CC;

void push_to_server(Player& p);
void push_to_client(Player& p);
mutex gs_mu;
CocosDenshion::SimpleAudioEngine* GameScene::audio;

GameScene::~GameScene()
{
    CC_SAFE_RELEASE(m_clsPtrUI);
}

float GameScene::getDis(Vec2 A, Vec2 B) 
{
    return sqrt((A.x - B.x)*(A.x - B.x) + (A.y - B.y)*(A.y - B.y));
}

float getDegree(Vec2 A, Vec2 B) 
{
    auto deltaY = B.y - A.y;
    auto deltaX = B.x - A.x;
    auto tangle = atan(abs(deltaY) / abs(deltaX));
    if (deltaY >= 0 && deltaX <= 0) return tangle;
    if (deltaY >= 0 && deltaX >= 0) return acos(-1) - tangle;
    if (deltaY <= 0 && deltaX >= 0) return acos(-1) + tangle;
    return 2 * acos(-1) - tangle;
}

void GameScene::moveMonster(float dt)
{
    if (Hall::getInstance()->isHost())
    {
        //	cocos2d::log("GameScene::update add %d monsters", CSETP.closeMonster.size());
        for (auto i : CSETP.closeMonster)
        {
            BroadcastServer::getInstance()->addSceneMonster(*(CSET->monster[i]), i);
        }
    }
    else
    {
        for (auto i : CSETP.closeMonster)
        {
            BroadcastClient::getInstance()->addSceneMonster(*(CSET->monster[i]), i);
        }
    }

    int sizeMonster = CSET->mHashId.size();
    Vec2 posMonster;
    // log("moveMonster test  %d",sizeMonster);
    for (int i = 0; i < sizeMonster; i++)
    {
        int id = CSET->mHashId.at(i);
        //   log("moveMonster test2");
        if (CSET->monster[id] == nullptr) continue;
        if (CSET->monster[id]->getHP() < 0 + 1e-6) continue;
        posMonster = CSET->monster[id]->getMonster()->getPosition();
        pair<int, double> mindisRecord(0, DBL_MAX);
        for (int j = 1; j <= CSET->getNetworkCnt(); j++)
        {
            if (CSET->player[j].isDie()) continue;
            double dis = getDis(posMonster, CSET->player[j].getPlayer()->getPosition());
            if (dis < mindisRecord.second) mindisRecord = make_pair(j, dis);
        }
        if (!mindisRecord.first) return;
        //   log("%d ----  -- - - -   %f --- - -- - -  %f",i, mindisRecord.second, 
             //  dis_close + dis_distant * (CSET->monster[id]->getKind()));
        if (/*(!CSET->monster[id]->getKind()) &&
            (mindisRecord.second <
                dis_close + dis_distant)
            ||*/
            mindisRecord.second > dis_distant * 3)
        {
            CSET->monster[id]->setNextDirection("Stop");
            continue;
        }
        auto tMonster = CSET->monster[id]->getMonster();
        auto degree = getDegree(tMonster->getPosition(),
            CSET->player[mindisRecord.first].getPlayer()->getPosition());        
        degree = degree / const_pi * 180;
        CSET->monster[id]->setDegree(degree);
        CSET->monster[id]->setNextDirection(CSET->Direction[((int)(degree + 22.5) % 360) / 45]);

        vector<int>::iterator findResult = find(
            CSET->player[mindisRecord.first].closeMonster.begin(),
            CSET->player[mindisRecord.first].closeMonster.end(),
            id
        );
        if (findResult == CSET->player[mindisRecord.first].closeMonster.end())
            CSET->player[mindisRecord.first].closeMonster.push_back(id);
        if (CSET->monster[id]->getKind() && mindisRecord.second < dis_close)
        {
            CSET->monster[id]->setIsAttack(true);
            CSET->monster[id]->UpdateAnim();
            CSET->player[mindisRecord.first].setHP(CSET->player[mindisRecord.first].getHP() - CSET->monster[id]->getDamage());
            continue;
        }
        if ((!CSET->monster[id]->getKind()||CSET->monster[id]->getKind()==3) && mindisRecord.second < dis_close + dis_distant) {
            bulletLayer->startMonsterAttack(id,mindisRecord.first);
            CSET->monster[id]->setIsAttack(true);
            if(CSET->monster[id]->getKind()==3) CSET->monster[id]->UpdateAnim();
            continue;
        }
        bulletLayer->stopMonsterAttack(id, mindisRecord.first);
        CSET->monster[id]->setIsAttack(false);
        CSET->monster[id]->UpdateAnim();
        CSET->monster[id]->Move(32767,tMonster, m_tmxPtrTileMap, m_tmxLayerPtrBlock1, m_tmxLayerPtrBlock2);
        /*            if (getDis(posMonster, CSET->player[j].getPlayer()->getPosition())
                        <
                        dis_close + dis_distant * (!CSET->monster[id]->getKind())){
                            CSET->monster[id]->Move(CSET->player[j].getPlayer()->getPosition(),
                                                    m_tmxPtrTileMap,inaccessible1,inaccessible2);
               //             log("%d , %f , %f", id, posMonster.x, posMonster.y);
                            CSET->monster[id]->UpdateAnim();
                    }
        */
    }


}

void GameScene::addMonster()
{
    int cnt = 0;
    cocos2d::TMXObjectGroup *enemyObjGroup = m_tmxPtrTileMap->getObjectGroup("enemy_group");
    double screenScale = Director::getInstance()->getContentScaleFactor();
    log(" the total amount is %d ", enemyObjGroup->getObjects().size());
    for (auto&enemy : enemyObjGroup->getObjects())
    {
        ValueMap& dict = enemy.asValueMap();
        if (dict["boss"].asInt() == 1) 
        {
            CSET->monster[++cnt] = new Boss();
            int x = dict["x"].asInt();
            int y = dict["y"].asInt();

            double px = (double)x * m_tmxPtrTileMap->getScale();
            double py = (double)y * m_tmxPtrTileMap->getScale();
            CSET->monster[cnt]->getMonster()->setPosition(px, py);
            this->addChild(CSET->monster[cnt]->getMonster(), -10);
            CSET->monster[cnt]->addBasicInfo();
            CSET->mHashId.push_back(cnt);
        }
        else
        if (dict["enemy"].asInt() == 1) 
        {
            CommonSetting::getInstance()->monster[++cnt] = new Monster(cnt);
            int x = dict["x"].asInt();
            int y = dict["y"].asInt();
            double px = (double)x * m_tmxPtrTileMap->getScale();
            double py = (double)y * m_tmxPtrTileMap->getScale();
            //   log("%d %d %d ----> %f %f", dict["id"].asInt() ,x, y, px, py);
            CSET->monster[cnt]->getMonster()->setPosition(px, py);
            this->addChild(CSET->monster[cnt]->getMonster(), -10);
            CSET->monster[cnt]->addBasicInfo();
            CSET->mHashId.push_back(cnt);
          //  CSET->monster[cnt]->getMonster()->setScale(2);
            //log("woc monster here %d", cnt);
        }
    }
}

//把窗口的视角设置到人物身上
void GameScene::setViewPointCenter(cocos2d::Point position)
{
    //求出屏幕的范围包括宽和高
    auto winSize = Director::getInstance()->getWinSize();

    //显示屏幕中心点的坐标大于屏幕宽和高的一半,防止部分视图在屏幕之外，即屏幕移出地图边界下/左方
    int x = MAX(position.x, winSize.width / 2);
    int y = MAX(position.y, winSize.height / 2);

    //显示屏幕中心的坐标小于边界减去屏幕宽和高的一半，防止部分视图在屏幕之外，即屏幕超出地图边界上/右方

    //整个瓦片地图的宽
    //getMapSize().width瓦片地图横向有多少个瓦片
    //getTileSize().width每一个瓦片的宽度
    int mapWidth = m_tmxPtrTileMap->getMapSize().width * m_tmxPtrTileMap->getTileSize().width * m_tmxPtrTileMap->getScale() / Director::getInstance()->getContentScaleFactor();
    x = MIN(x, mapWidth - winSize.width / 2);
    //整个瓦片地图的高
    int mapHeight = m_tmxPtrTileMap->getMapSize().height * m_tmxPtrTileMap->getTileSize().height * m_tmxPtrTileMap->getScale() / Director::getInstance()->getContentScaleFactor();
    y = MIN(y, mapHeight - winSize.height / 2);
    auto actualPosition = Point(x, y);

    auto centerOfView = Point(winSize.width / 2, winSize.height / 2);
    auto viewPoint = centerOfView - actualPosition;
    //重置显示屏幕的中心
    this->setPosition(viewPoint);
    //log("viewPoint : (%f , %f)  Position : (%f,%f)",viewPoint.x,viewPoint.y,position.x,position.y);
    winSize = Director::getInstance()->getWinSize();
    x = winSize.width / 2, y = winSize.height / 2;
    if (nullptr != m_gComPtrView)  m_gComPtrView->setPosition(-Director::getInstance()->getRunningScene()->getPosition().x,
        Director::getInstance()->getRunningScene()->getPosition().y);
    if (nullptr != m_winPtrBagWin) m_winPtrBagWin->setPosition(-Director::getInstance()->getRunningScene()->getPosition().x,
        Director::getInstance()->getRunningScene()->getPosition().y);
    //  if(nullptr!=m_gComPtrView) log("m_gComPtrView: (%f,%f)", m_gComPtrView->getPosition().x, m_gComPtrView->getPosition().y);
   // log("first window:: %f , %f", Director::getInstance()->getRunningScene()->getPosition().x,
      //  Director::getInstance()->getRunningScene()->getPosition().y);
}

void GameScene::solveUIRender()
{
    UIPackage::addPackage("UI/Joystick");
    UIConfig::horizontalScrollBar = "";
    UIConfig::verticalScrollBar = "";

    m_gComPtrView = UIPackage::createObject("Joystick", "Main")->as<GComponent>();
    m_gRoot->addChild(m_gComPtrView);
    //_groot->addChildAt(m_gComPtrView, 1000);
    m_clsPtrUI = YGMoudle::create(m_gComPtrView);
    m_clsPtrUI->retain();
    //   log("%f %f", _groot->getPosition().x, _groot->getPosition().y);
    m_clsPtrUI->addEventListener(YGMoudle::MOVE, [this](EventContext*context)
    {
        auto degree = context->getDataValue().asFloat() + 180.0f;
        auto comSet = CommonSetting::getInstance();
        CSETP.setDegree(degree);
        if (CSETP.isDie()) return;
        float dis = 22.5f, f_range = 45.0f;
        if (degree > 360 - dis || degree < dis) //除了第一个角度特判
            comSet->player[CSETN].setNextDirection(comSet->Direction[0]);
        //tf->setText(comSet->Direction[0] + to_string(degree));
        else
            for (int i = 1; i < 8; i++, f_range += 45.0f)//其他所有控制误差判断
                if (f_range - dis < degree && degree < f_range + dis)
                    //tf->setText(comSet->Direction[i] + to_string(degree));
                    comSet->player[comSet->getNetworkID()].setNextDirection(comSet->Direction[i]);
    });

    m_clsPtrUI->addEventListener(YGMoudle::END, [](EventContext*context)
    {
        //  tf->setText("");
        auto comSet = CommonSetting::getInstance();
        //   tf->setText(comSet->player[1].getNextDirection());
        comSet->player[comSet->getNetworkID()].setNextDirection("Stop");
    });

    m_winPtrBagWin = BagWindow::create();
    m_winPtrBagWin->retain();
    m_gComPtrView->getChild("bagBtn")->addClickListener([this](EventContext*) { 
        if (m_winPtrBagWin->isShowing()) m_winPtrBagWin->hide();
        else
            m_winPtrBagWin->show(); 
    });

    auto attackBtn = m_gComPtrView->getChild("n10");
    attackBtn->addClickListener([this](EventContext*) {
        if(!CSETP.isDie()) CSETP.setIsAttack(true);
    });
}

void GameScene::testBag(float dt) {
    m_winPtrBagWin->show();
    m_winPtrBagWin->hide();
}

void GameScene::setDietoBorn(float dt) {
    for(int i=1;i<=4;i++)
        if (CSET->player[i].isDie()) {
            CSET->player[i].setHP(CSET->player[i].getMaxHP());
            CSET->player[i].setIsScheduing(false);
            //log("test here %d", i);
        }
}

/*
预加载动画
*/
void GameScene::preAnim()
{
    CCSpriteFrameCache::sharedSpriteFrameCache()->addSpriteFramesWithFile("effects.plist");

    CCAnimation *animation = CCAnimation::create();

    for (int i = 2; i <= 21; i++)
    {

        CCString prename = "dieborn (";
        prename.appendWithFormat("%d", i);
        prename.append(").png");
        animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(prename.getCString()));
        log("%s", prename.getCString());
    }
    animation->setDelayPerUnit(0.2);
    AnimationCache::getInstance()->addAnimation(animation, "dieborn");
    CCAnimation *another_animation = CCAnimation::create();
    for (int i = 1; i <= 16; i++)
    {

        CCString prename = "levelup (";
        prename.appendWithFormat("%d", i);
        prename.append(").png");
        another_animation->addSpriteFrame(CCSpriteFrameCache::sharedSpriteFrameCache()->spriteFrameByName(prename.getCString()));
        log("%s", prename.getCString());
    }
    another_animation->setDelayPerUnit(0.2);
    AnimationCache::getInstance()->addAnimation(another_animation, "levelup");
    //创建Player攻击动画
    auto player_attack_texture = Director::getInstance()->getTextureCache()->addImage("player_attack.png");
    for (int i = 0; i < 8; i++)
    {
        player_attack[i].reserve(8);
        for (int j = 0; j < 8; j++)
        {
            auto frame = SpriteFrame::createWithTexture(player_attack_texture, CC_RECT_PIXELS_TO_POINTS(Rect(128 * j, 128 * i, 128, 128)));
            player_attack[i].pushBack(frame);
        }
        auto player_attack_anim = Animation::createWithSpriteFrames(player_attack[i], 0.1f);
        player_attack_anim->setRestoreOriginalFrame(false);
        string name = "player_attack_" + CommonSetting::getInstance()->Direction[i];
        //  log("%s %p", name.c_str(), player_attack_anim);
        AnimationCache::getInstance()->addAnimation(player_attack_anim, name);
    }
    // 创建player移动动画贴图
    auto player_move_texture = Director::getInstance()->getTextureCache()->addImage("player_move.png");
    for (int i = 0; i < 8; i++) 
    {
        player_move[i].reserve(8);
        for (int j = 0; j < 8; j++) 
        {
            auto frame = SpriteFrame::createWithTexture(player_move_texture, CC_RECT_PIXELS_TO_POINTS(Rect(128 * j, 128 * i, 128, 128)));
            player_move[i].pushBack(frame);
        }
        auto player_move_Animation = Animation::createWithSpriteFrames(player_move[i], 0.05f);
        player_move_Animation->setRestoreOriginalFrame(false);
        std::string name = "player_move_" + CommonSetting::getInstance()->Direction[i];
        AnimationCache::getInstance()->addAnimation(player_move_Animation, name);
    }
    // 创建monster1移动动画
    auto monster1_move_texture = Director::getInstance()->getTextureCache()->addImage("monster1_move.png");
    for (int i = 0; i < 8; i++)
    {
        monster1_move[i].reserve(8);
        for (int j = 0; j < 8; j++)
        {
            auto frame = SpriteFrame::createWithTexture(monster1_move_texture, CC_RECT_PIXELS_TO_POINTS(Rect(128 * j, 128 * i, 128, 128)));
            monster1_move[i].pushBack(frame);
        }
        auto monster1_move_Animation = Animation::createWithSpriteFrames(monster1_move[i], 0.1f);
        monster1_move_Animation->setRestoreOriginalFrame(false);
        string name = "monster1_move_" + CommonSetting::getInstance()->Direction[i];
        AnimationCache::getInstance()->addAnimation(monster1_move_Animation, name);
    }
    //创建monster1攻击动画
    auto monster1_attack_texture = Director::getInstance()->getTextureCache()->addImage("monster1_attack.png");
    for (int i = 0; i < 8; i++)
    {
        monster1_attack[i].reserve(8);
        for (int j = 0; j < 8; j++)
        {
            auto frame = SpriteFrame::createWithTexture(monster1_move_texture, CC_RECT_PIXELS_TO_POINTS(Rect(128 * j, 128 * i, 128, 128)));
            monster1_attack[i].pushBack(frame);
        }
        auto monster1_attack_Animation = Animation::createWithSpriteFrames(monster1_attack[i], 0.1f);
        monster1_attack_Animation->setRestoreOriginalFrame(false);
        string name = "monster1_attack_" + CommonSetting::getInstance()->Direction[i];
        AnimationCache::getInstance()->addAnimation(monster1_attack_Animation, name);
    }
    //创建Monster2移动动画
    auto monster2_move_texture = Director::getInstance()->getTextureCache()->addImage("monster2_move.png");
    for (int i = 0; i < 8; i++) 
    {
        monster2_move[i].reserve(8);
        for (int j = 0; j < 8; j++) 
        {
            auto frame = SpriteFrame::createWithTexture(monster2_move_texture, CC_RECT_PIXELS_TO_POINTS(Rect(128 * j, 128 * i, 128, 128)));
            monster2_move[i].pushBack(frame);
        }
        auto monster_move_animation = Animation::createWithSpriteFrames(monster2_move[i], 0.1f);
        monster_move_animation->setRestoreOriginalFrame(false);
        string name = "monster2_move_" + CommonSetting::getInstance()->Direction[i];
        AnimationCache::getInstance()->addAnimation(monster_move_animation, name);
    }
    //创建Boss移动动画
    auto boss_move_texture = Director::getInstance()->getTextureCache()->addImage("boss_move.png");
    for (int i = 0; i < 8; i++) 
    {
        boss_move[i].reserve(16);
        for (int j = 0; j < 16; j++) 
        {
            auto frame = SpriteFrame::createWithTexture(boss_move_texture, CC_RECT_PIXELS_TO_POINTS(Rect(176 * j, 176 * i, 176, 176)));
            boss_move[i].pushBack(frame);
        }
        auto boss_move_anim = Animation::createWithSpriteFrames(boss_move[i], 0.1f);
        boss_move_anim->setRestoreOriginalFrame(false);
        string name = "boss_move_" + CommonSetting::getInstance()->bossDirection[i];
        AnimationCache::getInstance()->addAnimation(boss_move_anim, name);
    }
    //创建boss攻击动画
    auto boss_attack_texture = Director::getInstance()->getTextureCache()->addImage("boss_attack.png");
    for (int i = 0; i < 8; i++) 
    {
        boss_attack[i].reserve(11);
        for (int j = 0; j < 11; j++) 
        {
            auto frame = SpriteFrame::createWithTexture(boss_attack_texture, CC_RECT_PIXELS_TO_POINTS(Rect(176 * j, 176 * i, 176, 176)));
            boss_attack[i].pushBack(frame);
        }
        auto boss_attack_anim = Animation::createWithSpriteFrames(boss_attack[i], 0.1f);
        boss_attack_anim->setRestoreOriginalFrame(false);
        string name = "boss_attack_" + CommonSetting::getInstance()->bossDirection[i];
        AnimationCache::getInstance()->addAnimation(boss_attack_anim, name);
    }
    //创建boss死亡动画
    auto boss_die_texture = Director::getInstance()->getTextureCache()->addImage("boss_die.png");
    for (int i = 0; i < 8; i++) 
    {
        boss_die[i].reserve(16);
        for (int j = 0; j < 11; j++) 
        {
            auto frame = SpriteFrame::createWithTexture(boss_die_texture, CC_RECT_PIXELS_TO_POINTS(Rect(176 * j, 176 * i, 176, 176)));
            boss_die[i].pushBack(frame);
        }
        auto boss_die_anim = Animation::createWithSpriteFrames(boss_die[i], 0.1f);
        boss_die_anim->setRestoreOriginalFrame(false);
        string name = "boss_die_" + CommonSetting::getInstance()->bossDirection[i];
        AnimationCache::getInstance()->addAnimation(boss_die_anim, name);
    }
}

void GameScene::addPlayer() 
{
    CommonSetting* comSet = CommonSetting::getInstance();
    comSet->setNetworkID(Hall::getInstance()->getMyPlayerID() + 1);
    stringstream ss;
    //	ss <<"add player ID: "<< Hall::getInstance()->getMyPlayerID() + 1;
    //	log(ss.str().data());

    for (int i = 1; i <= CSET->getNetworkCnt(); i++) 
    {
        Player::CreatePlayer(i);
        auto mySprite = CSET->player[i].getPlayer();
        mySprite->setPosition(9000 - i * 100, 7000 - i * 100);
       // mySprite->setScale(2);
        mySprite->setAnchorPoint(Vec2(0.5, 0.2));
        this->addChild(mySprite, -10);
        CSET->player[i].addBasicInfo();
        //CSETP.setNextDirection("West");
        //CSETP.Move(mySprite,m_tmxPtrTileMap,inaccessible1,inaccessible2);

    }
    auto initMove = MoveBy::create(0.8f, Vec2(0, -10));
    CSETP.getPlayer()->runAction(initMove);
    //this->setViewPointCenter(CSETP.getPlayer()->getPosition());
}



void GameScene::update(float delta) {
    if (Hall::getInstance()->isHost())
    {
        push_to_server(CSETP);
        //	cocos2d::log("GameScene::update add %d monsters", CSETP.closeMonster.size());
		for (auto i : CSETP.closeMonster)
		{
			BroadcastServer::getInstance()->addSceneMonster(*(CSET->monster[i]),i);
		}
    }
    else
    {
        push_to_client(CSETP);
        for (auto i : CSETP.closeMonster)
        {
            BroadcastClient::getInstance()->addSceneMonster(*(CSET->monster[i]), i);
        }
    }
    this->setViewPointCenter(CSETP.getPlayer()->getPosition());
    auto comSet = CommonSetting::getInstance();
    int networkCnt = comSet->getNetworkCnt();
    float maxLevel = 0;
    for (int i = 1; i <= networkCnt; i++)
        {
             maxLevel = max(maxLevel, CSET->player[i].getLevel());
            if (CSET->player[i].isDie()) 
            {
                if (!CSET->player[i].getIsScheduing()) 
                {
                    scheduleOnce(schedule_selector(GameScene::setDietoBorn),CSET->player[i].getLevel()*20+2);
                    CSET->player[i].setIsScheduing(true);
                }
                continue;
            }
            if(CSETN==i) CSETP.LevelUpAnim();
		    bool is_attack = false;
		    string nextDirection;
		    bool animDone = CSET->player[i].getAnimDone();
		    {
			    gs_mu.lock();
			    is_attack = CSET->player[i].isAttack();
			    nextDirection = CSET->player[i].getNextDirection();
			    animDone = CSET->player[i].getAnimDone();
			    gs_mu.unlock();
		    }
		    if (nextDirection != "Stop" || is_attack)
		    {
			    if (is_attack && animDone)
				    bulletLayer->playerAttack(i);
			    CSET->player[i].UpdateAnim();
			    if (nextDirection != "Stop")
				    CSET->player[i].Move(i,comSet->player[i].getPlayer(), m_tmxPtrTileMap, m_tmxLayerPtrBlock1, m_tmxLayerPtrBlock2);
			    //if (ishost)
			}
            CSET->player[i].m_bloodProgress->setBloodBar(CSET->player[i].getHP() /CSET->player->getMaxHP() * 100);
            CCString newOutput = CSET->player[i].getName();
            newOutput.append(" GPA : ");
            newOutput.appendWithFormat("%f", CSET->player[i].getLevel());
            CSET->player[i].m_proShow->setString(newOutput.getCString());
          // log("the player%d's level is %f", i, CSET->player[i].getLevel());
        }
 /*   for (int i = 1; i <= networkCnt; i++) 
    {
        if (CSET->player[i].getNextDirection() != "Stop" || CSET->player[i].isAttack()) 
        {
            if (CSET->player[i].isAttack()&&CSET->player[i].getAnimDone()) 
                bulletLayer->showAttack(i);
            CSET->player[i].UpdateAnim();
            if(CSET->player[i].getNextDirection()!="Stop")
                CSET->player[i].Move(comSet->player[i].getPlayer(), m_tmxPtrTileMap, m_tmxLayerPtrBlock1, m_tmxLayerPtrBlock2);
            //if (ishost)
        }
        log("the player %d HP is %f and the maxHP is %f", i,CSET->player[i].getHP(),100+CSET->player[i].getLevel()*1000);
        CSET->player[i].m_bloodProgress->setBloodBar(CSET->player[i].getHP() / (100 + CSET->player[i].getLevel() * 1000) * 100);
    }*/
    for (int i = 1; i <= CSET->getNetworkCnt(); i++)
        for (auto j : CSET->player[i].closeMonster) 
        {
            //log("monster HP --- - - - - - - - %f", CSET->monster[j]->getHP());
            CSET->monster[j]->m_bloodProgress->setBloodBar(CSET->monster[j]->getHP() / CSET->monster[j]->getMaxHP() * 100);
        }
    if (maxLevel >= 4.8) 
    {
        //调用GameOver 
    }
}

void GameScene::continueInit()
{
    audio = CocosDenshion::SimpleAudioEngine::getInstance();
    audio->playBackgroundMusic("GameSceneMusic.mp3", true);

    auto list = Hall::getInstance()->getNameList();
    int networkCNT = Hall::getInstance()->m_roomPlayerNumber;
    CSET->setNetworkCnt(networkCNT);
    for (int i = 0; i < CSET->getNetworkCnt(); i++)
    {
        CSET->player[i + 1].setName(list[i]);
    }
    //预加载plist
    auto plistManager = CCSpriteFrameCache::sharedSpriteFrameCache();
    plistManager->addSpriteFramesWithFile("blood.plist");

    m_tmxPtrTileMap = TMXTiledMap::create("map/map.tmx");
    m_tmxLayerPtrBlock2 = m_tmxPtrTileMap->getLayer("inaccessible2");
    m_tmxLayerPtrBlock2->setVisible(false);
    m_tmxLayerPtrBlock1 = m_tmxPtrTileMap->getLayer("inaccessible1");
    m_tmxLayerPtrBlock1->setVisible(false);
    //inaccessible1 = m_tmxPtrTileMap->getLayer("inaccessible1");
    //inaccessible2 = m_tmxPtrTileMap->getLayer("inaccessible2");
  //  m_tmxPtrTileMap->layerNamed("accessable1")->setVisible(true);
   // m_tmxPtrTileMap->setPosition(Vec2(-5000, -5000));
    m_tmxPtrTileMap->setScale(2);
    this->addChild(m_tmxPtrTileMap, -100);
    scheduleUpdate();
    preAnim();
    //添加摇杆
    solveUIRender();
    //设置玩家
    addPlayer();
    addMonster();//添加怪物
    addBullet();
    scheduleOnce(schedule_selector(GameScene::testBag), 0.1f);
    scheduleOnce(schedule_selector(GameScene::addItem), 1.0f);
    schedule(schedule_selector(GameScene::moveMonster), 0.2f, kRepeatForever, 5.0f);
    schedule(schedule_selector(GameScene::showPickUpMenu), 1.0f, kRepeatForever, 1.0f);
    //if (ishost)
    if (Hall::getInstance()->isHost())//只用于测试，生产环境使用if (ishost)
        {
            auto t = new boost::thread(start_broadcast_server,
                [this](st_CommonSetting a, BroadcastServer* b) {refresh_stcs_host(a, b); });

                ChatMessage msg;
                msg.message = "-1";
                msg.playerID = -1;
                ChatroomServer::get_instance()->post(msg);

            //HttpService::getInstance()->closeRoom();
            //发送房主已进入房间消息
        }
        else
        {
            auto t = new boost::thread(start_broadcast_client, Hall::getInstance()->getHostIP(), [this](st_CommonSetting a, BroadcastClient* b)
            {
                refresh_stcs_client(a, b);
            }, Hall::getInstance()->getMyPlayerID() + 1
                );
        }
}

void set_player_from_st(Player& target, const st_Player& source)
{   
    gs_mu.lock();
    target.setAnimDone(source.animDone);
    target.setDamage(source.damage);
    target.setHP(source.HP);
    target.setLevel(source.Level);
    target.setName(source.name);
    target.setNextDirection(source.nextDirection);
    target.setNowDirection(source.nowDirection);
    target.setScore(source.score);
	target.setDegree(source.degree);
    target.setSpeed(source.speed);
    target.setIsAttack(source.is_attack);
    target.setBulletType(source.bulletType);
    float x = target.getPlayer()->getPosition().x;
    float y = target.getPlayer()->getPosition().y;
    if ((x - source.x)*(x - source.x) + (y - source.y)*(y - source.y) > 500)
    {
    //    log("修正玩家位置");
        target.getPlayer()->setPosition(Vec2(source.x, source.y));
    }
    gs_mu.unlock();
}

void GameScene::refresh_commonsetting(int except)
{
    auto cs = CommonSetting::getInstance();
    //stringstream ss;
    for (int i = 0; i < 4; i++)
    {

        if (st_cs.st_Player[i + 1].name != ""&&i != except - 1)
        {
            set_player_from_st(cs->player[i + 1], st_cs.st_Player[i + 1]);
        }
    }
    /*ss << "-----------------\n";
    for (int i = 0; i < 4; i++)
    {
        ss << "player( " << i + 1 << " )" << "\t" << "animDone:" <<
            cs->player[i + 1].getAnimDone() << "now dir:" << cs->player[i + 1].getNowDirection()
            << "next dir:" << cs->player[i + 1].getNextDirection() << '\n';
    }
    ss << '\n';
    log(ss.str().data());*/
}


void set_stplayer_from_player(st_Player &sp, Player& p)
{
    sp.bulletType = p.getBulletType();
    sp.animDone = p.getAnimDone();
    sp.damage = p.getDamage();
    sp.HP = p.getHP();
    sp.Level = p.getLevel();
    log("set level to %d", sp.Level);
    sp.name = p.getName();
    sp.nextDirection = p.getNextDirection();
    sp.nowDirection = p.getNowDirection();
    sp.score = p.getScore();
    sp.speed = p.getSpeed();
    sp.is_attack = p.isAttack();
	sp.degree = p.getDegree();
    sp.x = p.getPlayer()->getPosition().x;
    sp.y = p.getPlayer()->getPosition().y;
}
void GameScene::refresh_stcs_host(st_CommonSetting st_cs_, BroadcastServer * serverptr)
{
    st_Player sp;
    set_stplayer_from_player(sp, CSETP);
    //stringstream ss;

    for (int i = 0; i < 4; i++)
    {
        st_cs.st_Player[i + 1] = st_cs_.st_Player[i + 1];
    }
    //auto cs = CommonSetting::getInstance();
    /*	ss << "-----------------\n";
        for (int i = 0; i < 4; i++)
        {
            ss <<"player( "<< i+ 1 <<" )"<<"\t"<<"animDone:"<<
                cs->player[i + 1].getAnimDone() <<"now dir:"<<cs->player[i+1].getNowDirection()
                <<"next dir:"<<cs->player[i+1].getNextDirection()<< '\n';
        }
        ss << '\n';
        log(ss.str().data());*/
	for (int i : st_cs_.collect_items)
	{
		removeItem(i);
	}
    refresh_commonsetting(1);	//1是host
    refresh_monsters(st_cs_.monsters);
}

void GameScene::refresh_monsters(vector<st_Monster> smonvec)
{
    for (auto mon : smonvec)
    {
        Monster* monref = CSET->monster[mon.monsterID];
        if (monref != nullptr)
        {
            monref->setDamage(mon.m_fDamage);
            monref->setDegree(mon.m_fDegree);
            monref->setIsAttack(mon.m_bIsAttack);
            if (monref->getHP() > mon.m_fHP)
                monref->setHP(mon.m_fHP);
            monref->setSpeed(mon.m_fSpeed);
            monref->setNextDirection(mon.m_strNextDirection);
            monref->setNowDirection(mon.m_strNowDirection);
            if (monref->getHP() >= 0 && monref != nullptr)
            {
                float x = monref->getMonster()->getPosition().x;
                float y = monref->getMonster()->getPosition().y;
                float dis = (x - mon.x)*(x - mon.x) + (y - mon.y)*(y - mon.y);
                if (dis > 1000 && x != 0 && y != 0)
                    monref->getMonster()->setPosition(Vec2(mon.x, mon.y));
            }
            else if (monref->getHP() < 0 + eps + 3) {
                monref->getMonster()->removeFromParent();
                CSET->monster[mon.monsterID] = nullptr;
            }
        }
    }

}

void GameScene::refresh_stcs_client(st_CommonSetting st_cs_, BroadcastClient * clientptr)
{
    st_Player sp;
    set_stplayer_from_player(sp, CSETP);
    stringstream ss;
    for (int i = 0; i < 4; i++)
    {
        st_cs.st_Player[i + 1] = st_cs_.st_Player[i + 1];

    }

    auto cs = CommonSetting::getInstance();
	for (int i : st_cs_.collect_items)
	{
		removeItem(i);
	}

    refresh_commonsetting(CSETN);
    refresh_monsters(st_cs_.monsters);
}

void push_to_server(Player& p)
{
    st_Player sp;
    set_stplayer_from_player(sp, p);
    auto server = BroadcastServer::getInstance();
    if (server == nullptr)
        return;
    else server->set_host_player_status(sp);
}

void push_to_client(Player& p)
{
    st_Player sp;
    set_stplayer_from_player(sp, p);
    auto client = BroadcastClient::getInstance();
    if (client == nullptr)
        return;
    else client->set_client_status(sp);

}

void GameScene::removeItem(int i)
{
    //在这里实现
    if (CSET->item[i]->getitem_PlayerId() == 0) {
        CSET->item[i]->getitemIcon()->getParent()->removeFromParent();
        CSET->item[i]->setitem_PlayerId(5);
        if (CSET->item[i]->isshow == true) {
            CSET->player[CSETN].getPlayer()->removeChildByTag(5, false);
            CSET->item[i]->isshow = false;
        }
    }
}

void GameScene::addBullet()
{
    bulletLayer = Bullet::create();
    this->addChild(bulletLayer,-20);
}


void GameScene::showPickUpMenu(float dt) {
    int sizeItem = CSET->iHashId.size();
    Vec2 itemPos;
    int playerId = CSET->getNetworkCnt();
    for (int i = 0; i < sizeItem; ++i) {
        int id = CSET->iHashId.at(i);
        if (CSET->item[id]->getitem_PlayerId() == 0) {
            if (CSET->item[id]->isshow == false) {
                itemPos = CSET->item[id]->getitemIcon()->getParent()->getPosition();
                double dis = getDis(itemPos, CSET->player[CSETN].getPlayer()->getPosition());
                if (dis < 100) {
                    auto node1 = CSET->item[id]->getPickUpMenu(id, CSETN);
                    CSETP.getPlayer()->addChild(node1, -100, 5);
                    auto winSize = Director::getInstance()->getWinSize();
                    // node1->setPosition(winSize.width / 3, winSize.height / 3);
                    auto scaleFactor = Director::getInstance()->getContentScaleFactor();
                    node1->setPosition(400 / scaleFactor, 300 / scaleFactor);
                    //this->runAction(Sequence::create(DelayTime::create(0.98f), CallFunc::create([&]() {
                    //	CSET->player[CSETN].getPlayer()->removeChildByTag(5, false);
                    //}), NULL));
                    CSET->item[id]->isshow = true;
                }
            }
            else if (CSET->item[id]->isshow = true) {
                itemPos = CSET->item[id]->getitemIcon()->getParent()->getPosition();
                double dis = getDis(itemPos, CSET->player[CSETN].getPlayer()->getPosition());
                if (dis >= 100) {
                    CSET->player[CSETN].getPlayer()->removeChildByTag(5, false);
                    CSET->item[id]->isshow = false;
                }
            }
        }
    }
}

void GameScene::addItem(float dt)
{

    vector<gun> gunlist = gun::initGunList();
    vector<armor> armorlist = armor::initArmorList();
    vector<consum> consumlist = consum::initConsumList();

    int cnt = 0;
    boost::mt19937 rng(0);
    cocos2d::TMXObjectGroup *itemObjGroup = m_tmxPtrTileMap->getObjectGroup("item_group");
    double screenScale = Director::getInstance()->getContentScaleFactor();
    for (auto&item : itemObjGroup->getObjects()) {
        ValueMap& dict = item.asValueMap();
        if (dict["collectable"].asInt() == 1) {
            if (rng() % 50 == 1) {
                int itemkind = rng() % 3;
                if (itemkind == 0) {
                    int typeId1 = rng() % 6;
                    CommonSetting::getInstance()->item[++cnt] = new gun(gunlist[typeId1]);
                }
                else if (itemkind == 1) {
                    int typeId2 = rng() % 2;
                    CommonSetting::getInstance()->item[++cnt] = new armor(armorlist[typeId2]);
                }
                else if (itemkind == 2) {
                    int typeId3 = rng() % 2;
                    CommonSetting::getInstance()->item[++cnt] = new consum(consumlist[typeId3]);
                }

                CSET->item[cnt]->creatItem(cnt);
                int x = dict["x"].asInt();
                int y = dict["y"].asInt();
                double px = (double)x * m_tmxPtrTileMap->getScale();
                double py = (double)y * m_tmxPtrTileMap->getScale();

                Node *itemNode = Node::create();
                auto itemLabel = Label::create(CSET->item[cnt]->getitem_Name(), "Arial", 40);
                itemNode->addChild(CSET->item[cnt]->getitemIcon());
                itemNode->addChild(itemLabel);

                itemLabel->setPosition(0, 50);

                itemNode->setPosition(px, py);
                this->addChild(itemNode, -100);
                CSET->iHashId.push_back(cnt);

            }
        }

    }
    pair<int, int> myRecord(0, 0);
    CommonSetting::getInstance()->player[CSETN].iHashID.push_back(myRecord);
}