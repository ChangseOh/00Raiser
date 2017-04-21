#ifndef __OPTIONLAYER_H__
#define __OPTIONLAYER_H__

#include "cocos2d.h"
#include "GameManager.h"

class OptionLayer : public cocos2d::Layer
{
public:
    static OptionLayer* create();

    virtual bool init();
	virtual void update(float);

	int touchIdx;
};

#endif // __OptionLayer_SCENE_H__
