//
//  GameLayer.cpp
//  TheBathhouseWars
//
//  Created by Wendell on 14/12/6.
//
//

#include "GameLayer.h"
#include "GameResource.h"
#include "EvilSprite.h"
#include "WretchSprite.h"
#include "audio/include/SimpleAudioEngine.h"

using namespace CocosDenshion;

int s_waterAudioID = -1;

GameLayer::GameLayer()
: _level(1)
{
    
}

bool GameLayer::init()
{
    bool ret = false;
    
    if (Layer::init()) {
        auto director = Director::getInstance();
        _visibleOrigin = director->getVisibleOrigin();
        _visibleSize = director->getVisibleSize();
        _centreX = _visibleOrigin.x + _visibleSize.width / 2;
        _centreY = _visibleOrigin.y + _visibleSize.height / 2;
        
        auto bg = Sprite::create("res/background.jpg");
        bg->setPosition(Vec2(s_centreX, s_centreY));
        addChild(bg);
        
        addPlayer();
        
        auto touchListener = EventListenerTouchOneByOne::create();
        touchListener->onTouchBegan = CC_CALLBACK_2(GameLayer::onTouchBegan, this);
        touchListener->onTouchMoved = CC_CALLBACK_2(GameLayer::onTouchMoved, this);
        touchListener->onTouchEnded = CC_CALLBACK_2(GameLayer::onTouchEnded, this);
        touchListener->onTouchCancelled = CC_CALLBACK_2(GameLayer::onTouchEnded, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
        
        auto contactListener = EventListenerPhysicsContact::create();
        contactListener->onContactBegin = CC_CALLBACK_1(GameLayer::onContactBegin, this);
        _eventDispatcher->addEventListenerWithSceneGraphPriority(contactListener, this);
        
        ret = true;
    }
    
    return ret;
}

void GameLayer::addPlayer()
{
    Vec2 evilPos[] = {
        Vec2(_visibleOrigin.x + 110, _visibleOrigin.y + 260),
        Vec2(_visibleOrigin.x + 160, _visibleOrigin.y + 425),
        
        Vec2(_visibleOrigin.x + 270, _visibleOrigin.y + 450),
        Vec2(_visibleOrigin.x + 700, _visibleOrigin.y + 450),
        
        Vec2(_visibleOrigin.x + _visibleSize.width - 160, _visibleOrigin.y + 415),
        Vec2(_visibleOrigin.x + _visibleSize.width - 110, _visibleOrigin.y + 250)
    };
    
    Vec2 waterPos[] = {
        Vec2(_visibleOrigin.x + 105, _visibleOrigin.y + 320),
        Vec2(_visibleOrigin.x + 145, _visibleOrigin.y + 430),
        
        Vec2(_visibleOrigin.x + 250, _visibleOrigin.y + 450),
        Vec2(_visibleOrigin.x + 705, _visibleOrigin.y + 450),
        
        Vec2(_visibleOrigin.x + _visibleSize.width - 150, _visibleOrigin.y + 425),
        Vec2(_visibleOrigin.x + _visibleSize.width - 110, _visibleOrigin.y + 300)
    };
    
    char str[50];
    auto frameCache = SpriteFrameCache::getInstance();
    
    Vector<SpriteFrame*> waterAnimFrames(6);
    for (int index = 0; index < 3; ++index) {
        sprintf(str, "water%d.png", index);
        auto frame = frameCache->getSpriteFrameByName( str );
        waterAnimFrames.pushBack(frame);
    }
    auto waterAnimation = Animation::createWithSpriteFrames(waterAnimFrames, 0.1f);
    
    for (int index = 0; index < 6; ++index) {
        int count = 6;
        if (index == 4) {
            count = 4;
        }
        
        Vector<SpriteFrame*> animFrames(6);
        for (int i = 0; i < count; ++i) {
            sprintf(str, "evil0%d%d.png", index, i);
            auto frame = frameCache->getSpriteFrameByName( str );
            animFrames.pushBack(frame);
        }
        
        _evilIdleAnimation[index] = Animation::createWithSpriteFrames(animFrames, 0.15f);
        
        auto waterSprite = Sprite::createWithSpriteFrameName("water0.png");
        waterSprite->setPosition(waterPos[index]);
        waterSprite->runAction( RepeatForever::create( Animate::create(waterAnimation) ) );
        
        auto evilSprite = EvilSprite::create(index);
        switch (index) {
            case 0:
                evilSprite->setScale(0.6f, 0.6f);
                break;
            case 1:
                evilSprite->setScale(0.5f, 0.5f);
                break;
            case 2:
                evilSprite->setScale(0.35f, 0.35f);
                waterSprite->setFlippedX(true);
                break;
            case 3:
                evilSprite->setScale(0.35f, 0.35f);
                break;
            case 4:
                evilSprite->setScale(0.5f, 0.5f);
                waterSprite->setFlippedX(true);
                break;
            case 5:
                evilSprite->setScale(0.65f, 0.65f);
                waterSprite->setFlippedX(true);
                break;
            default:
                break;
        }
        
        evilSprite->runAction( RepeatForever::create( Animate::create(_evilIdleAnimation[index]) ) );
        evilSprite->setPosition(evilPos[index]);
        addChild(evilSprite);
        
        addChild(waterSprite);
        _evilSprites.push_back(evilSprite);
    }
    
    _wretchSprite = WretchSprite::create();
    _wretchSprite->setPosition(Vec2(_centreX, _visibleOrigin.y + 70));
    addChild(_wretchSprite);
    
    auto slideFixNode = Node::create();
    auto slideFixBody = PhysicsBody::createEdgeSegment( Vec2(- s_visibleSize.width / 2, 0), Vec2( s_visibleSize.width / 2, 0) );
    slideFixBody->setCollisionBitmask(0);
    slideFixNode->setPosition(Vec2(s_centreX, s_visibleOrigin.y));
    slideFixBody->setDynamic(false);
    slideFixNode->setPhysicsBody(slideFixBody);
    addChild(slideFixNode);
    
    auto wretchSize = _wretchSprite->getContentSize();
    auto joint = PhysicsJointGroove::construct(slideFixBody, _wretchSprite->getPhysicsBody(), Vec2(- s_visibleSize.width / 2, 30), Vec2( s_visibleSize.width / 2 , 30), Vec2(0, -10));
    s_physicsWorld->addJoint(joint);
}

bool GameLayer::onTouchBegan(cocos2d::Touch *touch, cocos2d::Event *event)
{
    auto location = touch->getLocation();
    auto scene = Director::getInstance()->getRunningScene();
    auto arr = scene->getPhysicsWorld()->getShapes(location);
    
    PhysicsBody* body = nullptr;
    for (auto& obj : arr)
    {
        if ((obj->getBody()->getTag() == WretchTag) != 0)
        {
            body = obj->getBody();
            break;
        }
    }
    
    if (body != nullptr)
    {
        Node* mouse = Node::create();
        mouse->setAnchorPoint(Vec2::ANCHOR_MIDDLE);
        mouse->setPhysicsBody(PhysicsBody::create(PHYSICS_INFINITY, PHYSICS_INFINITY));
        mouse->getPhysicsBody()->setDynamic(false);
        mouse->setPosition(location);
        _parent->addChild(mouse);
        
        PhysicsJointPin* joint = PhysicsJointPin::construct(mouse->getPhysicsBody(), body, location);
        joint->setMaxForce(100000000.0f );
        scene->getPhysicsWorld()->addJoint(joint);
        _mouses.insert(std::make_pair(touch->getID(), mouse));
        
        return true;
    }
    
    return false;
}

void GameLayer::onTouchMoved(cocos2d::Touch *touch, cocos2d::Event *event)
{
    auto it = _mouses.find(touch->getID());
    
    auto pos = this->convertTouchToNodeSpace(touch);
    if (it != _mouses.end())
    {
        it->second->setPosition(pos);
    }
}

void GameLayer::onTouchEnded(cocos2d::Touch *touch, cocos2d::Event *event)
{
    auto it = _mouses.find(touch->getID());
    
    if (it != _mouses.end())
    {
        _parent->removeChild(it->second);
        _mouses.erase(it);
    }
}

bool GameLayer::onContactBegin(cocos2d::PhysicsContact &contact)
{
    auto nodeA = contact.getShapeA()->getBody()->getNode();
    auto nodeB = contact.getShapeB()->getBody()->getNode();
    if (nodeA && nodeB)
    {
        int tagA = nodeA->getTag();
        int tagB = nodeB->getTag();
        
        if (tagA == SoapTag) {
            switch (tagB) {
                case FloorTag: {
                    soapFallToTheFloor();
                    break;
                }
                case WretchTag: {
                    SimpleAudioEngine::getInstance()->playEffect("res/effect01.mp3");
                    break;
                }
                case EvilTag: {
                    nodeA->removeFromParentAndCleanup(true);
                    evilBeAttacked((EvilSprite*)nodeB);
                    SimpleAudioEngine::getInstance()->playEffect("res/effect01.mp3");
                    break;
                }
                default:
                    break;
            }
        }
        else if (tagB == SoapTag){
            switch (tagA) {
                case FloorTag: {
                    soapFallToTheFloor();
                    break;
                }
                case WretchTag: {
                    SimpleAudioEngine::getInstance()->playEffect("res/effect01.mp3");
                    break;
                }
                case EvilTag: {
                    SimpleAudioEngine::getInstance()->playEffect("res/effect01.mp3");
                    nodeB->removeFromParentAndCleanup(true);
                    evilBeAttacked((EvilSprite*)nodeA);
                    break;
                }
                default:
                    break;
            }
        }
    }
    return true;
}

void GameLayer::soapFallToTheFloor()
{
    _eventDispatcher->dispatchCustomEvent("fail");
}

void GameLayer::evilBeAttacked(EvilSprite *evilSprite)
{
    evilSprite->beAttacked();
    _evilSprites.remove(evilSprite);
    if (_evilSprites.size() > 0) {
        throwSoap();
    } else {
        _eventDispatcher->dispatchCustomEvent("success");
    }
}

void GameLayer::start(int level)
{
    s_waterAudioID = SimpleAudioEngine::getInstance()->playEffect("res/water.mp3", true);
    
    Vector<FiniteTimeAction*> actions;
    actions.pushBack(Blink::create(1.5f, 3));
    actions.pushBack(CCCallFunc::create([&](){
        throwSoap();
    }));
    
    for ( int i = 1; i < level; ++i) {
        actions.pushBack(DelayTime::create(1.65f));
        actions.pushBack(CCCallFunc::create([&](){
            throwSoap();
        }));
    }
    
    _wretchSprite->runAction(Sequence::create(actions));
}

void GameLayer::throwSoap()
{
    this->runAction(Sequence::create(DelayTime::create(0.75f), CCCallFunc::create([&](){
        auto sprite = Sprite::createWithSpriteFrameName("soap.png");
        sprite->setTag(SoapTag);
        sprite->setPosition(Vec2(_centreX,
                                 _visibleOrigin.y + _visibleSize.height - 100));
        sprite->setScale(0.5f, 0.5f);
        addChild(sprite);
        
        auto physicsBody = PhysicsBody::createCircle(20, PhysicsMaterial(0.1f, 1.0f, 1.0f));
        physicsBody->setContactTestBitmask(0xFFFFFFFF);
        sprite->setPhysicsBody(physicsBody);
        
        physicsBody->setVelocityLimit(800);
        physicsBody->setVelocity(Vec2(cocos2d::random(-100,100),cocos2d::random(-400,-200)));
        
        physicsBody->setAngularVelocity(cocos2d::random(-1.0f,1.0f));
        SimpleAudioEngine::getInstance()->playEffect("res/throwSoap.mp3");
    }), nullptr));
}
