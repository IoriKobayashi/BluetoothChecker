//******************************************************************************
//  MasterScene.h
//  マスター通信シーン
//
//  Created by Iori Kobayashi on 21/3/30.
//******************************************************************************
#ifndef __MASTER_SCENE_H__
#define __MASTER_SCENE_H__

#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "cocos2d.h"

USING_NS_CC;

//---------------------------------------------------------------------------
// マスター通信シーンクラス
class MasterScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    CREATE_FUNC(MasterScene);

    // 更新処理
    void UpdateScan(float);     // デバイススキャン中
    void UpdateConnect(float);  // 接続中

    // ボタンイベント
    void BtnEventReturn( Ref *, ui::Widget::TouchEventType );       // タイトルへ戻る
    void BtnEventScan( Ref *, ui::Widget::TouchEventType );         // スキャンボタン
    void BtnEventConnect( Ref *, ui::Widget::TouchEventType );      // 接続
    void BtnEventDisconnect( Ref *, ui::Widget::TouchEventType );   // 切断
    void BtnEventRead( Ref *, ui::Widget::TouchEventType );         // 読み込み

protected:
private:
    bool m_bScaning;        // スキャン中かどうか
    int m_nFoundPeri;       // 発見済ペリフェラル数
};

#endif