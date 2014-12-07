//
//  EvilSprite.h
//  TheBathhouseWars
//
//  Created by Wendell on 14/12/6.
//
//

#ifndef __BathhouseWars__EvilSprite__
#define __BathhouseWars__EvilSprite__

#include <cocos2d.h>
using namespace cocos2d;

class EvilSprite : public Sprite
{
public:
    static EvilSprite* create(int index);
    
    virtual bool init(int index);
    
    void beAttacked();
    
private:
    int _index;
    
    
};

#endif /* defined(__BathhouseWars__EvilSprite__) */
