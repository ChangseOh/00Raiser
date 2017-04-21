#include "GameMaster.h"

USING_NS_CC;
using namespace std;

GameMaster* GameMaster::create()
{
	GameMaster* sprite = new GameMaster();

	if (sprite->init())
		return sprite;

	CC_SAFE_DELETE(sprite);
	return NULL;
}

// on "init" you need to initialize your instance
bool GameMaster::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
	this->setName("GAMEMASTER");
	this->setGlobalZOrder(-1);
	this->setVisible(false);

	scheduleUpdate();

    return true;
}

void GameMaster::update(float dt)
{
	hasNew = (gold >= nextupgrade[0]);
	hasUpgrade = false;
	for (int i = 1; i < 12; i++)
		if (gold >= nextupgrade[i])
			hasUpgrade = true;
	hasReward = false;
	for (int i = 0; i < 9; i++)
		if (scorereward[i] >= nextreward[i])
			hasReward = true;
}