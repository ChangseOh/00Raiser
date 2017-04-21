#ifndef __GAMELAYER_H__
#define __GAMELAYER_H__

#include "cocos2d.h"
#include "GameManager.h"

class GameLayer : public cocos2d::Layer
{
public:
    static GameLayer* create();

    virtual bool init();
	virtual void update(float);

	Vec2 touchPos;
	int touchIdx;
	EventListenerTouchOneByOne* setListener();

	void eventMake(bool cri);
};

#endif // __GameLayer_SCENE_H__
