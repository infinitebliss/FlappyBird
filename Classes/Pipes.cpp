//
//  Pipes.cpp
//  FlappyBird
//
//  Created by Risa Yoneyama on 2016-06-14.
//
//

#include "Pipes.hpp"

#include "GameScene.hpp"

USING_NS_CC;

static const float SPACE_BETWEEN_PIPES = 300;

Pipes::Pipes()
{
    
}

Pipes::~Pipes()
{
    
}

bool Pipes::init()
{
    if(!Node::init()) return false;
    
    Size winSize = Director::getInstance()->getWinSize();
    
    Size size = Size(90, 1002);
    this->setContentSize(size);
    
    // top pipe
    _pipeTop = Sprite::create("pipe2.png");
    _pipeTop->setAnchorPoint(Vec2(0.5, 0));
    _pipeTop->setScale(0.3);
    this->addChild(_pipeTop);
    
    PhysicsBody* _pipeTopBody = PhysicsBody::createBox(_pipeTop->getContentSize());
    _pipeTopBody->setDynamic(false);
    _pipeTopBody->setCategoryBitmask(static_cast<int>(PhysicsCategory::PIPE));
    _pipeTopBody->setCollisionBitmask(static_cast<int>(PhysicsCategory::BIRD));
    _pipeTopBody->setContactTestBitmask(static_cast<int>(PhysicsCategory::BIRD));
    _pipeTop->setPhysicsBody(_pipeTopBody);
    
    // bottom pipe
    _pipeBottom = Sprite::create("pipe1.png");
    _pipeBottom->setAnchorPoint(Vec2(0.5, 1));
    _pipeBottom->setScale(0.3);
    this->addChild(_pipeBottom);
    
    // choose random place for pipe
    int rand = random(0, 10);
    float height = winSize.height - 800 - (20 * rand);
    _pipeTop->setPosition(Vec2(size.width / 2.0, height + SPACE_BETWEEN_PIPES / 2.0));
    _pipeBottom->setPosition(Vec2(size.width / 2.0, height - SPACE_BETWEEN_PIPES / 2.0));
    
    PhysicsBody* _pipeBottomBody = PhysicsBody::createBox(_pipeBottom->getContentSize());
    _pipeBottomBody->setDynamic(false);
    _pipeBottomBody->setCategoryBitmask(static_cast<int>(PhysicsCategory::PIPE));
    _pipeBottomBody->setCollisionBitmask(static_cast<int>(PhysicsCategory::BIRD));
    _pipeBottomBody->setContactTestBitmask(static_cast<int>(PhysicsCategory::BIRD));
    _pipeBottom->setPhysicsBody(_pipeBottomBody);
    
    // physics body of whole pipe
    PhysicsBody* _pipeBody = PhysicsBody::createBox(size);
    _pipeBody->setDynamic(false);
    _pipeBody->setRotationEnable(false);
    _pipeBody->setCategoryBitmask(static_cast<int>(PhysicsCategory::PIPEDONE));
    _pipeBody->setContactTestBitmask(static_cast<int>(PhysicsCategory::BIRD));
    this->setPhysicsBody(_pipeBody);
    
    return true;
}