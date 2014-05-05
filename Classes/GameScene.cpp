//
//  GameScene.cpp
//  nyanball
//
//  Created by RYO on 2014/05/04.
//
//

#include "GameScene.h"

USING_NS_CC;

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



// 背景を作成、描画
void GameScene::createBackground()　
{
    // ウィンドウサイズを取得
    CCSize winSize = CCDirector::sharedDirector()->getWinSize();
    
    // 背景
    CCSprite* bg = CCSprite::create("background.png");
    bg->setPosition(CCPoint(winSize.width / 2, winSize.height / 2));
    this->addChild(bg, kZOrder_Background, kTagBackground);
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
    
    // 左フリッパー
    CCPoint leftFlipperPos = ccp(winSize.width * 0.3, winSize.height * 0.04);
    CCSprite* leftFlipper = CCSprite::create("flipper_left.png");
    leftFlipper->setPosition(leftFlipperPos);
    this->addChild(leftFlipper, kZOrder_Flipper);
    
    // 右フリッパー
    CCPoint rightFlipperPos = ccp(winSize.width * 0.7, winSize.height * 0.04);
    CCSprite* rightFlipper = CCSprite::create("flipper_right.png");
    rightFlipper->setPosition(rightFlipperPos);
    this->addChild(rightFlipper, kZOrder_Flipper);
}


// 初期化関数
bool GameScene::init()
{
    if(!CCLayer::init()) {
        return false;
    }
    
    // 背景を作成、描画
    createBackground();
    
    // ボタンを作成、描画
    createButton();
    
    // フリッパーを作成、描画
    createFlipper();
    
    return true;
}















