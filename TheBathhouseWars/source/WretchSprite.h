//
//  WretchSprite.h
//  TheBathhouseWars
//
//  Created by Wendell on 14/12/6.
//
//

#ifndef __BathhouseWars__WretchSprite__
#define __BathhouseWars__WretchSprite__

#include <cocos2d.h>
using namespace cocos2d;

class WretchSprite : public Sprite
{
public:
    CREATE_FUNC(WretchSprite);
    
    WretchSprite();
    virtual bool init();
    
private:
    
};

#endif /* defined(__BathhouseWars__WretchSprite__) */
