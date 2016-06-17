//
//  Pipes.hpp
//  FlappyBird
//
//  Created by Risa Yoneyama on 2016-06-14.
//
//

#ifndef Pipes_hpp
#define Pipes_hpp

#include <stdio.h>

class Pipes : public cocos2d::Node
{
protected:
    Pipes();
    ~Pipes();
    bool init() override;
    
public:
    CREATE_FUNC(Pipes);
private:
    cocos2d::Sprite* _pipeTop;
    cocos2d::Sprite* _pipeBottom;
};

#endif /* Pipes_hpp */
