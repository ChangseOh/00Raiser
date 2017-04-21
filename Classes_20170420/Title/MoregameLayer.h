#ifndef __MOREGAMELAYER_H__
#define __MOREGAMELAYER_H__

#include "cocos2d.h"
#include "GameManager.h"

class MoregameLayer : public cocos2d::Layer
{
public:
    static MoregameLayer* create();

    virtual bool init();
	virtual void update(float);

	int touchIdx;
};

#endif // __MoregameLayer_SCENE_H__
