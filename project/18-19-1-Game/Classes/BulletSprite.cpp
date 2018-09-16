#include "BulletSprite.h"

USING_NS_CC;
using namespace std;

void BulletSprite::setPlayerID(int index)
{
	playerID = index;
}

int BulletSprite::getPlayerID() const
{
	return playerID;
}

void BulletSprite::setMonsterID(int index)
{
	monsterID = index;
}

int BulletSprite::getMonsterID() const
{
	return monsterID;
}

void BulletSprite::setDamage(float dam)
{
	damage = dam;
}

float BulletSprite::getDamage() const
{
	return damage;
}

//´´½¨¾«Áé
BulletSprite * BulletSprite::create(const string& filename)
{
	BulletSprite *sprite = new BulletSprite();
	if (sprite && sprite->initWithFile(filename))
	{

		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

BulletSprite* BulletSprite::createWithTexture(Texture2D *texture)
{
	BulletSprite *sprite = new (std::nothrow) BulletSprite();
	if (sprite && sprite->initWithTexture(texture))
	{
		sprite->autorelease();
		return sprite;
	}
	CC_SAFE_DELETE(sprite);
	return nullptr;
}

bool BulletSprite::operator==(BulletSprite& b)
{
	if (playerID != b.playerID)
		return false;
	if (monsterID != b.monsterID)
		return false;
	if (damage != b.damage)
		return false;
	return true;
}
