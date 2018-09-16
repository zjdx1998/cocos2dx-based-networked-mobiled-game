
#include "CommonSetting.h"
#include "Bullet.h"
//音效
//#include "SimpleAudioEngine.h"
#include <math.h>
#define PI 3.1415926

USING_NS_CC;
using namespace std;
/*
Bullet* Bullet::getInstance() 
{
    if (instance == nullptr) instance = new Bullet();
    return instance;
}*/

Bullet::~Bullet()
{
}

void Bullet::setSpeed(float s)
{
	speed = s;
}

float Bullet::getSpeed()const
{
	if (speed) return speed;
	else return 0.0f;
}

void Bullet::setPower(float pow)
{
	power = pow;
}

float Bullet::getPower() const
{
	return power;
}

void Bullet::setType(type t)
{
	distance = DISTANCE;
	switch (t)
	{
	case BLACK: 
	{
		speed = 1.0f;
		thisType = t; 
		picName="bullet1.png";
		nowTexture = bullet1;
		power = 15.0f;
		break;
	}
	case GREEN:
	{
		speed = 0.9f;
		thisType = t;
		picName="bullet2.png";
		nowTexture = bullet2;
		power = 3.0f;
		break;
	}
	case BLUE:
	{
		speed = 0.7f;
		thisType = t;
		picName = "bullet3.png";
		nowTexture = bullet3;
		power = 4.0f;
		break;
	}
	case YELLOW:
	{
		speed = 0.5f;
		thisType = t;
		picName = "bullet5.png";
		nowTexture = bullet4;
		power = 5.0f;
		break;
	}
	case X_RAY:
	{
		speed = 0.5f;
		thisType = t;
		picName = "bullet6.png";
		nowTexture = bullet5;
		power = 15.0f;
		break;
	}
	default:
	{
		speed = 0.5f;
		thisType = t;
		picName = "bullet1.png";
		nowTexture = bullet1;
		power = 2.0f;
		break;
	}
	}
}
void Bullet::setType(const string& str)
{
	//BLACK,GREEN,BLUE,YELLOW,X_RAY
	if (str == "BLACK") { setType(BLACK); }
	if (str == "GREEN") { setType(GREEN); }
	if (str == "BLUE") { setType(BLUE); }
	if (str == "YELLOW") { setType(YELLOW); }
	if (str == "X_RAY") { setType(X_RAY); }
}

type Bullet::getType() const
{
	return thisType;
}

void Bullet::setPicName(std::string name)
{
	picName = name;
}

std::string Bullet::getPicName()
{
	return picName;
}

void Bullet::setDistance(float d)
{
	distance=d;
}
float Bullet::getDistance() const
{
	return distance;
}

void Bullet::setMonsterDt(const float dt)
{
	monsterDt = dt;
}

void Bullet::setPlayerDt(const float dt)
{
	playerDt = dt;
}

bool Bullet::init()
{
	if (!Layer::init())
	{
		return false;
	}
	//创建子弹
	bullet1 = Director::getInstance()->getTextureCache()->addImage("bullet1.png");
	bullet2 = Director::getInstance()->getTextureCache()->addImage("bullet2.png");
	bullet3 = Director::getInstance()->getTextureCache()->addImage("bullet3.png");
	bullet4 = Director::getInstance()->getTextureCache()->addImage("bullet5.png");
	bullet5 = Director::getInstance()->getTextureCache()->addImage("bullet6.png");
	bulletFire = Director::getInstance()->getTextureCache()->addImage("fireball.png");
	thisType = BLUE;
	setType(thisType);
	monsterDt = 0.8f;
	playerDt = 0.8f;
	for (int i = 1; i < 5; i++)
	{
		canAttack[i] = true;
	}
	
    schedule(schedule_selector(Bullet::playerCanAttack), playerDt);
	schedule(schedule_selector(Bullet::ifCollide), 0.2f, kRepeatForever, 0.1f);
	schedule(schedule_selector(Bullet::monsterAttack), monsterDt);
	return true;
}

void Bullet::playerCanAttack(float t)
{
	for(int i = 1; i <= CSET->getNetworkCnt(); i++)
		canAttack[i] = true;
}

void Bullet::playerAttack(int index)
{
	if (canAttack[index])//如果满足当前间隔时间
	{
		//得到发出子弹的玩家及子弹种类
		Sprite * player = CommonSetting::getInstance()->player[index].getPlayer();
		string nowType = CSET->player[index].getBulletType();
		setType(nowType);

		//计算子弹的角度
		double degree = CSET->player[index].getDegree();
		double arf = -(degree - 180)*PI / 180;

		//设置发出子弹的位置
		auto size = player->getBoundingBox().size;
		Vec2 birthPlace = Vec2(player->getPosition().x + cos(arf) * size.height,
			player->getPosition().y + sin(arf) * size.height +(size.height / 4));

		BulletSprite * bullet = BulletSprite::createWithTexture(nowTexture);
		bullet->setPosition(birthPlace);
		bullet->setRotation(degree);
		bullet->setPlayerID(index);
		bullet->setDamage(power);
		this->addChild(bullet);
		
		bulletVector.pushBack(bullet);

		//auto fadeIn = FadeIn::create(0.1f);
		auto moveBy = MoveBy::create(speed, Vec2(cos(arf)*distance * 10, sin(arf)*distance * 10));
		//auto fadeOut = FadeOut::create(0.1f);

		auto actionDone = CallFunc::create([bullet, this]() {
			this->removeChild(bullet);
			//将子弹从_bulletVector中移除
			bulletVector.eraseObject(bullet);
		});
			//this,callfuncN_selector(Bullet::bulletMoveFinished));
		Sequence * sequence = Sequence::create(moveBy, actionDone, nullptr);
		bullet->runAction(sequence);
		canAttack[index] = false;
	}
}


float Bullet::getDegree(Vec2 A, Vec2 B)
{
	auto deltaY = B.y - A.y;
	auto deltaX = B.x - A.x;
	auto tangle = atan(abs(deltaY) / abs(deltaX));
	if (deltaY >= 0 && deltaX <= 0) return tangle;
	if (deltaY >= 0 && deltaX >= 0) return acos(-1) - tangle;
	if (deltaY <= 0 && deltaX >= 0) return acos(-1) + tangle;
	return 2 * acos(-1) - tangle;
}

float Bullet::getDis(Vec2 A, Vec2 B)
{
	return sqrt((A.x - B.x)*(A.x - B.x) + (A.y - B.y)*(A.y - B.y));
}

void Bullet::startMonsterAttack(int index, int minDisId)
{
	if (CSET->player[minDisId].getHP() < 0 + eps)
	{
		return;
	}
	if (monsterID.empty())
	{
		monsterID.push_back(index);
		minDisPlayerID.push_back(minDisId);
		return;
	}
	else
	{
		for (size_t i = 0; i < monsterID.size(); i++)
		{
			if (monsterID[i] == index)
			{
				minDisPlayerID[i] = minDisId;
				return;
			}
		}
		monsterID.push_back(index);
		minDisPlayerID.push_back(minDisId);
	}
}

void Bullet::stopMonsterAttack(int index, int minDisId)
{
	for (size_t i = 0; i < monsterID.size(); i++)
	{
		if (monsterID[i] == index && minDisPlayerID[i] == minDisId)
		{
			monsterID.erase(monsterID.begin() + i);
			minDisPlayerID.erase(minDisPlayerID.begin() + i);
			i--;
		}
	}
}

void Bullet::monsterAttack(float delayTime)
{
	if (monsterID.size() > 0)
	{
		for (int i = 0; i < (int)monsterID.size(); i++)
		{
			Sprite * monster = CSET->monster[monsterID[i]]->getMonster();
			auto size = monster->getBoundingBox().size;

			auto degree = getDegree(monster->getPosition(),
				CSET->player[minDisPlayerID[i]].getPlayer()->getPosition());
			degree = degree * 180 / const_pi;
			CSET->monster[monsterID[i]]->setDegree(degree);
			double arc = -(degree - 180) * PI / 180;

			Vec2 birthPlace = Vec2(monster->getPosition().x + cos(arc) * size.height / 2,
				monster->getPosition().y + sin(arc) * size.height / 2 + (size.height / 4));
            BulletSprite* bullet = nullptr;
            if(CSET->monster[monsterID[i]]->getKind()==3) 
                bullet = BulletSprite::createWithTexture(bullet5);
			else 
                bullet = BulletSprite::createWithTexture(bulletFire);
			bullet->setPosition(birthPlace);
			bullet->setRotation(degree);
			bullet->setMonsterID(monsterID[i]);
			this->addChild(bullet);

			monsterBullet.pushBack(bullet);

			auto moveBy = MoveBy::create(speed, Vec2(cos(arc) * 10 * distance, sin(arc) * 10 * distance));
			auto delay = DelayTime::create(0.5f);

	
			auto actionDone = CallFunc::create([bullet, this]() {
				this->removeChild(bullet,true);
				//将子弹从monsterBullet中移除
				monsterBullet.eraseObject(bullet);
			});
			Sequence * sequence = Sequence::create(delay, moveBy,actionDone, nullptr);
			bullet->runAction(sequence);

		}
			
	}

}


//去除子弹
void Bullet::removeBullet(BulletSprite * bullet)
{
	if (bullet != nullptr)
	{
		this->removeChild(bullet, true);
		bulletVector.eraseObject(bullet);
		monsterBullet.eraseObject(bullet);
	}
}

//伤害
//人物
//
//Damage = (initial Damage = 10) + Level * 10 + EquipmentBuff
//怪物
//
//$Damage = (initialDamage = 8) + SystemTimeBuffer $

void Bullet::ifCollide(float dt)
{
	//需要删除的bullet
	Vector<BulletSprite *> bulletToDel;
	//遍历所有由人发出的bullet
	for (BulletSprite* bt : bulletVector)
	{
		BulletSprite * bullet = bt;
		auto id = bullet->getPlayerID();
		for (int index = 0; index < CSET->player[id].closeMonster.size(); index++)
		{
			auto mt = CSET->player[id].closeMonster[index];
			Monster * monster = CommonSetting::getInstance()->monster[mt];

			//monster->getMonster()->getBoundingBox().intersectsRect(bullet->getBoundingBox())
			if (monster->getMonster()->getBoundingBox().containsPoint(bullet->getPosition()))//碰撞
			{
				//Damage = (initial Damage = 10) + Level * 10 + EquipmentBuff
				float nowDamage = CSET->player[id].getLevel() * 10 + CSET->player[id].getDamage() + bullet->getDamage();
				monster->setHP(monster->getHP() - nowDamage);
				// log("after monsterHP %f and the level is %f", monster->getHP(),CSETP.getLevel());
				//this->removeBullet(bullet);
				bulletToDel.pushBack(bullet);

				if (monster->getHP() < 0 + eps)
				{
					//打死小怪，人物加分
					CSET->player[id].setScore(CSET->player[id].getScore() + 5 - monster->getKind() * 2);
					monster->getMonster()->removeFromParent();
					CommonSetting::getInstance()->monster[mt] = nullptr;
					//怪物从closeMonster里去掉
					for (int i = 1; i <= CSET->getNetworkCnt(); i++)
					{
						if (i == id)
						{
							CSET->player[id].closeMonster.erase(CSET->player[id].closeMonster.begin() + index);
							index--;
						}
						else
							for (int j=0;j< CSET->player[i].closeMonster.size();j++)
							{
								if (CSET->player[i].closeMonster[j] == mt)
								{
									CSET->player[i].closeMonster.erase(CSET->player[i].closeMonster.begin() + j);
									j--;
								}
							}
					}
					
					//怪物子弹删除
					for (int i = 0; i < monsterID.size(); i++)
					{
						if (monsterID[i] == mt)
						{
							monsterID.erase(monsterID.begin() + i);
							minDisPlayerID.erase(minDisPlayerID.begin() + i);
							i--;
						}
					}
				}
				break;
			}
		}

		for (int i = 1; i <= CSET->getNetworkCnt(); i++)
		{
			if(i==bullet->getPlayerID()){}
			else {
				Player*  player = &(CommonSetting::getInstance()->player[i]);

				if (player->getPlayer()->getBoundingBox().containsPoint(bullet->getPosition()))//碰撞
				{
					player->setHP(player->getHP() - CSETP.getDamage()- bullet->getDamage());
					// log("after monsterHP %f and the level is %f", monster->getHP(),CSETP.getLevel());
					//this->removeBullet(bullet);
					bulletToDel.pushBack(bullet);
					if (player->getHP() < 0 + eps)
					{
						//人死复生
						//player->getPlayer()->removeFromParent();
						//player = nullptr;
						player->PlayerDie();

						CSET->player[id].setScore(CSET->player[id].getScore() + 5 + 10*player->getLevel());
					}
					break;
				}
			}
		}
	}
	//遍历所有由怪物发出的子弹
	for (BulletSprite* bt : monsterBullet)
	{
		BulletSprite* bullet = bt;
		auto id = bullet->getMonsterID();
		if (CSET->monster[id] == nullptr) continue;
		for (int i = 1; i <= CSET->getNetworkCnt(); i++)
		{
			Player * player = &(CommonSetting::getInstance()->player[i]);
			//monster->getMonster()->getBoundingBox().intersectsRect(bullet->getBoundingBox())
			if (player->getPlayer()->getBoundingBox().containsPoint(bullet->getPosition()))
			{
				//  log("initial monsterHP %f", monster->getHP());
				player->setHP(player->getHP()  - CSET->monster[id]->getDamage());
				//log("%f", player->getHP());
				// log("after monsterHP %f and the level is %f", monster->getHP(),CSETP.getLevel());
				bulletToDel.pushBack(bullet);

				if (player->getHP() < 0 + eps)
				{
					 player->PlayerDie();
				}
				break;
			}
		}
		
	}

	for (BulletSprite* bt : bulletToDel)
	{

		BulletSprite * bullet = bt;
		removeBullet(bullet);//删除子弹
	}
}



