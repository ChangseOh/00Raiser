#include "GameLayer.h"
#include "UpgradeLayer.h"
#include "RewardLayer.h"
#include "Title\TitleScene.h"

USING_NS_CC;
using namespace std;

GameLayer* GameLayer::create()
{
	GameLayer* sprite = new GameLayer();

	if (sprite->init())
		return sprite;

	CC_SAFE_DELETE(sprite);
	return NULL;
}

// on "init" you need to initialize your instance
bool GameLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	touchIdx = -1;
	this->setTag(0);
	setName("GAME_LAYER");

	auto bg = Sprite::create("gamebg1.png");
	bg->setPosition(center);
	addChild(bg);

	auto listener = setListener();

	string fs[11] = {
		"help.png",
		"home.png",
		"scoreboard.png",
		"upgrade.png",
		"reward.png",
		"potion1.png",
		"potion2.png",
		"fruits.png",
		"notice.png",
		"notice2.png",
		"notice.png",
	};
	bool isListen[11] = {
		true,true,false,true,true,true,true,true,
		false,false,false
	};
	string tagname[11] = {
		"HELP",
		"HOME",
		"SCOREBOARD",
		"UPGRADE",
		"REWARD",
		"POTION1",
		"POTION2",
		"FRUITS",
		"NOTICE1",
		"NOTICE2",
		"NOTICE3",
	};
	Vec2 pos[11] = {
		Vec2(1068,76),
		Vec2(1182,76),
		Vec2(991,432),
		Vec2(847,592),
		Vec2(1119,592),
		Vec2(116,622),
		Vec2(295,622),
		Vec2(482,622),
		Vec2(940,523),
		Vec2(751,523),
		Vec2(1222,523),
	};
	for (int i = 0; i < 11; i++)
	{
		auto spr = Sprite::create(fs[i]);
		spr->setPosition(pos[i].x, visibleSize.height - pos[i].y);
		spr->setName(tagname[i]);
		spr->setTag(i + 1);
		addChild(spr);

		spr->setVisible(i < 8);
		if(isListen[i])
			_eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), spr);
		if (i > 7)
			spr->runAction(RepeatForever::create(Sequence::create(
				FadeIn::create(0.5f),
				DelayTime::create(1.0f),
				FadeOut::create(0.5f),
				NULL)));
	}

	auto money = Label::create();
	money->setTextColor(Color4B::BLACK);
	money->setSystemFontSize(24);
	money->setPosition(830, visibleSize.height - 432);
	money->setName("MONEY");
	money->setString(numComma(gold));
	money->setAnchorPoint(Vec2(0.0f, 0.5f));
	addChild(money);

	Vec2 labelpos[3] = {
		Vec2(175,620),
		Vec2(345,620),
		Vec2(515,620),
	};
	string labelname[3] = {
		"POTIONEA1",
		"POTIONEA2",
		"FRUITSEA",
	};
	for (int i = 0; i < 3;i++)
	{
		auto ea = Label::create();
		ea->setTextColor(Color4B::BLACK);
		ea->setSystemFontSize(24);
		ea->enableOutline(Color4B::WHITE, 2);
		ea->setPosition(labelpos[i].x, visibleSize.height - labelpos[i].y);
		ea->setName(labelname[i]);
		if (i == 0)
			ea->setString(numComma(potionA));
		if (i == 1)
			ea->setString(numComma(potionB));
		if (i == 2)
			ea->setString(numComma(fruits));
		ea->setAnchorPoint(Vec2(1.0f, 0.5f));
		addChild(ea);
	}

	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	scheduleUpdate();

    return true;
}

void GameLayer::attack(Vec2 pos, int kind)
{

	bool cri = (critical > 0) && (random(1, 100) < 11) && (kind == 0);
	auto heart = Sprite::create("checker.png");
	heart->setScale(0.1f);
	heart->setPosition(pos);
	if (cri)
		heart->setColor(Color3B(128, 255, 255));
	addChild(heart);

	heart->runAction(Sequence::create(
		ScaleTo::create(0.2f, 0.5f + (weapon * 0.05f) * (cri ? 2 : 1)),
		ScaleBy::create(0.1f, 0.8f),
		FadeOut::create(0.1f),
		CallFunc::create([=](void) {
		eventMake(cri);
		heart->removeFromParent();
	}),
		NULL));
}
void GameLayer::update(float dt)
{

	getChildByName("NOTICE2")->setVisible(hasNew);
	getChildByName("NOTICE1")->setVisible(hasUpgrade);
	getChildByName("NOTICE3")->setVisible(hasReward);
	((Label*)getChildByName("MONEY"))->setString(numComma(gold));



	//자동화 처리
	if (playerAuto > 0)
	{
		playerAuto -= dt;
		if (playerAuto <= 0)
		{
			attack(center, 0);
			playerAuto = autoTime;
		}
	}
	if (multipleAuto > 0)
	{
		multipleAuto -= dt;
		if (multipleAuto <= 0)
		{
			attack(center, 1);
			multipleAuto = multiple;
		}
	}
	if (fruitsAuto > 0)
	{
		fruitsAuto -= dt;
	}
	if (awakeAuto > 0)
	{
		awakeAuto -= dt;
	}



	int _touchIdx = touchIdx;
	Vec2 _touchPos = touchPos;
	switch (_touchIdx)
	{
	case 0:
		attack(_touchPos, 0);
		awakeLeft++;
		if (awakeLeft >= wakeup)
			awakeAuto = wakeuppower;
		touchIdx = -1;
		break;
	case 1:
		touchIdx = -1;
		break;
	case 2:
	{
		auto scene = TitleScene::createScene();
		Director::getInstance()->replaceScene(scene);
		touchIdx = -1;
	}
		break;
	case 4:
	{
		getParent()->getChildByName("UPGRADE_LAYER")->setLocalZOrder(1);
		this->setLocalZOrder(0);
	}
		touchIdx = -1;
		break;
	case 5:
		getParent()->getChildByName("REWARD_LAYER")->setLocalZOrder(1);
		this->setLocalZOrder(0);
		touchIdx = -1;
		break;
	case 6://POTION1
		touchIdx = -1;
		break;
	case 7://POTION2
		touchIdx = -1;
		break;
	case 8://과일
		fruits--;
		((Label*)getChildByName("FRUITSEA"))->setString(numComma(fruits));
		fruitsAuto = fruitsTime;
		touchIdx = -1;
		break;
	case 77:
		if (fruitsAuto > 0)
			break;
		fruits++;
		((Label*)getChildByName("FRUITSEA"))->setString(numComma(fruits));
		touchIdx = -1;
		break;
	}

}
EventListenerTouchOneByOne* GameLayer::setListener()
{

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [=](Touch* touch, Event* event) {
		auto target = event->getCurrentTarget();
		auto loc = touch->getLocation();
		//loc = target->convertToNodeSpace(loc);
		if (target->getBoundingBox().containsPoint(loc))
		{
			target->setColor(Color3B::GRAY);
			return true;
		}
	};
	listener->onTouchMoved = [=](Touch* touch, Event* event) {
		auto target = event->getCurrentTarget();
		auto loc = touch->getLocation();
		if (target->getBoundingBox().containsPoint(loc))
		{

		}
		else
		{
			target->setColor(Color3B::WHITE);
		}
	};
	listener->onTouchEnded = [=](Touch* touch, Event* event) {
		auto target = event->getCurrentTarget();
		auto loc = touch->getLocation();
		if (target->getBoundingBox().containsPoint(loc))
		{
			target->setColor(Color3B::WHITE);
			touchIdx = target->getTag();
			if (touchIdx == 0)
			{
				touchNum++;
				touchPos = loc;
			}
			if (touchIdx >= 77)
				target->removeFromParent();
		}
	};

	return listener;
}
void GameLayer::eventMake(bool cri)
{
	float add = (1 * (level + 1) + (weapon * 10));
	if (cri)
	{
		add = add + add * critical;
	}
	if(fruitsAuto>0)
		add+=add*
	if (awakeAuto > 0)
		add = add * 2;
	shootNum++;
	gold = gold + (int)add;

	if (fruitsTime > 0 && random(1, 1000) < 11)
	{
		auto fruitspr = Sprite::create("raspberry.png");
		fruitspr->setPosition(center);
		fruitspr->setTag(77);
		addChild(fruitspr);
		_eventDispatcher->addEventListenerWithSceneGraphPriority(setListener(), fruitspr);
	}
}
