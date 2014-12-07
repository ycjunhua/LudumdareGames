//
//  GameScene.cpp
//  TheBathhouseWars
//
//  Created by Wendell on 14/12/6.
//
//

#include "GameScene.h"
#include "GameResource.h"
#include "GameLayer.h"
#include "audio/include/SimpleAudioEngine.h"

using namespace CocosDenshion;

Vec2 s_visibleOrigin;
Size s_visibleSize;
float s_centreX;
float s_centreY;
PhysicsWorld* s_physicsWorld;

GameScene::GameScene()
: _gameover(false)
, _level(1)
{
    
}

bool GameScene::init()
{
    bool ret = initWithPhysics();
    
    s_physicsWorld = _physicsWorld;
    //_physicsWorld->setDebugDrawMask(PhysicsWorld::DEBUGDRAW_ALL);
    
    auto director = Director::getInstance();
    s_visibleOrigin = director->getVisibleOrigin();
    s_visibleSize = director->getVisibleSize();
    s_centreX = s_visibleOrigin.x + s_visibleSize.width / 2;
    s_centreY = s_visibleOrigin.y + s_visibleSize.height / 2;
    
    //left
    auto wallNode = Node::create();
    auto wallLeft = PhysicsBody::createEdgeBox(Size(500, s_visibleSize.height),PhysicsMaterial(1, 1.0f, 0.0f));
    wallLeft->setDynamic(false);
    wallNode->setPhysicsBody(wallLeft);
    wallNode->setPosition(Vec2(s_visibleOrigin.x - 250, s_centreY));
    addChild(wallNode);
    
    //right
    wallNode = Node::create();
    auto wallRight = PhysicsBody::createEdgeBox(Size(500, s_visibleSize.height),PhysicsMaterial(1, 1.05f, 0.0f));
    wallRight->setDynamic(false);
    wallNode->setPhysicsBody(wallRight);
    wallNode->setPosition(Vec2(s_visibleOrigin.x + s_visibleSize.width + 250, s_centreY));
    addChild(wallNode);
    
    //front
    wallNode = Node::create();
    auto wallFront = PhysicsBody::createEdgeBox(Size(s_visibleSize.width, 500),PhysicsMaterial(1, 1.05f, 0.0f));
    wallFront->setDynamic(false);
    wallNode->setPhysicsBody(wallFront);
    wallNode->setPosition(Vec2(s_centreX, s_visibleOrigin.y + s_visibleSize.height + 250));
    addChild(wallNode);
    
    //floor
    auto floorNode = Node::create();
    floorNode->setTag(FloorTag);
    auto floorBody = PhysicsBody::createEdgeBox(Size(s_visibleSize.width, 200),PhysicsMaterial(1, 0.03f, 1.0f));
    floorBody->setDynamic(false);
    floorBody->setContactTestBitmask(0xFFFFFFFF);
    floorNode->setPhysicsBody(floorBody);
    floorNode->setPosition(Vec2(s_centreX, s_visibleOrigin.y - 100));
    this->addChild(floorNode);
    
    //resource
    auto frameCache = SpriteFrameCache::getInstance();
    frameCache->addSpriteFramesWithFile("res/resource.plist");
    
    _gameLayer = GameLayer::create();
    addChild(_gameLayer);
    addMenuLayer();
    
    auto audio = SimpleAudioEngine::getInstance();
    audio->preloadBackgroundMusic("res/fail.mp3");
    audio->preloadEffect("res/water.mp3");
    audio->preloadEffect("res/effect01.mp3");
    audio->preloadEffect("res/effect02.mp3");
    audio->preloadEffect("res/throwSoap.mp3");
    
    _eventDispatcher->addCustomEventListener("fail", std::bind(&GameScene::onFail, this, std::placeholders::_1));
    _eventDispatcher->addCustomEventListener("success", std::bind(&GameScene::onSuccess, this, std::placeholders::_1));
    
    return ret;
}

void GameScene::addMenuLayer()
{
    _menuLayer = LayerColor::create(Color4B(0,0,0,100));
    addChild(_menuLayer);
    auto playLabel = Label::createWithTTF("Play", "fonts/arial.ttf", 50);
    auto playItem = MenuItemLabel::create(playLabel, [&](Ref* sender){
        _menuLayer->removeFromParentAndCleanup(true);
        _gameLayer->start(_level);
        _menuLayer = nullptr;
    });
    playItem->setPosition(Vec2(s_centreX, s_centreY));
    auto menu = Menu::create( playItem, nullptr);
    menu->setPosition( Vec2(0,0) );
    _menuLayer->addChild(menu);
}

void GameScene::onFail(EventCustom* event)
{
    if (!_gameover) {
        _gameover = true;
        SimpleAudioEngine::getInstance()->playEffect("res/effect02.mp3");
        if (s_waterAudioID != -1) {
            SimpleAudioEngine::getInstance()->stopEffect(s_waterAudioID);
            s_waterAudioID = -1;
        }
        
        auto overAction = Sequence::create(FadeOut::create(1.0f), CCCallFunc::create([&](){
            _gameLayer->removeFromParentAndCleanup(true);
            SimpleAudioEngine::getInstance()->playBackgroundMusic("res/fail.mp3", true);
        }), nullptr);
        _gameLayer->runAction(overAction);
        
        _menuLayer = Sprite::create("res/fail.png");
        _menuLayer->setPosition(Vec2(s_centreX, s_centreY));
        _menuLayer->setOpacity(0);
        addChild(_menuLayer);
        
        auto menuAction = Sequence::create(FadeIn::create(0.5f), CCCallFunc::create([&](){
            auto retryLabel = Label::createWithTTF("Retry", "fonts/arial.ttf", 50);
            auto retryItem = MenuItemLabel::create(retryLabel, CC_CALLBACK_1(GameScene::retry, this));
            retryItem->setPosition(Vec2(s_centreX, s_centreY));
            retryItem->setOpacity(0);
            retryItem->runAction(FadeIn::create(0.5f));
            
            auto menu = Menu::create( retryItem, nullptr);
            menu->setPosition( Vec2(0,0) );
            _menuLayer->addChild(menu);
        }), nullptr);
        _menuLayer->runAction(menuAction);
    }
}

void GameScene::onSuccess(EventCustom* event)
{
    if (s_waterAudioID != -1) {
        SimpleAudioEngine::getInstance()->stopEffect(s_waterAudioID);
        s_waterAudioID = -1;
    }
    
    auto overAction = Sequence::create(FadeOut::create(1.0f), CCCallFunc::create([&](){
        _gameLayer->removeFromParentAndCleanup(true);
    }), nullptr);
    _gameLayer->runAction(overAction);
    
    _menuLayer = Sprite::create("res/success.png");
    _menuLayer->setPosition(Vec2(s_centreX, s_centreY));
    _menuLayer->setOpacity(0);
    addChild(_menuLayer);
    
    char str[50];
    auto frameCache = SpriteFrameCache::getInstance();
    
    Vector<SpriteFrame*> waterAnimFrames(6);
    for (int index = 0; index < 3; ++index) {
        sprintf(str, "water%d.png", index);
        auto frame = frameCache->getSpriteFrameByName( str );
        waterAnimFrames.pushBack(frame);
    }
    auto waterAnimation = Animation::createWithSpriteFrames(waterAnimFrames, 0.1f);
    auto waterSprite = Sprite::createWithSpriteFrameName("water0.png");
    waterSprite->setPosition(Vec2(s_visibleOrigin.x + 130, s_visibleOrigin.y + 420));
    _menuLayer->addChild(waterSprite);
    waterSprite->runAction( RepeatForever::create( Animate::create(waterAnimation) ) );
    
    auto menuAction = Sequence::create(FadeIn::create(0.5f), CCCallFunc::create([&](){
        auto retryLabel = Label::createWithTTF("Retry", "fonts/arial.ttf", 30);
        auto retryItem = MenuItemLabel::create(retryLabel, CC_CALLBACK_1(GameScene::retry, this));
        retryItem->setPosition(Vec2(s_visibleOrigin.x + 75, s_visibleOrigin.y + 80));
        retryItem->setOpacity(0);
        retryItem->runAction(FadeIn::create(0.5f));
        
        auto nextLabel = Label::createWithTTF("Next", "fonts/arial.ttf", 30);
        auto nextItem = MenuItemLabel::create(nextLabel, CC_CALLBACK_1(GameScene::next, this));
        nextItem->setPosition(Vec2(s_visibleOrigin.x + s_visibleSize.width - 75, s_visibleOrigin.y + 85));
        nextItem->setOpacity(0);
        nextItem->runAction(FadeIn::create(0.5f));
        
        auto menu = Menu::create( retryItem, nextItem, nullptr);
        menu->setPosition( Vec2(0,0) );
        _menuLayer->addChild(menu);
    }), nullptr);
    _menuLayer->runAction(menuAction);
}

void GameScene::retry(Ref* sender)
{
    SimpleAudioEngine::getInstance()->stopBackgroundMusic();

    _gameover = false;
    _menuLayer->removeFromParentAndCleanup(true);
    
    _gameLayer = GameLayer::create();
    addChild(_gameLayer);
    
    _gameLayer->start(_level);
}

void GameScene::next(Ref* sender)
{
    _level += 1;
    retry(sender);
}
