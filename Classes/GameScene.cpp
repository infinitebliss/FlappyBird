//
//  GameScene.cpp
//  FlappyBird
//
//  Created by Risa Yoneyama on 2016-06-14.
//
//

#include "GameScene.hpp"

#include "Bird.hpp"
#include "Pipes.hpp"

#include "ui/CocosGUI.h"

USING_NS_CC;

static const Vec2 GRAVITY_ACCELERATION(0, 7000);
static const Vec2 IMPULSE_VELOCITY(0, -700000);
static const float SCREEN_MIDDLE(501);

GameScene::GameScene()
{
    _gameState = GameState::WAITING;
    _score = 0;
    _timePast = 5;
    _pipeAppearFreq = 3;
    _greyBird = NULL;
    _tap = NULL;
    _getReady = NULL;
}

GameScene::~GameScene()
{
    
}

Scene* GameScene::createScene()
{
    // create scene with physics
    Scene* scene = Scene::createWithPhysics();
    PhysicsWorld* world = scene->getPhysicsWorld();
    world->setGravity(GRAVITY_ACCELERATION);
    
#if COCOS2D_DEBUG > 0
    // world->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
#endif
    
    Layer* layer = GameScene::create();
    scene->addChild(layer);
    
    return scene;
}

bool GameScene::init()
{
    if(!Layer::init())
    {
        return false;
    }
    
    gameSetUp();
    
    // event listener for touch
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [this](Touch* touch, Event* event){
        Size winSize = Director::getInstance()->getWinSize();
        if(_gameState == GameState::WAITING){
            playSetUp();
        }
        if(_gameState == GameState::PLAYING){
            _bird->getPhysicsBody()->setVelocity(Vec2(0, 0));
            _bird->getPhysicsBody()->applyImpulse(IMPULSE_VELOCITY);
        }
        return true;
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    
    auto contactlistener = EventListenerPhysicsContact::create();
    contactlistener->onContactBegin = [this](PhysicsContact& contact)
    {
        // get physics shape
        PhysicsShape* shapeA = contact.getShapeA();
        PhysicsShape* shapeB = contact.getShapeB();
        
        // get the physics body
        PhysicsBody* bodyA = shapeA->getBody();
        PhysicsBody* bodyB = shapeB->getBody();
        
        // get the categories
        int categoryA = bodyA->getCategoryBitmask();
        int categoryB = bodyB->getCategoryBitmask();
        
        
        if((categoryA == static_cast<int>(PhysicsCategory::PIPE) && categoryB == static_cast<int>(PhysicsCategory::BIRD)) || (categoryA == static_cast<int>(PhysicsCategory::BIRD) && categoryB == static_cast<int>(PhysicsCategory::PIPE)))
        {
            if(_gameState == GameState::PLAYING){
                // if the bird touches a pipe or the edge of the screen
                _gameState = GameState::GAMEOVER;
                gameOver();
            }
        }
        
        if((categoryA == static_cast<int>(PhysicsCategory::BIRD) && categoryB == static_cast<int>(PhysicsCategory::WALL)) || (categoryB == static_cast<int>(PhysicsCategory::BIRD) && categoryA == static_cast<int>(PhysicsCategory::WALL)))
        {
            if(_gameState == GameState::PLAYING){
                // if the bird touches a pipe or the edge of the screen
                _gameState = GameState::GAMEOVER;
                gameOver();
            }
        }
        return true;
    };
    contactlistener->onContactSeparate = [this](PhysicsContact& contact)
    {
        // get physics shape
        PhysicsShape* shapeA = contact.getShapeA();
        PhysicsShape* shapeB = contact.getShapeB();
        
        // get the physics body
        PhysicsBody* bodyA = shapeA->getBody();
        PhysicsBody* bodyB = shapeB->getBody();
        
        // get the categories
        int categoryA = bodyA->getCategoryBitmask();
        int categoryB = bodyB->getCategoryBitmask();
        
        if((categoryA == static_cast<int>(PhysicsCategory::PIPEDONE) && categoryB == static_cast<int>(PhysicsCategory::BIRD)) || (categoryA == static_cast<int>(PhysicsCategory::BIRD) && categoryB == static_cast<int>(PhysicsCategory::PIPEDONE)))
        {
            if(_gameState == GameState::PLAYING){
                _score++;
            }
        }
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(contactlistener, this);

    return true;
}

void GameScene::update(float dt)
{
    Size winSize = Director::getInstance()->getWinSize();
    if(_pipeAppearFreq <= _timePast){
        Pipes* pipes = Pipes::create();
        pipes->setPosition(Vec2(winSize.width + pipes->getContentSize().width / 2.0, winSize.height - SCREEN_MIDDLE));
        pipes->setAnchorPoint(Vec2(0.5, 0.5));
        this->addChild(pipes);
        
        MoveBy* moveby = MoveBy::create(4.0, Vec2(-720 -pipes->getContentSize().width, 0));
        pipes->runAction(Sequence::create(moveby, RemoveSelf::create(), NULL));
        _timePast = 0;
    }
    _timePast += dt;
    
    // update score label
    _scoreLabel->setString(StringUtils::toString(_score));
}

void GameScene::gameSetUp()
{
    // size of screen
    Size winSize = Director::getInstance()->getWinSize();
    
    // create background
    Sprite* background = Sprite::create("background.png");
    background->setPosition(Vec2(winSize.width / 2.0, winSize.height / 2.0));
    this->addChild(background);
    
    // create the moving ground
    Sprite* ground1 = Sprite::create("ground.png");
    ground1->setAnchorPoint(Vec2(0, 0));
    ground1->setPosition(Vec2(0, 0));
    this->addChild(ground1, 1);
    
    Sprite* ground2 = Sprite::create("ground.png");
    ground2->setAnchorPoint(Vec2(0, 0));
    ground2->setPosition(Vec2(winSize.width - 20, 0));
    this->addChild(ground2, 1);
    
    // animation of ground
    MoveBy* moveout = MoveBy::create(4.0, Vec2(-ground1->getContentSize().width, 0));
    MoveBy* moveleft1 = MoveBy::create(8.0, Vec2(-ground2->getContentSize().width * 2, 0));
    MoveTo* moveback1 = MoveTo::create(0, Vec2(winSize.width, 0));
    Action* repeat = Repeat::create(Sequence::create(moveback1, moveleft1, NULL), -1);
    ground1->runAction(Sequence::create(moveout, repeat, NULL));
    MoveBy* moveleft2 = MoveBy::create(8.0, Vec2(-ground2->getContentSize().width * 2, 0));
    MoveTo* moveback2 = MoveTo::create(0, Vec2(winSize.width, 0));
    ground2->runAction(RepeatForever::create(Sequence::create(moveback2, moveleft2, NULL)));
    
    // create the walls
    // top wall
    Sprite* topwall = Sprite::create("box.png");
    topwall->setPosition(Vec2(winSize.width / 2.0, winSize.height + winSize.height / 2.0));
    this->addChild(topwall);
    PhysicsBody* topwallbody = PhysicsBody::createBox(topwall->getContentSize());
    topwallbody->setDynamic(false);
    topwallbody->setCategoryBitmask(static_cast<int>(PhysicsCategory::WALL));
    topwallbody->setContactTestBitmask(static_cast<int>(PhysicsCategory::BIRD));
    topwallbody->setCollisionBitmask(static_cast<int>(PhysicsCategory::BIRD));
    topwall->setPhysicsBody(topwallbody);
    // ground
    Sprite* ground = Sprite::create("box.png");
    ground->setPosition(Vec2(winSize.width / 2.0, -winSize.height / 2.0));
    this->addChild(ground);
    PhysicsBody* groundbody = PhysicsBody::createBox(ground->getContentSize());
    groundbody->setDynamic(false);
    groundbody->setCategoryBitmask(static_cast<int>(PhysicsCategory::WALL));
    groundbody->setContactTestBitmask(static_cast<int>(PhysicsCategory::BIRD));
    groundbody->setCollisionBitmask(static_cast<int>(PhysicsCategory::BIRD));
    ground->setPhysicsBody(groundbody);
    
    // Bird
    _bird = Bird::create();
    _bird->setPosition(Vec2(200, winSize.height / 2.0));
    _bird->getPhysicsBody()->setGravityEnable(false);
    this->addChild(_bird, 2);
    
    // bobbing animation
    MoveBy* moveup = MoveBy::create(0.2, Vec2(0, 40));
    MoveBy* movedown = MoveBy::create(0.2, Vec2(0, -40));
    RepeatForever* repeatforever = RepeatForever::create(Sequence::create(moveup, movedown, NULL));
    repeatforever->setTag(1);
    _bird->runAction(repeatforever);
    
    // tap image
    _tap = Sprite::create("tap.png");
    _tap->setPosition(Vec2(winSize.width / 2.0, winSize.height - 700));
    this->addChild(_tap);
    
    // grey image
    _greyBird = Sprite::create("grey_bird.png");
    _greyBird->setPosition(Vec2(winSize.width / 2.0, winSize.height - 550));
    this->addChild(_greyBird);
    
    // get ready
    _getReady = Sprite::create("getready.png");
    _getReady->setPosition(Vec2(winSize.width / 2.0, winSize.height + _getReady->getContentSize().height / 2.0));
    this->addChild(_getReady);
    MoveBy* move = MoveBy::create(0.5, Vec2(0, -300));
    _getReady->runAction(move);
    
    _timePast = 5;
    _score = 0;
 
    this->unscheduleUpdate();
}

void GameScene::playSetUp()
{
    Size winSize = Director::getInstance()->getWinSize();
    // schedule update to start pipes appearing
    this->scheduleUpdate();
    _bird->getPhysicsBody()->setGravityEnable(true);
    _bird->stopActionByTag(1);
    // remove the get ready image
    MoveBy* move = MoveBy::create(0.3, Vec2(0, 300));
    _getReady->runAction(Sequence::create(move, RemoveSelf::create(), NULL));
    _getReady = NULL;
    // remove the grey bird
    if(_greyBird){
        _greyBird->removeFromParent();
        _greyBird = NULL;
    }
    // remove the tap image
    if(_tap){
        _tap->removeFromParent();
        _tap = NULL;
    }
    // score label
    _scoreLabel = Label::createWithSystemFont(StringUtils::toString(_score), "Marker Felt", 80);
    _scoreLabel->setPosition(Vec2(winSize.width / 2.0, winSize.height - 300));
    _scoreLabel->enableShadow();
    this->addChild(_scoreLabel, 3);
    _gameState = GameState::PLAYING;

}

void GameScene::gameOver()
{
    log("game over");
    Size winSize = Director::getInstance()->getWinSize();
    UserDefault* ud = UserDefault::getInstance();
    
    this->unscheduleUpdate();
    
    // remove score label
    if(_scoreLabel){
        _scoreLabel->removeFromParent();
        _scoreLabel = NULL;
    }
    
    // update high score
    int _highScore = ud->getIntegerForKey("high score", 0);
    if(_score > _highScore){
        ud->setIntegerForKey("high score", _score);
        _highScore = _score;
    }
    
    // reset bird contact bitmasks
    PhysicsBody* birdBody = _bird->getPhysicsBody();
    birdBody->setCollisionBitmask(static_cast<int>(PhysicsCategory::NONE));
    birdBody->setContactTestBitmask(static_cast<int>(PhysicsCategory::NONE));
    
    // make the screen dark with black
    Sprite* blackBg = Sprite::create("black_background.png");
    blackBg->setPosition(Vec2(winSize.width / 2.0, winSize.height / 2.0));
    blackBg->setOpacity(0);
    FadeTo* fadeto = FadeTo::create(0.2, 128);
    blackBg->runAction(fadeto);
    this->addChild(blackBg, 3);
    
    // add gameover
    Sprite* gameover = Sprite::create("gameover.png");
    gameover->setPosition(Vec2(winSize.width / 2.0, winSize.height - 300));
    gameover->setScale(1.5);
    this->addChild(gameover, 4);
    
    // final game score
    Sprite* finalscore = Sprite::create("final_score.png");
    finalscore->setPosition(Vec2(winSize.width / 2.0, winSize.height - 700));
    this->addChild(finalscore, 4);
    Label* _scoreLbl = Label::createWithSystemFont(StringUtils::toString(_score), "Marker Felt", 60);
    _scoreLbl->setPosition(Vec2(100, 180));
    finalscore->addChild(_scoreLbl, 4);
    Label* _highscoreLbl = Label::Label::createWithSystemFont(StringUtils::toString(_highScore), "Marker Felt", 60);
    _highscoreLbl->setPosition(Vec2(100, 70));
    finalscore->addChild(_highscoreLbl, 4);


    
    // add replay button
    cocos2d::ui::Button* replaybutton = ui::Button::create("play.png", "play.png", "play.png");
    replaybutton->setPosition(Vec2(winSize.width / 2.0, 300));
    replaybutton->addTouchEventListener([&](Ref* sender, cocos2d::ui::Widget::TouchEventType type){
        switch (type)
        {
            case ui::Widget::TouchEventType::BEGAN:
                break;
            case ui::Widget::TouchEventType::ENDED:
                _gameState = GameState::WAITING;
                this->removeAllChildren();
                gameSetUp();
                break;
            default:
                break;
        }
    });
    this->addChild(replaybutton, 4);
}