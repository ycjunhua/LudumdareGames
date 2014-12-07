//
//  GameLayer.h
//  TheBathhouseWars
//
//  Created by Wendell on 14/12/6.
//
//

#ifndef __BathhouseWars__GameLayer__
#define __BathhouseWars__GameLayer__

#include <cocos2d.h>

using namespace cocos2d;

class EvilSprite;
class WretchSprite;

class GameLayer : public Layer
{
public:
    CREATE_FUNC(GameLayer);
    
    GameLayer();
    virtual bool init();
    
    bool onTouchBegan(Touch* touch, Event* event);
    void onTouchMoved(Touch* touch, Event* event);
    void onTouchEnded(Touch* touch, Event* event);
    
    bool onContactBegin(PhysicsContact& contact);
    void start(int level = 1);
    
private:
    void addPlayer();
    
    void evilBeAttacked(EvilSprite* evilSprite);
    void soapFallToTheFloor();
    void throwSoap();
    
    bool _gameover;
    WretchSprite* _wretchSprite;
    std::list< EvilSprite* > _evilSprites;
    
    Animation* _evilIdleAnimation[6];
    
    float _centreX;
    float _centreY;
    Vec2 _visibleOrigin;
    Size _visibleSize;
    
    std::unordered_map<int, Node*> _mouses;
    
    int _level;
};

#endif /* defined(__BathhouseWars__GameLayer__) */
