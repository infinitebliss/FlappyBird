//
//  Bird.cpp
//  FlappyBird
//
//  Created by Risa Yoneyama on 2016-06-14.
//
//

#include "Bird.hpp"

#include "GameScene.hpp"

static const int FRAME_COUNT = 3;

USING_NS_CC;

Bird::Bird()
{
    
}

Bird::~Bird()
{
    
}

bool Bird::init()
{
    if(!Node::init()) return false;
    
    // image settings
    _image = Sprite::create("bird.png");
    // animation of bird
    Size frameSize = Size(_image->getContentSize().width / FRAME_COUNT, _image->getContentSize().height);
    _image->setTextureRect(Rect(0, 0, frameSize.width, frameSize.height));
    Vector<SpriteFrame *> frames;
    for(int i = 0; i< FRAME_COUNT; ++i){
        SpriteFrame* frame;
        frame = SpriteFrame::create("bird.png", Rect(frameSize.width * i, 0, frameSize.width, frameSize.height));
        frames.pushBack(frame);
        
    }
    Animation* animation = Animation::createWithSpriteFrames(frames);
    animation->setDelayPerUnit(0.05);
    _image->runAction(RepeatForever::create(Animate::create(animation)));
    float scaleFactor = 0.8;
    _image->setScale(scaleFactor);
    this->addChild(_image);
    
    // physics body of bird
    PhysicsBody* body = PhysicsBody::createCircle(_image->getContentSize().width * 0.4);
    body->setDynamic(true);
    body->setRotationEnable(false);
    body->setCategoryBitmask(static_cast<int>(PhysicsCategory::BIRD));
    body->setContactTestBitmask(static_cast<int>(PhysicsCategory::PIPE) | static_cast<int>(PhysicsCategory::WALL) | static_cast<int>(PhysicsCategory::PIPEDONE));
    body->setCollisionBitmask(static_cast<int>(PhysicsCategory::PIPE));
    body->setVelocityLimit(100000);
    this->setPhysicsBody(body);
    
    this->setContentSize(_image->getContentSize() * scaleFactor);
    
    this->scheduleUpdate();
    
    return true;
}

void Bird::update(float dt)
{
    Vec2 v = this->getPhysicsBody()->getVelocity();
    CCLOG("x: %f, y: %f", v.x, v.y);
    if(0 < v.y)
    {
        if(v.y < 5000){
          this->setRotation(-20);
        }
        else if(v.y < 10000){
            this->setRotation(-30);
        }
        else{
            this->setRotation(-45);
        }
    }
    else if(v.y == 0){
        this->setRotation(0);
    }
    else if(v.y < 0){
        if(-50 < v.y){
            this->setRotation(20);
        }
        else if(-100 < v.y){
            this->setRotation(30);
        }
        else{
            this->setRotation(45);
        }
    }
}