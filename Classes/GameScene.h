#ifndef __GAMESCENE_H__
#define __GAMESCENE_H__

#include "cocos2d.h"

class GameSelectLayer;
class GameLayer;
class MainMenuLayer;
class HelpLayer;

class GameScene
{
public:
    GameScene();
    ~GameScene();
    static GameScene* instance();
    static cocos2d::CCScene* scene() { return instance()->m_scene; }

    void startGame(int level);
    void showGameSelectLayer();
    void startNewGame();
    void startNextGame(int level);
    void resumeGame();
    void showHelp();
    void hideHelp();
private:
    static GameScene* m_instance;
    cocos2d::CCScene* m_scene;
    GameSelectLayer* m_gameSelectLayer;
    GameLayer* m_gameLayer;
    MainMenuLayer* m_mainMenuLayer;
    HelpLayer* m_helpLayer;
};

#endif //__GAMESCENE_H__