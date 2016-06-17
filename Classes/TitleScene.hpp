//
//  TitleScene.hpp
//  FlappyBird
//
//  Created by Risa Yoneyama on 2016-06-13.
//
//

#ifndef TitleScene_hpp
#define TitleScene_hpp

#include <stdio.h>

class TitleScene :public cocos2d::Layer
{
private:
    TitleScene();
    ~TitleScene();
    virtual bool init();
    
public:
    static cocos2d::Scene* createScene();
    CREATE_FUNC(TitleScene);
};
#endif /* TitleScene_hpp */
