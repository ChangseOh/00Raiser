#pragma execution_character_set("utf-8")
#include "AppDelegate.h"
#include "Title\TitleScene.h"
#include "GameManager.h"

USING_NS_CC;

static cocos2d::Size designResolutionSize = cocos2d::Size(1280, 720);
static cocos2d::Size smallResolutionSize = cocos2d::Size(1280, 720);
static cocos2d::Size mediumResolutionSize = cocos2d::Size(1280, 720);
static cocos2d::Size largeResolutionSize = cocos2d::Size(1280, 720);

AppDelegate::AppDelegate()
{
	//std::string candidate[7] = {
	//	"",
	//	"Danny Kim",
	//	"안재환",
	//	"Google사용자",
	//	"암흑대마왕",
	//	"Mini Choi",
	//	"남기율"
	//};

	//int drawNum = random(1, 6);
	//CCLOG("Congratulation! %d %s", drawNum, candidate[drawNum]);
	//CCLOG("-");
}

AppDelegate::~AppDelegate() 
{
}

// if you want a different context, modify the value of glContextAttrs
// it will affect all platforms
void AppDelegate::initGLContextAttrs()
{
    // set OpenGL context attributes: red,green,blue,alpha,depth,stencil
    GLContextAttrs glContextAttrs = {8, 8, 8, 8, 24, 8};

    GLView::setGLContextAttrs(glContextAttrs);
}

// if you want to use the package manager to install more packages,  
// don't modify or remove this function
static int register_all_packages()
{
    return 0; //flag for packages manager
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    auto director = Director::getInstance();
    auto glview = director->getOpenGLView();
    if(!glview) {
#if (CC_TARGET_PLATFORM == CC_PLATFORM_WIN32) || (CC_TARGET_PLATFORM == CC_PLATFORM_MAC) || (CC_TARGET_PLATFORM == CC_PLATFORM_LINUX)
        glview = GLViewImpl::createWithRect("RaiseGame", cocos2d::Rect(0, 0, designResolutionSize.width, designResolutionSize.height));
#else
        glview = GLViewImpl::create("RaiseGame");
#endif
        director->setOpenGLView(glview);
    }

    // turn on display FPS
    director->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    director->setAnimationInterval(1.0f / 60);

    // Set the design resolution
    glview->setDesignResolutionSize(designResolutionSize.width, designResolutionSize.height, ResolutionPolicy::NO_BORDER);
    auto frameSize = glview->getFrameSize();
    // if the frame's height is larger than the height of medium size.
    if (frameSize.height > mediumResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(largeResolutionSize.height/designResolutionSize.height, largeResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is larger than the height of small size.
    else if (frameSize.height > smallResolutionSize.height)
    {        
        director->setContentScaleFactor(MIN(mediumResolutionSize.height/designResolutionSize.height, mediumResolutionSize.width/designResolutionSize.width));
    }
    // if the frame's height is smaller than the height of medium size.
    else
    {        
        director->setContentScaleFactor(MIN(smallResolutionSize.height/designResolutionSize.height, smallResolutionSize.width/designResolutionSize.width));
    }

    register_all_packages();

	visibleSize = Size(1280, 720);
	center = visibleSize * 0.5f;

	{
		gold = 0;
		weapon = 0;
		level = 0;

		hasNew = false;
		hasUpgrade = false;
		hasReward = false;

		for (int i = 0; i < 12; i++)
			nextupgrade[i] = 10 * i;
		nextupgrade[0] = 100;
		for (int i = 0; i < 9; i++)
		{
			scorereward[i] = 0;
			nextreward[i] = 10;
		}

		int max[11] = {
			100, 100, 100, 11, 100,
			11, 11, 100, 100, 100,
			11
		};
		int bai[11] = {
			3, 1, 1, 2, 2,
			2, 4, 2, 2, 2, 3
		};
		for (int i = 0; i < 11; i++)
		{
			upgrademax[i] = max[i];
			upgraderate[i] = bai[i];
		}

		makeData();
	}

	auto scheduler = Director::getInstance()->getScheduler();
	scheduler->performFunctionInCocosThread([](void)->void {

		CCLOG("game master");
	});

    // create a scene. it's an autorelease object
    auto scene = TitleScene::createScene();

    // run
    director->runWithScene(scene);

    return true;
}
void AppDelegate::gameMaster(float dt)
{
	CCLOG("game master");
}
// This function will be called when the app is inactive. Note, when receiving a phone call it is invoked.
void AppDelegate::applicationDidEnterBackground() {
    Director::getInstance()->stopAnimation();

    // if you use SimpleAudioEngine, it must be paused
    // SimpleAudioEngine::getInstance()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    Director::getInstance()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::getInstance()->resumeBackgroundMusic();
}
