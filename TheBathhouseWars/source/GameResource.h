//
//  GameResource.h
//  TheBathhouseWars
//
//  Created by Wendell on 14/12/6.
//
//

#ifndef __BathhouseWars__GameResource__
#define __BathhouseWars__GameResource__

#include "cocos2d.h"

using namespace cocos2d;

const int EvilTag = 10;
const int WretchTag = 20;
const int FloorTag = 30;
const int SoapTag = 40;

extern int s_waterAudioID;

extern Vec2 s_visibleOrigin;
extern Size s_visibleSize;
extern float s_centreX;
extern float s_centreY;
extern PhysicsWorld* s_physicsWorld;

#endif /* defined(__BathhouseWars__GameResource__) */
