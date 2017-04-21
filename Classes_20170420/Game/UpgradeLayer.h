#ifndef __UPGRADELAYER_H__
#define __UPGRADELAYER_H__

#include "cocos2d.h"
#include "GameManager.h"

class UpgradeLayer : public cocos2d::Layer
{
public:
    static UpgradeLayer* create();

    virtual bool init();
	virtual void update(float);

	float elast;
	Vec2 startPos;

	Node* touchednode;
	int touchIdx;
	EventListenerTouchOneByOne* setListener();
};

#endif // __UpgradeLayer_SCENE_H__
