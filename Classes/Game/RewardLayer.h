#ifndef __REWARDLAYER_H__
#define __REWARDLAYER_H__

#include "cocos2d.h"
#include "GameManager.h"

class RewardLayer : public cocos2d::Layer
{
public:
    static RewardLayer* create();

    virtual bool init();
	virtual void update(float);

	float elast;
	Vec2 startPos;

	Node* touchednode;
	int touchIdx;
	EventListenerTouchOneByOne* setListener();
};

#endif // __RewardLayer_SCENE_H__
