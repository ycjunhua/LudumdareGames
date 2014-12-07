//
//  WretchSprite.cpp
//  TheBathhouseWars
//
//  Created by Wendell on 14/12/6.
//
//

#include "WretchSprite.h"
#include "GameResource.h"

WretchSprite::WretchSprite()
{
    
}

bool WretchSprite::init()
{
    bool ret = false;
    if (Sprite::initWithSpriteFrameName("hand.png")) {
        auto hitBody = PhysicsBody::createCircle(100, PhysicsMaterial(1, 1.2f, 0.0f));
        hitBody->setTag(WretchTag);
        hitBody->setRotationEnable(false);
        hitBody->setContactTestBitmask(0xFFFFFFFF);
        hitBody->setLinearDamping(1.0f);
        setPhysicsBody(hitBody);
        
        setScale(0.45f);
        setTag(WretchTag);
        
        ret = true;
    }
    
    return ret;
}



