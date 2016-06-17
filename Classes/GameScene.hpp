//
//  GameScene.hpp
//  FlappyBird
//
//  Created by Risa Yoneyama on 2016-06-14.
//
//

#ifndef GameScene_hpp
#define GameScene_hpp

#include "Bird.hpp"

enum class PhysicsCategory
{
    NONE,
    BIRD = 1 << 0,
    PIPE = 1 << 1,
    PIPEDONE = 1 << 2,
    WALL = 1 << 3
};

// game state enum class
enum class GameState
{
    WAITING = 0,
    PLAYING,
    GAMEOVER
};

class GameScene : public cocos2d::Layer
{
private:
    GameScene();
    ~GameScene();
    virtual bool init();
    
public:
    static cocos2d::Scene* createScene();
    void update(float dt);
    CREATE_FUNC(GameScene);
    
    // functions
    void gameSetUp();
    void playSetUp();
    void gameOver();
    
    
private:
    GameState _gameState;
    Bird* _bird;
    cocos2d::Sprite* _getReady;
    cocos2d::Sprite* _greyBird;
    cocos2d::Sprite* _tap;
    int _score;
    cocos2d::Label* _scoreLabel;
    float _pipeAppearFreq;
    float _timePast;
};




#endif /* GameScene_hpp */
