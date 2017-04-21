#include "MoregameLayer.h"

USING_NS_CC;


MoregameLayer* MoregameLayer::create()
{
	MoregameLayer* sprite = new MoregameLayer();

	if (sprite->init())
		return sprite;

	CC_SAFE_DELETE(sprite);
	return NULL;
}

// on "init" you need to initialize your instance
bool MoregameLayer::init()
{
    if ( !Layer::init() )
    {
        return false;
    }

	setName("TITLE_LAYER");

	auto clayer = LayerColor::create(Color4B(255, 255, 255, 255));
	addChild(clayer);
   
	scheduleUpdate();

    return true;
}

void MoregameLayer::update(float dt)
{
}