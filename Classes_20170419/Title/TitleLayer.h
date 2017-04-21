#ifndef __TITLELAYER_H__
#define __TITLELAYER_H__

#include "cocos2d.h"
#include "GameManager.h"

USING_NS_CC;
using namespace std;

class TitleLayer : public cocos2d::Layer
{
public:
    static TitleLayer* create();

    virtual bool init();
	virtual void update(float);

	int touchIdx;
	EventListenerTouchOneByOne* setListener();
};

#endif // __TitleLayer_SCENE_H__
