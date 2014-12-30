#include "GameScene.h"
#include "GameLayer.h"
#include "BackgroundLayer.h"
#include "GameSelectLayer.h"
#include "MainMenuLayer.h"
#include "HelpLayer.h"

USING_NS_CC;

GameScene* GameScene::m_instance = NULL;

GameScene::GameScene()
    : m_gameLayer(NULL)
    , m_gameSelectLayer(NULL)
    , m_mainMenuLayer(NULL)
    , m_helpLayer(NULL)
{
    m_scene = CCScene::create();
    CC_ASSERT(m_scene != NULL);

    BackGroundLayer* bgLayer = BackGroundLayer::create();
    CC_ASSERT(bgLayer != NULL);
    m_scene->addChild(bgLayer, 10);

    m_gameSelectLayer = GameSelectLayer::create();
    CC_ASSERT(m_gameSelectLayer != NULL);
    m_gameSelectLayer->retain();

    m_gameLayer = GameLayer::create();
    CC_ASSERT(m_gameLayer != NULL);
    m_gameLayer->retain();

    m_mainMenuLayer = MainMenuLayer::create();
    CC_ASSERT(m_mainMenuLayer != NULL);
    m_mainMenuLayer->retain();
    m_scene->addChild(m_mainMenuLayer, 100);

    m_helpLayer = HelpLayer::create();
    CC_ASSERT(m_helpLayer != NULL);
    m_helpLayer->retain();
}

GameScene::~GameScene()
{
    CC_SAFE_RELEASE(m_gameLayer);
    CC_SAFE_RELEASE(m_gameSelectLayer);
    CC_SAFE_RELEASE(m_mainMenuLayer);
    CC_SAFE_RELEASE(m_helpLayer);
}

GameScene* GameScene::instance()
{
    if (!m_instance)
    {
        m_instance = new GameScene();
    }
    return m_instance;
}

void GameScene::startGame(int level)
{
    CCUserDefault::sharedUserDefault()->setStringForKey("save", "");
    CCUserDefault::sharedUserDefault()->setStringForKey("save_step","0");

    CC_ASSERT(level > 0 && level <= 15);
    m_scene->removeChild(m_gameSelectLayer);

    m_gameLayer->startGame(level);

    m_scene->addChild(m_gameLayer, 200);
}

void GameScene::startNextGame(int level)
{
    CCUserDefault::sharedUserDefault()->setStringForKey("save", "");
    CCUserDefault::sharedUserDefault()->setStringForKey("save_step","0");

    CC_ASSERT(level > 0 && level <= 19);

    m_gameLayer->startGame(level);
}

void GameScene::showGameSelectLayer()
{
    m_scene->removeChild(m_gameLayer);
    m_scene->addChild(m_gameSelectLayer, 200);
}

void GameScene::startNewGame()
{
    CCUserDefault::sharedUserDefault()->setStringForKey("save", "");
    CCUserDefault::sharedUserDefault()->setStringForKey("save_step","0");

    m_scene->removeChild(m_mainMenuLayer);
    m_scene->addChild(m_gameSelectLayer, 200);
}

void GameScene::resumeGame()
{
    m_scene->removeChild(m_mainMenuLayer);
    m_gameLayer->resumeGame();
    m_scene->addChild(m_gameLayer, 200);
}

void GameScene::showHelp()
{
    m_scene->removeChild(m_mainMenuLayer);
    m_scene->addChild(m_helpLayer, 300);
}

void GameScene::hideHelp()
{
    m_scene->removeChild(m_helpLayer);
    m_scene->addChild(m_mainMenuLayer, 100);
}
