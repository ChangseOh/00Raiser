#include "TitleLayer.h"
#include "Game\GameScene.h"

USING_NS_CC;
using namespace std;

TitleLayer* TitleLayer::create()
{
	TitleLayer* sprite = new TitleLayer();

	if (sprite->init())
		return sprite;

	CC_SAFE_DELETE(sprite);
	return NULL;
}

// on "init" you need to initialize your instance
bool TitleLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	touchIdx = -1;
	setName("TITLE_LAYER");

	//auto clayer = LayerColor::create(Color4B(255, 255, 255, 255));
	//addChild(clayer);
	auto bg = Sprite::create("titlebg.png");
	bg->setPosition(center);
	addChild(bg);

	string fs[4] = {
		"start.png",
		"bossroom.png",
		"option.png",
		"moregame.png",
	};
	string tagname[4] = {
		"START",
		"BOSSROOM",
		"OPTION",
		"MOREGAME",
	};
	Vec2 pos[4] = {
		Vec2(1082, 226),
		Vec2(1082, 326),
		Vec2(1082, 426),
		Vec2(1082, 526),
	};

	auto listener = setListener();

	for (int i = 0; i < 4; i++)
	{
		auto spr = Sprite::create(fs[i]);
		spr->setPosition(pos[i].x, visibleSize.height - pos[i].y);
		spr->setName(tagname[i]);
		spr->setTag(i + 1);
		addChild(spr);

		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), spr);
	}
   
	scheduleUpdate();

    return true;
}

void TitleLayer::update(float dt)
{
	int _touchIdx = touchIdx;
	switch (_touchIdx)
	{
	case 1:
	{
		auto scene = GameScene::createScene();
		Director::getInstance()->replaceScene(scene);
	}
		break;
	case 2:
		touchIdx = -1;
		break;
	case 3:
		touchIdx = -1;
		break;
	case 4:
		touchIdx = -1;
		break;
	}
}
EventListenerTouchOneByOne* TitleLayer::setListener()
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
		}
	};

	return listener;
}