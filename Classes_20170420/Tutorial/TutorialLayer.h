#ifndef __TUTORIALLAYER_H__
#define __TUTORIALLAYER_H__

#include "cocos2d.h"
#include "GameManager.h"

class TutorialLayer : public cocos2d::Layer
{
public:
    static TutorialLayer* create();

    virtual bool init();
	virtual void update(float);

	int touchIdx;
};

#endif // __TutorialLayer_SCENE_H__
