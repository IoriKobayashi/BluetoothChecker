//******************************************************************************
//  TitleScene.cpp
//  タイトルシーン
//
//  Created by Iori Kobayashi on 21/3/29.
//******************************************************************************
#include "TitleScene.h"
#include "MasterScene.h"
#include "SlaveScene.h"

USING_NS_CC;

//---------------------------------------------------------------------------
// シーンのクリエイト
//---------------------------------------------------------------------------
Scene* TitleScene::createScene()
{
    return TitleScene::create();
}

//---------------------------------------------------------------------------
// 初期化処理
//---------------------------------------------------------------------------
bool TitleScene::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    // 変数宣言
    auto visibleSize = Director::getInstance()->getVisibleSize();   // 有効サイズ
    Vec2 origin = Director::getInstance()->getVisibleOrigin();      // シーンのオリジナル位置

    // 背景スプライトの作成
    Sprite *back = Sprite::create( "white.png" );
    back->setColor( Color3B::BLACK );
    back->setPosition( Vec2(visibleSize.width / 2, visibleSize.height / 2) );
    back->setContentSize( visibleSize );
    this->addChild( back );

    // タイトルテキストの作成
    ui::Text *pTextTitle = ui::Text::create( "Bluetooth Checker", "mplus-2c-medium.ttf", 16.0f );
    pTextTitle->setColor( Color3B::WHITE );
    pTextTitle->setPosition( Vec2(visibleSize.width / 2, visibleSize.height / 10 * 8) );
    this->addChild( pTextTitle );

    // マスター通信用ボタンの作成
    ui::Button* pBtnMaster = ui::Button::create( "white.png" );
    pBtnMaster->setScale9Enabled( true );
    pBtnMaster->setContentSize( Size(150.0f, 30.0f) );
    pBtnMaster->setTitleColor( Color3B::BLACK );
    pBtnMaster->setTitleText( "Master(親)通信" );
    pBtnMaster->setTitleFontSize( 16.0f );
    pBtnMaster->setPosition( Vec2(visibleSize.width / 2, visibleSize.height / 10 * 5) );
    pBtnMaster->addTouchEventListener( CC_CALLBACK_2( TitleScene::BtnEventMaster, this ));
    this->addChild( pBtnMaster, 0, "BtnMaster" );

    // スレーブ通信用ボタンの作成
    ui::Button* pBtnSlave = ui::Button::create( "white.png" );
    pBtnSlave->setScale9Enabled( true );
    pBtnSlave->setContentSize( Size(150.0f, 30.0f) );
    pBtnSlave->setTitleColor( Color3B::BLACK );
    pBtnSlave->setTitleText( "Slave(子)通信" );
    pBtnSlave->setTitleFontSize( 16.0f );
    pBtnSlave->setPosition( Vec2(visibleSize.width / 2, visibleSize.height / 10 * 3) );
    pBtnSlave->addTouchEventListener( CC_CALLBACK_2( TitleScene::BtnEventSlave, this ));
    this->addChild( pBtnSlave, 0, "BtnSlave" );

    return true;
}

//---------------------------------------------------------------------------
// マスター通信ボタンイベント
//---------------------------------------------------------------------------
void TitleScene::BtnEventMaster(Ref *pSender, ui::Widget::TouchEventType type)
{
    switch( type ){
        case ui::Widget::TouchEventType::BEGAN:
        case ui::Widget::TouchEventType::MOVED:
            break;
        case ui::Widget::TouchEventType::CANCELED:
            break;
        case ui::Widget::TouchEventType::ENDED:
        {
            // マスター通信シーンに移行する
            Scene * scene { MasterScene::createScene() };
            Director::getInstance()->replaceScene( scene );
        }
            break;
        default:
            break;
    }
}

//---------------------------------------------------------------------------
// スレーブ通信ボタンイベント
//---------------------------------------------------------------------------
void TitleScene::BtnEventSlave(Ref *pSender, ui::Widget::TouchEventType type)
{
    switch( type ){
        case ui::Widget::TouchEventType::BEGAN:
        case ui::Widget::TouchEventType::MOVED:
            break;
        case ui::Widget::TouchEventType::CANCELED:
            break;
        case ui::Widget::TouchEventType::ENDED:
        {
            // マスター通信シーンに移行する
            Scene * scene { SlaveScene::createScene() };
            Director::getInstance()->replaceScene( scene );
        }
            break;
        default:
            break;
    }
}
