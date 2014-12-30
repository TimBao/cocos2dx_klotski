#include "AppDelegate.h"
#include "GameScene.h"
#include <string>
#include "SimpleAudioEngine.h"

USING_NS_CC;

static const int TOTAL=19;
const std::string levels[TOTAL]={
    "V00H01V03S11S12V20Q21V23S40S43",
    "S00H01S03V10Q11V13V30S31S32V33",
    "V00H01S03V20Q11V13S31S41V32S33",
    "H01S10S20Q11S13S23V30V31V32V33",
    "S00S10Q01S03S13V20H21V23V31V32",
    "V00Q01V03V20H21V23S40S31S32S43",
    "V00Q01V03S20H21S23V30S31S32V33",
    "V00Q01V03S20S21S22S23V30H31V33",
    "S00Q01S03V10H21V13V30S31S32V33",
    "Q00V02V03H20S22S23V30V31S32S33",
    "V00Q01V03S30V21V22S33S40H41S43",
    "Q00H02V12S13S23V20V21S32S33H41",
    "S00S10Q01S03S13V20V21H22H32H41",
    "V00Q01V03S20S30V21S23S33H40H42",
    "S00S10V01V02V03H20S22S23H30Q32",
    "Q00H02H12V20V21H22S40S32S33S43",
    "V00Q01V03H20H22S30H31S33S40S43",
    "S00Q01S03V10H21V13S30H31S33H41",
    "S00Q01S03S10S13H20H22H30H32H41"
};

AppDelegate::AppDelegate() {

}

AppDelegate::~AppDelegate() 
{
}

bool AppDelegate::applicationDidFinishLaunching() {
    // initialize director
    CCDirector* pDirector = CCDirector::sharedDirector();
    CCEGLView* pEGLView = CCEGLView::sharedOpenGLView();

    pDirector->setOpenGLView(pEGLView);

    CCSize designSize = CCSizeMake(720, 1280);
    pEGLView->setDesignResolutionSize(designSize.width, designSize.height,  kResolutionShowAll);

    std::vector<std::string> searchPaths;
    searchPaths.push_back("fonts");
    searchPaths.push_back("images");
    searchPaths.push_back("sounds");
    CCFileUtils::sharedFileUtils()->setSearchPaths(searchPaths);

    CCSpriteFrameCache* cacher = CCSpriteFrameCache::sharedSpriteFrameCache();
    cacher->addSpriteFramesWithFile("Klotski0.plist");

    if (CCUserDefault::sharedUserDefault()->getIntegerForKey("total", 0) ==0)
    {
        CCUserDefault::sharedUserDefault()->setIntegerForKey("total", TOTAL);
        for (int i = 1; i <= TOTAL; ++i)
        {
            CCUserDefault::sharedUserDefault()->setStringForKey(CCString::createWithFormat("level_%d",i)->getCString(), levels[i-1]);
            CCUserDefault::sharedUserDefault()->setIntegerForKey(CCString::createWithFormat("score_%d",i)->getCString(), 0);
        }
    }

    //CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("win.ogg");
    //CocosDenshion::SimpleAudioEngine::sharedEngine()->preloadEffect("bg.ogg");

    // turn on display FPS
    //pDirector->setDisplayStats(true);

    // set FPS. the default value is 1.0/60 if you don't call this
    pDirector->setAnimationInterval(1.0 / 60);

    //// create a scene. it's an autorelease object
    CCScene *pScene = GameScene::scene();

    // run
    pDirector->runWithScene(pScene);

    return true;
}

// This function will be called when the app is inactive. When comes a phone call,it's be invoked too
void AppDelegate::applicationDidEnterBackground() {
    CCDirector::sharedDirector()->stopAnimation();

    // if you use SimpleAudioEngine, it must be pause
    // SimpleAudioEngine::sharedEngine()->pauseBackgroundMusic();
}

// this function will be called when the app is active again
void AppDelegate::applicationWillEnterForeground() {
    CCDirector::sharedDirector()->startAnimation();

    // if you use SimpleAudioEngine, it must resume here
    // SimpleAudioEngine::sharedEngine()->resumeBackgroundMusic();
}
