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

class GameScene : public cocos2d::CCLayer
{
protected:
    
    // 画像のタグ
    enum kTag
    {
        kTagBackground = 1,
    };
    
    // Zバッファの順番
    enum kZOrder
    {
        kZOrder_Background = 1,
        kZOrder_Flipper,
        kZOrder_Button,
    };
    
    // 各画像の作成関数
    void createBackground();
    void createButton();
    void createFlipper();
    
public:
    virtual bool init();
    static cocos2d::CCScene* scene();
    
    // cocosで使われる.create()関数が定義されている
    CREATE_FUNC(GameScene);
};

#endif /* defined(__nyanball__GameScene__) */
