#ifndef __GAMEMASTER_H__
#define __GAMEMASTER_H__

#include "cocos2d.h"
#include "GameManager.h"

class GameMaster : public cocos2d::Layer
{
public:
    static GameMaster* create();

    virtual bool init();
	virtual void update(float);

};

#endif // __GameMaster_SCENE_H__
