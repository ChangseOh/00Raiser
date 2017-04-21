#include "GameScene.h"
#include "GameLayer.h"
#include "UpgradeLayer.h"
#include "RewardLayer.h"

USING_NS_CC;

Scene* GameScene::createScene()
{
    // 'scene' is an autorelease object
    auto scene = Scene::create();
    
    // 'layer' is an autorelease object
    auto layer = GameScene::create();

    // add layer as a child to scene
    scene->addChild(layer);

    // return the scene
    return scene;
}

// on "init" you need to initialize your instance
bool GameScene::init()
{
    if ( !Layer::init() )
    {
        return false;
    }
    
	auto gameLayer = GameLayer::create();
	gameLayer->setLocalZOrder(1);
	addChild(gameLayer);

	auto upgradeLayer = UpgradeLayer::create();
	upgradeLayer->setLocalZOrder(0);
	addChild(upgradeLayer);

	auto rewardLayer = RewardLayer::create();
	rewardLayer->setLocalZOrder(0);
	addChild(rewardLayer);
	
	return true;
}
