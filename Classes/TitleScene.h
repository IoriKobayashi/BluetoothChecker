//******************************************************************************
//  TitleScene.h
//  タイトルシーン
//
//  Created by Iori Kobayashi on 21/3/29.
//******************************************************************************
#ifndef __TITLE_SCENE_H__
#define __TITLE_SCENE_H__

#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "cocos2d.h"

USING_NS_CC;

class TitleScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    CREATE_FUNC(TitleScene);

    // ボタンイベント
    void BtnEventMaster( Ref *, ui::Widget::TouchEventType );   // マスター通信
    void BtnEventSlave(Ref *, ui::Widget::TouchEventType );     // スレーブ通信

    
protected:
private:
};

#endif
