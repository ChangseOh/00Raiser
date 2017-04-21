#include "TutorialLayer.h"

USING_NS_CC;


TutorialLayer* TutorialLayer::create()
{
	TutorialLayer* sprite = new TutorialLayer();

	if (sprite->init())
		return sprite;

	CC_SAFE_DELETE(sprite);
	return NULL;
}

// on "init" you need to initialize your instance
bool TutorialLayer::init()
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

void TutorialLayer::update(float dt)
{
}