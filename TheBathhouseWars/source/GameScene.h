//
//  GameScene.h
//  TheBathhouseWars
//
//  Created by Wendell on 14/12/6.
//
//

#ifndef __BathhouseWars_GameScene_h
#define __BathhouseWars_GameScene_h

#include "cocos2d.h"

using namespace cocos2d;

class GameLayer;

class GameScene : public Scene
{
public:
    CREATE_FUNC(GameScene);
    
    GameScene();
    virtual bool init() override;
    
private:
    void addMenuLayer();
    void onFail(EventCustom* event);
    void onSuccess(EventCustom* event);
    void retry(Ref* sender);
    void next(Ref* sender);
    
    Node* _menuLayer;
    GameLayer* _gameLayer;
    
    int _level;
    bool _gameover;
};

#endif
