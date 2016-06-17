//
//  TitleScene.cpp
//  FlappyBird
//
//  Created by Risa Yoneyama on 2016-06-13.
//
//

#include "TitleScene.hpp"

#include "GameScene.hpp"
#include "Bird.hpp"

USING_NS_CC;

TitleScene::TitleScene()
{
    
}

TitleScene::~TitleScene()
{
    
}


Scene* TitleScene::createScene()
{
    Scene* scene = Scene::create();
    Layer* layer = TitleScene::create();
    scene->addChild(layer);
    return scene;
}

bool TitleScene::init()
{
    if(!Layer::init()){
        return false;
    }
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
    this->addChild(ground1);
    
    Sprite* ground2 = Sprite::create("ground.png");
    ground2->setAnchorPoint(Vec2(0, 0));
    ground2->setPosition(Vec2(winSize.width - 20, 0));
    this->addChild(ground2);
    
    // animation of ground
    MoveBy* moveout = MoveBy::create(4.0, Vec2(-ground1->getContentSize().width, 0));
    MoveBy* moveleft1 = MoveBy::create(8.0, Vec2(-ground2->getContentSize().width * 2, 0));
    MoveTo* moveback1 = MoveTo::create(0, Vec2(winSize.width, 0));
    Action* repeat = Repeat::create(Sequence::create(moveback1, moveleft1, NULL), -1);
    ground1->runAction(Sequence::create(moveout, repeat, NULL));
    MoveBy* moveleft2 = MoveBy::create(8.0, Vec2(-ground2->getContentSize().width * 2, 0));
    MoveTo* moveback2 = MoveTo::create(0, Vec2(winSize.width, 0));
    ground2->runAction(RepeatForever::create(Sequence::create(moveback2, moveleft2, NULL)));
    
    // create title image
    Sprite* title = Sprite::create("title.png");
    title->setPosition(Vec2(winSize.width/ 2.0, winSize.height - 300));
    title->setScale(0.4);
    this->addChild(title);
    
    // create the bird
    Sprite* bird = Sprite::create("bird.png");
    
    // flying animation of bird
    Size frameSize = Size(bird->getContentSize().width / 3, bird->getContentSize().height);
    bird->setTextureRect(Rect(0, 0, frameSize.width, frameSize.height));
    Vector<SpriteFrame *> frames;
    for(int i = 0; i < 3; ++i){
        SpriteFrame* frame;
        frame = SpriteFrame::create("bird.png", Rect(frameSize.width * i, 0, frameSize.width, frameSize.height));
        frames.pushBack(frame);
        
    }
    Animation* animation = Animation::createWithSpriteFrames(frames);
    animation->setDelayPerUnit(0.05);
    bird->runAction(RepeatForever::create(Animate::create(animation)));
    
    // bobbing animation of bird
    MoveBy* moveup = MoveBy::create(0.2, Vec2(0, 40));
    MoveBy* movedown = MoveBy::create(0.2, Vec2(0, -40));
    bird->runAction(RepeatForever::create(Sequence::create(moveup, movedown, NULL)));
    
    bird->setPosition(Vec2(winSize.width / 2.0, winSize.height / 2.0));
    this->addChild(bird);
    
    // play button
    Sprite* play = Sprite::create("play.png");
    play->setPosition(Vec2(winSize.width / 2.0, 400));
    this->addChild(play);

    
    // 画面をタッチした時にメイン画面へ
    auto listener = EventListenerTouchOneByOne::create();
    listener->onTouchBegan = [=](Touch* touch, Event* event){
        Vec2 touchBegan = touch->getLocation();
        // if the play button is pressed
        if(play->getBoundingBox().containsPoint(touchBegan)){
            // 何度も押せないように一度押したらアクションを無効化する
            this->getEventDispatcher()->removeAllEventListeners();
            // 0.5秒待ってからCallFuncを呼ぶ
            auto delay = DelayTime::create(0.5);
            // ゲームを始めるアクション
            auto startGame = CallFunc::create([]{
                auto scene = GameScene::createScene();
                auto transition = TransitionFade::create(0.3f, scene);
                Director::getInstance()->replaceScene(transition);
            });
            
            this->runAction(Sequence::create(delay, startGame, NULL));
        }
        return true;
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);

    
    return true;
}