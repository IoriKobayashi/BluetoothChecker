//******************************************************************************
//  SlaveScene.h
//  スレーブ通信シーン
//
//  Created by Iori Kobayashi on 21/3/30.
//******************************************************************************
#ifndef __SLAVE_SCENE_H__
#define __SLAVE_SCENE_H__

#include "ui/UIWidget.h"
#include "ui/CocosGUI.h"
#include "cocos2d.h"

USING_NS_CC;

//---------------------------------------------------------------------------
// スレーブ通信シーンクラス
class SlaveScene : public cocos2d::Scene, public ui::EditBoxDelegate
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    CREATE_FUNC(SlaveScene);
    
    // EditBoxのデリゲート用
    virtual void editBoxEditingDidBegin(ui::EditBox*);      // 開始
    virtual void editBoxEditingDidEnd(ui::EditBox*);        // 終了
    virtual void editBoxTextChanged(ui::EditBox*, const std::string&);          // 変更検出
    virtual void editBoxReturn(ui::EditBox*);               // リターン検出

    // ボタンイベント
    void BtnEventReturn( Ref *, ui::Widget::TouchEventType );       // タイトルへ戻る
    void BtnEventAdvertise( Ref *, ui::Widget::TouchEventType );    // アドバタイズボタン

protected:
private:
    bool m_bAdvertising;
};

#endif
