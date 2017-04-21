#pragma execution_character_set("utf-8")
#include "RewardLayer.h"

USING_NS_CC;
using namespace std;

RewardLayer* RewardLayer::create()
{
	RewardLayer* sprite = new RewardLayer();

	if (sprite->init())
		return sprite;

	CC_SAFE_DELETE(sprite);
	return NULL;
}

// on "init" you need to initialize your instance
bool RewardLayer::init()
{
	if (!Layer::init())
	{
		return false;
	}

	elast = 0;
	touchIdx = -1;
	setName("REWARD_LAYER");

	auto bg = Sprite::create("upgradebg.png");
	bg->setPosition(center);
	addChild(bg);

	auto logo = Sprite::create("upgradelogo.png");
	logo->setPosition(203, visibleSize.height - 86);
	addChild(logo);

	auto listener = setListener();
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

	auto exit = Sprite::create("close.png");
	exit->setPosition(1194, visibleSize.height - 74);
	exit->setTag(13);
	addChild(exit);
	_eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), exit);

	auto scrollayer = Layer::create();
	scrollayer->setTag(0);
	addChild(scrollayer);

	string title[9] = {
		"��ǳ�ڻ�",
		"õ�����",
		"ũ��Ƽ�ùڻ�",
		"�нž�ȣ��",
		"����游",
		"�����ڻ�",
		"���Źڻ�",
		"�����ڻ�",
		"����� ����",
	};
	string exp[9] = {
		"�ű� ��Ʈ",
		"��ǳ ������",
		"Ȯ���� ����, ������ ���",
		"��ǳ ���� ������ ������ ���Ÿ� �ش� -> ���� ������ ��ȿ�ð� ���� 945->6000",
		"�ڵ����� �⸦ ��� ��ǳ�� �߻��Ѵ� (����) : 3000 -> 2328750 : ���� �ӵ� ������ �ٲ��",
		"�ڵ����� ��ǳ �߻��ϴ� �н��� ��ȯ",
		"�� �� �߻��ϸ� �������. n�ʰ� 100% Ȯ���� ũ��Ƽ��",
		"�н��� �߻� �ӵ� ����",
		"�������ӽð� ����",
	};
	for (int i = 0; i < 9; i++)
	{
		auto base = Sprite::create("upgradebase.png");
		base->setPosition(229 + 420 * i, visibleSize.height - 404);
		base->setTag(i);
		scrollayer->addChild(base);

		//�׸��̸�
		auto ttle = Label::create();
		ttle->setTextColor(Color4B::BLACK);
		ttle->setSystemFontSize(24);
		ttle->enableOutline(Color4B::WHITE, 2);
		ttle->setPosition(175, 485 - 43);
		ttle->setString(title[i]);
		base->addChild(ttle);

		//����
		auto expl = Label::create();
		expl->setTextColor(Color4B::GRAY);
		expl->setSystemFontSize(24);
		expl->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
		expl->setDimensions(302, 242);
		expl->enableOutline(Color4B::WHITE, 2);
		expl->setPosition(175, 485 - 196);
		expl->setString(exp[i]);
		base->addChild(expl);

		//�׷���
		auto graphbase = Sprite::create("rewardgraphbase.png");
		graphbase->setPosition(175, 485 - 421);
		base->addChild(graphbase);
		auto graphbody = Sprite::create("rewardgraph.png");
		graphbody->setPosition(35, 485 - 421);
		graphbody->setAnchorPoint(Vec2(0.0f, 0.5f));
		graphbody->setName("BODY");
		graphbody->setTextureRect(Rect(0, 0, 1, 55));
		base->addChild(graphbody);

		//���� ��ǥ
		auto price = Label::create();
		price->setTextColor(Color4B::WHITE);
		price->setSystemFontSize(24);
		price->enableOutline(Color4B::GREEN, 2);
		price->setPosition(175, 485 - 421);
		price->setString("00/00");
		price->setName("LABEL");
		base->addChild(price);

		//�ޱ��ư
		auto btn = Sprite::create("recievebutton.png");
		btn->setPosition(175, 485 - 421);
		btn->setName("BUTTON");
		btn->setTag(14);
		btn->setVisible(false);
		base->addChild(btn);
		//175, 421

		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), btn);
	}

	scheduleUpdate();

	return true;
}

void RewardLayer::update(float dt)
{
	if (elast != 0)
	{
		float posx = getChildByTag(0)->getPositionX();
		posx -= elast;
		if (posx > 0)
			posx = 0;
		if (posx < -2520)
			posx = -2520;
		getChildByTag(0)->setPositionX(posx);
		if (elast > 0)
		{
			elast -= 0.5f;
			if (elast < 0)
				elast = 0;
		}
		else
			if(elast < 0)
			{
				elast += 0.5f;
				if (elast > 0)
					elast = 0;
			}
	}

	for (int i = 0; i < 9;i++)
	{
		float percent = (float)(scorereward[i] * 100 / nextreward[i]) * 0.01f;
		if (percent < 0)
			percent = 0;
		if (percent > 1)
			percent = 1;
		getChildByTag(0)->getChildByTag(i)->getChildByName("BUTTON")->setVisible(scorereward[i] >= nextreward[i]);
		((Label*)getChildByTag(0)->getChildByTag(i)->getChildByName("LABEL"))->setString(numComma(scorereward[i]) + "/" + numComma(nextreward[i]));
		((Sprite*)getChildByTag(0)->getChildByTag(i)->getChildByName("BODY"))->setTextureRect(Rect(0, 0, 281 * percent, 55));
	}


	int _touchIdx = touchIdx;
	switch (_touchIdx)
	{
	case 0:
		touchIdx = -1;
		break;
	case 1:
		touchIdx = -1;
		break;
	case 2:
		touchIdx = -1;
		break;
	case 3:
		touchIdx = -1;
		break;
	case 13:
	{
		getParent()->getChildByName("GAME_LAYER")->setLocalZOrder(1);
		this->setLocalZOrder(0);
		touchIdx = -1;
	}
	break;
	//case 2:
	//	touchIdx = -1;
	//	break;
	}
}
EventListenerTouchOneByOne* RewardLayer::setListener()
{

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [=](Touch* touch, Event* event) {
		auto target = event->getCurrentTarget();
		auto loc = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		if (!target->isVisible())
			return false;

		if (rect.containsPoint(loc))
		{
			elast = 0;
			startPos = loc;
			target->setColor(Color3B::GRAY);
			return true;
		}
	};
	listener->onTouchMoved = [=](Touch* touch, Event* event) {
		auto target = event->getCurrentTarget();
		auto loc = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		if (rect.containsPoint(loc))
		{
			float posx = getChildByTag(0)->getPositionX();
			elast = (startPos.x - loc.x);
			posx -= elast;
			if (posx > 0)
				posx = 0;
			if (posx < -2520)
				posx = -2520;
			getChildByTag(0)->setPositionX(posx);
			startPos = loc;
		}
		else
		{
			target->setColor(Color3B::WHITE);
		}
	};
	listener->onTouchEnded = [=](Touch* touch, Event* event) {
		auto target = event->getCurrentTarget();
		auto loc = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		if (rect.containsPoint(loc))
		{
			target->setColor(Color3B::WHITE);
			touchIdx = target->getTag();
			if (touchIdx == 14)
			{
				touchIdx = target->getParent()->getTag();
			}
		}
		else
			touchIdx = -1;
	};

	return listener;
}
