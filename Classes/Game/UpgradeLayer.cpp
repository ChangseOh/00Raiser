#pragma execution_character_set("utf-8")
#include "UpgradeLayer.h"

USING_NS_CC;
using namespace std;

UpgradeLayer* UpgradeLayer::create()
{
	UpgradeLayer* sprite = new UpgradeLayer();

	if (sprite->init())
		return sprite;

	CC_SAFE_DELETE(sprite);
	return NULL;
}

// on "init" you need to initialize your instance
bool UpgradeLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	elast = 0;
	touchIdx = -1;
	setName("UPGRADE_LAYER");

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

	string title[12] = {
		"하트강화",
		"멘탈단련",
		"크리티컬",
		"요술열매",
		"기력충전",
		"분신술",
		"각성",
		"분신강화",
		"각성강화",
		"거름주기",
		"약초꾼",
		"농축과즙",
	};
	string exp[12] = {
		"신규 하트",
		"장풍 레벨업",
		"확률은 고정, 데미지 상승",
		"장풍 맞은 나무가 가끔씩 열매를 준다 -> 이후 열매의 유효시간 증가 945->6000",
		"자동으로 기를 모아 장풍을 발사한다 (오토) : 3000 -> 2328750 : 이후 속도 증가로 바뀐다",
		"자동으로 장풍 발사하는 분신을 소환",
		"몇 번 발사하면 각성모드. n초간 100% 확률로 크리티컬",
		"분신의 발사 속도 증가",
		"각성지속시간 증가",
		"나무 성장. 드롭량 15% 증가",
		"약초꾼이 나뭇잎을 따온다",
		"리워드 광고. 15초간 각성모드 & 위력 3배",
	};
/*
요술열매 - 장풍 맞은 나무가 가끔씩 열매를 준다 -> 이후 열매의 유효시간 증가 945->6000
기력충전 - 자동으로 기를 모아 장풍을 발사한다 (오토) : 3000 -> 2328750 : 이후 속도 증가로 바뀐다
분신술 - 자동으로 장풍 발사하는 분신을 소환
각성 - 몇 번 발사하면 각성모드. n초간 100% 확률로 크리티컬
분신강화 - 분신의 발사 속도 증가
각성강화 - 각성지속시간 증가
거름주기 - 나무 성장. 드롭량 15% 증가
약초꾼 - 약초꾼이 나뭇잎을 따온다
농축과즙 - 리워드 광고. 15초간 각성모드 & 위력 3배
*/
	for (int i = 0; i < 12; i++)
	{
		auto base = Sprite::create("upgradebase.png");
		base->setPosition(229 + 420 * i, visibleSize.height - 404);
		base->setTag(i);
		scrollayer->addChild(base);

		//항목이름
		auto ttle = Label::create();
		ttle->setTextColor(Color4B::BLACK);
		ttle->setSystemFontSize(24);
		ttle->enableOutline(Color4B::WHITE, 2);
		ttle->setPosition(175, 485 - 43);
		ttle->setString(title[i]);
		base->addChild(ttle);

		//설명
		auto expl = Label::create();
		expl->setTextColor(Color4B::GRAY);
		expl->setSystemFontSize(24);
		expl->setAlignment(TextHAlignment::CENTER, TextVAlignment::TOP);
		expl->setDimensions(302, 242);
		expl->enableOutline(Color4B::WHITE, 2);
		expl->setPosition(175, 485 - 196);
		expl->setName("EXP");
		expl->setString(exp[i]);
		base->addChild(expl);

		//가격
		auto price = Label::create();
		price->setTextColor(Color4B(255, 200, 128, 255));
		price->setSystemFontSize(24);
		price->enableOutline(Color4B::WHITE, 2);
		price->setPosition(175, 485 - 350);
		price->setString(numComma(nextupgrade[i]));
		price->setName("PRICE");
		base->addChild(price);

		//버튼
		auto btn = Sprite::create("upgradebutton.png");
		btn->setPosition(175, 485 - 421);
		btn->setName("BUTTON");
		btn->setTag(14);
		base->addChild(btn);
		//175, 421

		_eventDispatcher->addEventListenerWithSceneGraphPriority(listener->clone(), btn);
	}

	scheduleUpdate();

    return true;
}

void UpgradeLayer::update(float dt)
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
			if (elast < 0)
			{
				elast += 0.5f;
				if (elast > 0)
					elast = 0;
			}
	}

	for (int i = 0; i < 12; i++)
	{
		bool buttonOn = (gold >= nextupgrade[i]);
		getChildByTag(0)->getChildByTag(i)->getChildByName("BUTTON")->setColor(buttonOn ? Color3B::WHITE : Color3B::GRAY);
		getChildByTag(0)->getChildByTag(i)->getChildByName("BUTTON")->setVisible(upgradelevel[i] < upgrademax[i]);
	}



	int _touchIdx = touchIdx;
	//Vec2 _touchPos = touchPos;
	switch (_touchIdx)
	{
		/*
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		"",
		*/
	default:
		if (0 <= _touchIdx && _touchIdx <= 11)
		{
			scorereward[_touchIdx]++;
			upgradelevel[_touchIdx]++;
			gold -= nextupgrade[_touchIdx];
			nextupgrade[_touchIdx] += nextupgrade[_touchIdx] * upgraderate[_touchIdx];
			((Label*)getChildByTag(0)->getChildByTag(_touchIdx)->getChildByName("PRICE"))->setString(numComma(nextupgrade[_touchIdx]));
			makeData();
			touchIdx = -1;
		}
		break;
	case 0://하트강화
		scorereward[_touchIdx]++;
		gold -= nextupgrade[_touchIdx];
		weapon++;
		nextupgrade[0] += nextupgrade[0] * 3;
		((Label*)getChildByTag(0)->getChildByTag(_touchIdx)->getChildByName("PRICE"))->setString(numComma(nextupgrade[_touchIdx]));
		touchIdx = -1;
		break;
	case 1://멘탈단련
		scorereward[_touchIdx]++;
		gold -= nextupgrade[_touchIdx];
		level++;
		nextupgrade[1] += nextupgrade[1] * 1;
		((Label*)getChildByTag(0)->getChildByTag(_touchIdx)->getChildByName("PRICE"))->setString(numComma(nextupgrade[_touchIdx]));
		touchIdx = -1;
		break;
	case 2://크리티컬
		scorereward[_touchIdx]++;
		gold -= nextupgrade[_touchIdx];
		critical += 0.1f;
		nextupgrade[2] += nextupgrade[2] * 1;
		((Label*)getChildByTag(0)->getChildByTag(_touchIdx)->getChildByName("PRICE"))->setString(numComma(nextupgrade[_touchIdx]));
		touchIdx = -1;
		break;
	case 3://요술열매
		gold -= nextupgrade[_touchIdx];
		fruitsTime += 0.5f;
		nextupgrade[_touchIdx] += nextupgrade[_touchIdx] * 2;
		((Label*)getChildByTag(0)->getChildByTag(_touchIdx)->getChildByName("PRICE"))->setString(numComma(nextupgrade[_touchIdx]));
		touchIdx = -1;
		break;
	case 4://기력충전
		gold -= nextupgrade[_touchIdx];
		if(autoTime==0)
			autoTime = 5.0f;
		else
			autoTime -= 0.05f;
		nextupgrade[_touchIdx] += nextupgrade[_touchIdx] * 2;
		((Label*)getChildByTag(0)->getChildByTag(_touchIdx)->getChildByName("PRICE"))->setString(numComma(nextupgrade[_touchIdx]));
		touchIdx = -1;
		break;
	case 5://분신술
		gold -= nextupgrade[_touchIdx];
		if (multiple == 0)
			multiple = 5.0f;
		else
			multiple -= 0.05f;
		nextupgrade[_touchIdx] += nextupgrade[_touchIdx] * 2;
		((Label*)getChildByTag(0)->getChildByTag(_touchIdx)->getChildByName("PRICE"))->setString(numComma(nextupgrade[_touchIdx]));
		touchIdx = -1;
		break;
	case 6://각성
		gold -= nextupgrade[_touchIdx];
		if (wakeup == 0)
			wakeup = 750;
		else
			wakeup -= 50;
		nextupgrade[_touchIdx] += nextupgrade[_touchIdx] * 4;
		((Label*)getChildByTag(0)->getChildByTag(_touchIdx)->getChildByName("PRICE"))->setString(numComma(nextupgrade[_touchIdx]));
		touchIdx = -1;
		break;
	case 7://분신강화
		gold -= nextupgrade[_touchIdx];
		multiplepower += 0.02f;
		nextupgrade[_touchIdx] += nextupgrade[_touchIdx] * 2;
		((Label*)getChildByTag(0)->getChildByTag(_touchIdx)->getChildByName("PRICE"))->setString(numComma(nextupgrade[_touchIdx]));
		touchIdx = -1;
		break;
	case 8://각성강화
		gold -= nextupgrade[_touchIdx];
		wakeuppower += 0.02f;
		nextupgrade[_touchIdx] += nextupgrade[_touchIdx] * 2;
		((Label*)getChildByTag(0)->getChildByTag(_touchIdx)->getChildByName("PRICE"))->setString(numComma(nextupgrade[_touchIdx]));
		touchIdx = -1;
		break;
	case 9://거름주기
		gold -= nextupgrade[_touchIdx];
		dropBonus += 0.02f;
		nextupgrade[_touchIdx] += nextupgrade[_touchIdx] * 2;
		((Label*)getChildByTag(0)->getChildByTag(_touchIdx)->getChildByName("PRICE"))->setString(numComma(nextupgrade[_touchIdx]));
		touchIdx = -1;
		break;
	case 10://약초꾼
		//gold -= nextupgrade[_touchIdx];
		//dropBonus += 0.02f;
		//nextupgrade[_touchIdx] += nextupgrade[_touchIdx] * 3;
		//((Label*)getChildByTag(0)->getChildByTag(_touchIdx)->getChildByName("PRICE"))->setString(numComma(nextupgrade[_touchIdx]));
		touchIdx = -1;
		break;
	case 11://농축과즙
		java_VIEWAD();
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
EventListenerTouchOneByOne* UpgradeLayer::setListener()
{

	auto listener = EventListenerTouchOneByOne::create();
	listener->setSwallowTouches(true);
	listener->onTouchBegan = [=](Touch* touch, Event* event) {
		auto target = event->getCurrentTarget();
		auto loc = target->convertToNodeSpace(touch->getLocation());
		Size s = target->getContentSize();
		Rect rect = Rect(0, 0, s.width, s.height);
		if (rect.containsPoint(loc))
		{
			elast = 0;
			startPos = touch->getLocation();
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
			elast = (startPos.x - touch->getLocation().x);
			float posx = getChildByTag(0)->getPositionX();
			posx -= elast;
			if (posx > 0)
				posx = 0;
			if (posx < -3780)
				posx = -3780;
			getChildByTag(0)->setPositionX(posx);
			startPos = touch->getLocation();
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
		CCLOG("%s %d", target->getName().c_str(), target->getTag());
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
