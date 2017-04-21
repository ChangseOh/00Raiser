#include "OptionLayer.h"

USING_NS_CC;


OptionLayer* OptionLayer::create()
{
	OptionLayer* sprite = new OptionLayer();

	if (sprite->init())
		return sprite;

	CC_SAFE_DELETE(sprite);
	return NULL;
}

// on "init" you need to initialize your instance
bool OptionLayer::init()
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

void OptionLayer::update(float dt)
{
}