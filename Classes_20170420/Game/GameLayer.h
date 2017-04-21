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

	float playerAuto;//플레이어 오토
	float multipleAuto;//분신 오토
	float fruitsAuto;//과일 발동
	int awakeLeft;//각성까지
	float awakeAuto;//각성발동

	void attack(Vec2 pos, int kind);
	//void attack2(Vec2 pos, int kind);
};

#endif // __GameLayer_SCENE_H__
