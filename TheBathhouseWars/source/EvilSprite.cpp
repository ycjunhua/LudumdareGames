//
//  EvilSprite.cpp
//  TheBathhouseWars
//
//  Created by Wendell on 14/12/6.
//
//

#include "EvilSprite.h"
#include "GameResource.h"

EvilSprite* EvilSprite::create(int index)
{
    auto ret = new (std::nothrow) EvilSprite();
    if (ret && ret->init(index))
    {
        ret->autorelease();
    }
    else
    {
        CC_SAFE_DELETE(ret);
    }
    return ret;
}

bool EvilSprite::init(int index)
{
    bool ret = false;
    _index = index;
    char firistFrameName[20];
    sprintf(firistFrameName, "evil0%d0.png", index);
    if (Sprite::initWithSpriteFrameName(firistFrameName)) {
        auto physicsBody = PhysicsBody::createBox(_contentSize * 0.6f);
        physicsBody->setDynamic(false);
        physicsBody->setContactTestBitmask(0xFFFFFFFF);
        setPhysicsBody(physicsBody);
        setTag(EvilTag);
        
        ret = true;
    }
    
    return ret;
}

void EvilSprite::beAttacked()
{
    setPhysicsBody(nullptr);
    
    auto beAttacked = Sequence::create(FadeOut::create(1.0f), CCCallFunc::create([&](){
        this->removeFromParentAndCleanup(true);
    }),nullptr);
    runAction(beAttacked);
}

