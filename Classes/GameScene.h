//
//  GameScene.h
//  nyanball
//
//  Created by RYO on 2014/05/04.
//
//

#ifndef __nyanball__GameScene__
#define __nyanball__GameScene__

#include "cocos2d.h"
#include "Box2D/Box2D.h"

// 衝突判定クラス
class GamePhysicsContactListener : public b2ContactListener
{
    
protected:
    cocos2d::CCObject* m_target;
    cocos2d::SEL_CallFunc m_selector;
    
public:
    
    // コンストラクタ
    GamePhysicsContactListener(cocos2d::CCObject* target, cocos2d::SEL_CallFunc selecter);
    void BeginContact(b2Contact* contact);
};


class GameScene : public cocos2d::CCLayer
{
protected:
    
    bool debug_flg;
    void createDebug();
    void tapDebug();
    
    // 画像のタグ
    enum kTag
    {
        kTagBackground = 1,
    };
    
    // Zバッファの順番
    enum kZOrder
    {
        kZOrder_Background = 1,
        kZOrder_Score,
        kZOrder_Flipper,
        kZOrder_Ball,
        kZOrder_Button,
    };
    
    // 各画像の作成関数
    void createBackground();
    void createButton();
    void createFlipper();
    
    // 物理ワールドの作成
    b2World* world;
    void initPhysics();
    
    // 筐体
    b2Body* bgBody;
    b2Body* leftFlipperBody;
    b2Body* rightFlipperBody;
    
    cocos2d::CCLabelTTF* score;
    GamePhysicsContactListener* gamePhysicsContactListener;
    void createScore();
    void updateScore();
    
    // リセットボタン
    int ballCount;
    void createReset();
    void tapReset();
    
    // ボールの生成
    void createBall();
    virtual void ccTouchesBegan(cocos2d::CCSet* touches, cocos2d::CCEvent* event);
    
public:
    virtual bool init();
    static cocos2d::CCScene* scene();
    
    // cocosで使われる.create()関数が定義されている
    CREATE_FUNC(GameScene);
    
    // 更新関数
    void update(float dt);
};


#endif /* defined(__nyanball__GameScene__) */














