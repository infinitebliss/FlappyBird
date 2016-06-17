//
//  Bird.hpp
//  FlappyBird
//
//  Created by Risa Yoneyama on 2016-06-14.
//
//

#ifndef Bird_hpp
#define Bird_hpp

#include <stdio.h>

class Bird : public cocos2d::Node
{
protected:
    Bird();
    ~Bird();
    bool init() override;
    
public:
    void update(float dt);
    CREATE_FUNC(Bird);
    
    // image getter
    cocos2d::Sprite* getImage() { return _image; }
    
private:
    cocos2d::Sprite* _image;
};

#endif /* Bird_hpp */
