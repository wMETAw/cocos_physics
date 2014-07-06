//
//  GameScene.cpp
//  nyanball
//
//  Created by RYO on 2014/05/04.
//
//

#include "GameScene.h"
#include "GB2ShapeCache-x.h"
#include "AppMacros.h"

#define PTM_RATIO 1024
USING_NS_CC;


static int scorePoint;

GamePhysicsContactListener::GamePhysicsContactListener(CCObject* target, SEL_CallFunc selecter)
{
    m_target = target;
    m_selector = selecter;
}


void GamePhysicsContactListener::BeginContact(b2Contact* contact)
{
    CCObject* UserDataA = (CCObject*)contact->GetFixtureA()->GetUserData();
    CCObject* UserDataB = (CCObject*)contact->GetFixtureB()->GetUserData();
    
    CCString* stringA = dynamic_cast<CCString*>(UserDataA);
    CCString* stringB = dynamic_cast<CCString*>(UserDataB);
    
    if(stringA->intValue() > 0 || stringB->intValue() > 0)
    {
        scorePoint += stringA->intValue();
        scorePoint += stringB->intValue();
        
        (m_target->*m_selector)();
    }
}



CCScene* GameScene::scene()
{
    // シーンを作成
    CCScene* scene = CCScene::create();
    
    // レイヤーを作成
    GameScene* layer = GameScene::create();
    
    // シーンにレイヤーをセット
    scene->addChild(layer);
    
    return scene;
}


// 初期化関数
bool GameScene::init()
{
    if(!CCLayer::init()) {
        return false;
    }
    
    ballCount = 5;
    debug_flg = false;
    
    // シェイプデータの読み込み
    gbox2d::GB2ShapeCache::sharedGB2ShapeCache()->addShapesWithFile("nyanball.plist");
    
    // タップを有効にする
    setTouchEnabled(true);
    
    // 物理空間の作成、初期化
    initPhysics();
    
    // 背景を作成、描画
    createBackground();
    
    // ボタンを作成、描画
    createButton();
    
    // フリッパーを作成、描画
    createFlipper();
    
    // 得点の表示
    createScore();
    
    // リセットボタンの表示
    createReset();
    
    // デバッグボタンの表示
    createDebug();
    
    // 毎フレーム処理の開始
    scheduleUpdate();
    
    return true;
}


// 物理ワールド初期化
void GameScene::initPhysics()
{
    // 重力の設定
    b2Vec2 gravity;
    gravity.Set(0.0f, -0.8f);
    
    world = new b2World(gravity);
    
    // 衝突判定処理
    gamePhysicsContactListener = new GamePhysicsContactListener(this, callfunc_selector(GameScene::updateScore));
    world->SetContactListener(gamePhysicsContactListener);
    
}


// スコアの作成
void GameScene::createScore()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    // 得点の表示
    score = CCLabelTTF::create("0", "", SCORE_FONT_SIZE);
    score->setPosition(ccp(winSize.width * 0.5, winSize.height * 0.18));
    this->addChild(score, kZOrder_Score);
}


// 背景を作成、描画
void GameScene::createBackground()
{
    // ウィンドウサイズを取得
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    // 背景
    CCSprite* bg = CCSprite::create("background.png");
    bg->setPosition(CCPoint(winSize.width / 2, winSize.height / 2));
    this->addChild(bg, kZOrder_Background, kTagBackground);
    
    // 背景の物理構造
    b2BodyDef bgBodyDef;
    bgBodyDef.type = b2_staticBody;
    bgBodyDef.position.Set(bg->getPositionX() / PTM_RATIO, bg->getPositionY() / PTM_RATIO);
    bgBodyDef.userData = bg;
    bgBody = world->CreateBody(&bgBodyDef);
    
    gbox2d::GB2ShapeCache* sc = gbox2d::GB2ShapeCache::sharedGB2ShapeCache();
    sc->addFixturesToBody(bgBody, "background");
    bg->setAnchorPoint(sc->anchorPointForShape("background"));
}

// ボタンを作成、描画
void GameScene::createButton()
{
    // ウィンドウサイズを取得
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    // 左ボタン
    CCSprite* leftButton = CCSprite::create("button_left.png");
    leftButton->setPosition(ccp(winSize.width * 0.2, winSize.height * 0.1));
    this->addChild(leftButton, kZOrder_Button);
    
    // 右ボタン
    CCSprite* rightButton = CCSprite::create("button_right.png");
    rightButton->setPosition(ccp(winSize.width * 0.8, winSize.height * 0.1));
    this->addChild(rightButton, kZOrder_Button);
}


// フリッパーを作成、描画
void GameScene::createFlipper()
{
    // ウィンドウサイズを取得
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    gbox2d::GB2ShapeCache* sc = gbox2d::GB2ShapeCache::sharedGB2ShapeCache();
    
    // 左フリッパー
    CCPoint leftFlipperPos = ccp(winSize.width * 0.3, winSize.height * 0.04);
    CCSprite* leftFlipper = CCSprite::create("flipper_left.png");
    leftFlipper->setPosition(leftFlipperPos);
    this->addChild(leftFlipper, kZOrder_Flipper);
    
    // 左フリッパーの物理構造
    b2BodyDef leftFlipperBodyDef;
    leftFlipperBodyDef.type = b2_dynamicBody;
    leftFlipperBodyDef.position.Set(leftFlipperPos.x /PTM_RATIO, leftFlipperPos.y / PTM_RATIO);
    leftFlipperBodyDef.userData = leftFlipper;
    
    leftFlipperBody = world->CreateBody(&leftFlipperBodyDef);
    sc->addFixturesToBody(leftFlipperBody, "flipper_left");
    leftFlipper->setAnchorPoint(sc->anchorPointForShape("flipper_left"));
    leftFlipperBody->SetTransform(leftFlipperBody->GetPosition(), CC_DEGREES_TO_RADIANS(0));
    
    // 左フリッパーのジョイント
    b2RevoluteJointDef leftJoinDef;
    leftJoinDef.Initialize(leftFlipperBody, bgBody, leftFlipperBody->GetPosition());
    leftJoinDef.lowerAngle = CC_DEGREES_TO_RADIANS(-45);
    leftJoinDef.upperAngle = CC_DEGREES_TO_RADIANS(0);
    leftJoinDef.enableLimit = true;
    world->CreateJoint(&leftJoinDef);
    
    
    // 右フリッパー
    CCPoint rightFlipperPos = ccp(winSize.width * 0.7, winSize.height * 0.04);
    CCSprite* rightFlipper = CCSprite::create("flipper_right.png");
    rightFlipper->setPosition(rightFlipperPos);
    this->addChild(rightFlipper, kZOrder_Flipper);
    
    // 右フリッパーの物理構造
    b2BodyDef rightFlipperBodyDef;
    rightFlipperBodyDef.type = b2_dynamicBody;
    rightFlipperBodyDef.position.Set(rightFlipperPos.x /PTM_RATIO, rightFlipperPos.y / PTM_RATIO);
    rightFlipperBodyDef.userData = rightFlipper;
    
    rightFlipperBody = world->CreateBody(&rightFlipperBodyDef);
    sc->addFixturesToBody(rightFlipperBody, "flipper_right");
    rightFlipper->setAnchorPoint(sc->anchorPointForShape("flipper_right"));
    rightFlipperBody->SetTransform(rightFlipperBody->GetPosition(), CC_DEGREES_TO_RADIANS(0));
    
    // 右フリッパーのジョイント
    b2RevoluteJointDef rightJoinDef;
    rightJoinDef.Initialize(rightFlipperBody, bgBody, rightFlipperBody->GetPosition());
    rightJoinDef.lowerAngle = CC_DEGREES_TO_RADIANS(0);
    rightJoinDef.upperAngle = CC_DEGREES_TO_RADIANS(45);
    rightJoinDef.enableLimit = true;
    world->CreateJoint(&rightJoinDef);
    
}


// 画像の更新
void GameScene::update(float dt)
{
    int velocityIterations = 8;
    int positionIterations = 1;
    
    world->Step(dt, velocityIterations, positionIterations);
    
    // 物理ワールドすべての剛体をループ
    for (b2Body* b = world->GetBodyList(); b; b = b->GetNext())
    {
        if(b->GetUserData() != NULL) {
            CCSprite* myActor = (CCSprite*)b->GetUserData();
            
            if(b->GetPosition().y < 0) {
                world->DestroyBody(b);
                this->removeChild(myActor);
                continue;
            }

            myActor->setPosition(CCPointMake(b->GetPosition().x * PTM_RATIO, b->GetPosition().y * PTM_RATIO));
            myActor->setRotation(-1 * CC_RADIANS_TO_DEGREES(b->GetAngle()));
        }
    }
}


// 得点を更新
void GameScene::updateScore()
{
    CCString* string = CCString::createWithFormat("%d", scorePoint);
    score->setString(string->getCString());
}


// ボールの生成
void GameScene::createBall()
{
    // 制限数を設定
    if(ballCount <= 0 && debug_flg == false) {
        return;
    }
    ballCount--;
    
    // ウィンドウサイズの取得
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    // ボールの作成
    CCPoint ballPoint = ccp(winSize.width * 0.25, winSize.height * 0.95);
    CCSprite* ball = CCSprite::create("ball.png");
    ball->setPosition(ballPoint);
    this->addChild(ball, kZOrder_Ball);
    
    // ボールの物理構造
    b2BodyDef ballBodyDef;
    ballBodyDef.type = b2_dynamicBody;
    ballBodyDef.position.Set(ballPoint.x / PTM_RATIO, ballPoint.y / PTM_RATIO);
    ballBodyDef.userData = ball;
    b2Body* body = world->CreateBody(&ballBodyDef);
    
    gbox2d::GB2ShapeCache* sc = gbox2d::GB2ShapeCache::sharedGB2ShapeCache();
    sc->addFixturesToBody(body, "ball");
    ball->setAnchorPoint(sc->anchorPointForShape("ball"));
}


// タップ開始
void GameScene::ccTouchesBegan(CCSet* touches, CCEvent* event)
{
    CCSetIterator it;
    for (it = touches->begin(); it != touches->end(); ++it)
    {
        CCTouch* touch = (CCTouch*)(*it);
        
        if(!touch) {
            break;
        }
        
        CCSize winSize = CCDirector::sharedDirector()->getWinSize();
        
        CCDirector* pDirector = CCDirector::sharedDirector();
        CCPoint location = pDirector->convertToGL(touch->getLocationInView());
        
        // 画面上部をタップしたらボール生成
        if(location.y > winSize.height / 2) {
            createBall();
            
            // 画面の右をタップ
        } else if (location.x > winSize.width / 2 ) {
            rightFlipperBody->ApplyTorque(-1);
            
            // 画面の左をタップ
        } else {
            leftFlipperBody->ApplyTorque(1);
        }
        
    }
}


// リセットボタン作成
void GameScene::createReset()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    // スコアの表示
    CCMenuItemImage* reset = CCMenuItemImage::create("button_reset1.png", "button_reset2.png", this, menu_selector(GameScene::tapReset));
    reset->setPosition(ccp(winSize.width * 0.8, winSize.height * 0.94));
    
    CCMenu* menu = CCMenu::create(reset, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu, kZOrder_Button);
}


// リセット処理
void GameScene::tapReset()
{
    CCScene* scene = GameScene::scene();
    CCDirector::sharedDirector()->replaceScene(scene);
}


// デバッグボタン作成
void GameScene::createDebug()
{
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    // スコアの表示
    CCMenuItemImage* debug = CCMenuItemImage::create("button_reset2.png", "button_reset1.png", this, menu_selector(GameScene::tapDebug));
    debug->setPosition(ccp(winSize.width * 0.6, winSize.height * 0.94));
    
    CCMenu* menu = CCMenu::create(debug, NULL);
    menu->setPosition(CCPointZero);
    this->addChild(menu, kZOrder_Button);
}


// デバッグ処理
void GameScene::tapDebug()
{
    debug_flg = true;
}










