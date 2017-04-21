#ifndef __GAMEMANAGER_H__
#define __GAMEMANAGER_H__

#include "cocos2d.h"
//#include "SimpleAudioEngine.h"
#include "audio/include/AudioEngine.h"

USING_NS_CC;

extern Size visibleSize;
extern Vec2 center;

extern ValueMap strs;
extern int _audioID;

extern bool soundOnOff;
extern tm adtime;

extern int gold;//나뭇잎
extern int fruits;//열매 소지 갯수
extern int potionA;//포션1 소지 갯수
extern int potionB;//포션2 소지 갯수

extern int upgradelevel[11];
extern int upgrademax[11];//최대치
extern int upgraderate[11];//업그레이드시마다 적용되는 증액 배율
extern int weapon;//장풍단계
extern int level;//장풍레벨
extern float critical;//크리티컬 (0이면 크리티컬 없다. 이후 데미지값)
extern float fruitsTime;//열매 0이면 열매 생성 안함. 이후는 열매 유효시간
extern float autoTime;//자동
extern float multiple;//분신. 0이면 없다. 이후는 발사 간격 시간(=발사속도)
extern float multiplepower;//분신 파워
extern int wakeup;//각성. 0이면 각성 조건 없음. 이후는 각성에 필요한 연타 수
extern float wakeuppower;//각성파워
extern float dropBonus;//드롭량 증가
extern float autoquest;//약초꾼


//달성기록
extern int touchNum;//터치 횟수
extern int shootNum;//실제 발사 수
extern int maxItem;//최대 열매 소지 수
extern int viewtimeCm;//리워드 동영상 광고를 시청한 횟수
extern tm playtime;//게임 켜놓은 시간

//공용 플래그
extern bool hasNew;//장풍 단계를 높일 수 있다.
extern bool hasUpgrade;//뭔가 업그레이드 할 수 있는 것이 있다
extern bool hasReward;//보상받을 수 있는 것이 있다.

//업그레이드 목표 수치
extern int nextupgrade[12];
//보상 목표 수치
extern int nextreward[9];
extern int scorereward[9];
void makeData();

Vec2 getDest(Vec2 pos1, int angle, float speed);
int getAngle(Vec2 pos1, Vec2 pos2);
//int needGold(int level);
int getValue(int raw);
std::string numComma(int num);
void playEffect(const char* fname);
void playMusic(const char* fname);
void stopMusic();
void playEffect(int kind);
void preLoadEffect();

void saveGame();
bool loadGame();
void loadOption();
void saveOption();

cocos2d::Sprite* hitEffect(std::string, cocos2d::Vec2);
cocos2d::Sprite* hitEffect(std::string fname, cocos2d::Vec2 pos, float scale);
Vector<Node*> getChildrenByName(Node* node, std::string name);
double getRange(Vec2 pos1, Vec2 pos2);
//std::string codedStr(std::string src);

void makeStrJson();

RenderTexture* getHotShot();
bool getScreenShot();
void java_INITAD();
void java_VIEWAD();
void java_HIDEAD();
void java_VIEWINTERSTIALAD();
void java_ACHIEVE(int idx);
void java_LEADERBOARD(int kind, int score);
void java_MOREGAME(int);
void java_SAVECLOUD();
#endif
