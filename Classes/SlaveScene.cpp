//******************************************************************************
//  SlaveScene.cpp
//  スレーブ通信シーン
//
//  Created by Iori Kobayashi on 21/3/29.
//******************************************************************************
#include "SlaveScene.h"
#include "TitleScene.h"
#include "BluetoothDriver.h"

//---------------------------------------------------------------------------
// define定義
//---------------------------------------------------------------------------
#define OBJNAME_BTNADVERTISE  "BtnAdv"

USING_NS_CC;

//---------------------------------------------------------------------------
// シーンのクリエイト
//---------------------------------------------------------------------------
Scene* SlaveScene::createScene()
{
    return SlaveScene::create();
}

//---------------------------------------------------------------------------
// 初期化処理
//---------------------------------------------------------------------------
bool SlaveScene::init()
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

    // メンバ変数の初期化
    m_bAdvertising = false;     // 最初はアドバタイズ停止状態

    // 背景スプライトの作成
    Sprite * back = Sprite::create( "white.png" );
    back->setColor( Color3B::BLACK );
    back->setPosition( Vec2(visibleSize.width / 2, visibleSize.height / 2) );
    back->setContentSize( visibleSize );
    this->addChild( back );

    // 戻るボタン
    ui::Button* pBtnReturn = ui::Button::create( "white.png" );
    pBtnReturn->setScale9Enabled( true );
    pBtnReturn->setContentSize( Size(50.0f, 30.0f) );
    pBtnReturn->setTitleColor( Color3B::RED );
    pBtnReturn->setTitleText( "戻る" );
    pBtnReturn->setTitleFontSize( 16.0f );
    pBtnReturn->setPosition( Vec2(visibleSize.width / 6, visibleSize.height / 20 * 19) );
    pBtnReturn->addTouchEventListener( CC_CALLBACK_2( SlaveScene::BtnEventReturn, this ));
    this->addChild( pBtnReturn, 0 );

    // タイトルテキストの作成
    ui::Text* pTextTitle = ui::Text::create( "Slave通信モード", "mplus-2c-medium.ttf", 16.0f );
    pTextTitle->setColor( Color3B::WHITE );
    pTextTitle->setPosition( Vec2(visibleSize.width / 2, visibleSize.height / 20 * 17) );
    this->addChild( pTextTitle );

    // アドバタイズボタン
    ui::Button* pBtnScan = ui::Button::create( "white.png" );
    pBtnScan->setScale9Enabled( true );
    pBtnScan->setContentSize( Size(200.0f, 30.0f) );
    pBtnScan->setTitleColor( Color3B::BLACK );
    pBtnScan->setTitleText( "アドバタイズ開始" );
    pBtnScan->setTitleFontSize( 16.0f );
    pBtnScan->setPosition( Vec2(visibleSize.width / 2, visibleSize.height / 20 * 15) );
    pBtnScan->addTouchEventListener( CC_CALLBACK_2( SlaveScene::BtnEventAdvertise, this ));
    this->addChild( pBtnScan, 0, OBJNAME_BTNADVERTISE );

    // 入力ボックスの設置
    ui::EditBox* pEdit = ui::EditBox::create( Size(150.0f, 30.0f), ui::Scale9Sprite::create("white.png") );
    pEdit->setFont( "mplus-2c-medium.ttf", 12.0f );
    pEdit->setFontColor( Color3B::BLACK );
    pEdit->setPlaceHolder( "Masterに送るデータ" );
    pEdit->setPlaceholderFontColor( Color3B::GRAY );
    pEdit->setPlaceholderFont( "mplus-2c-medium.ttf", 12.0f );
    pEdit->setMaxLength( 10 );
    pEdit->setText( "" );
    pEdit->setReturnType( ui::EditBox::KeyboardReturnType::DONE );
    pEdit->setInputMode( ui::EditBox::InputMode::EMAIL_ADDRESS );
    pEdit->setPosition( Vec2(visibleSize.width / 2, visibleSize.height / 2) );
    pEdit->setDelegate(this);
    this->addChild( pEdit, 0 );

    return true;
}

//---------------------------------------------------------------------------
// タイトルへ戻るボタンイベント
//---------------------------------------------------------------------------
void SlaveScene::BtnEventReturn(Ref *pSender, ui::Widget::TouchEventType type)
{
    switch( type ){
        case ui::Widget::TouchEventType::BEGAN:
        case ui::Widget::TouchEventType::MOVED:
            break;
        case ui::Widget::TouchEventType::CANCELED:
            break;
        case ui::Widget::TouchEventType::ENDED:
        {
            // アドバタイズを停止しておく
            BluetoothDriver::AdvertiseStop();
            // タイトルシーンに移行する
            Scene * scene { TitleScene::createScene() };
            Director::getInstance()->replaceScene( scene );
        }
            break;
        default:
            break;
    }
}

//---------------------------------------------------------------------------
// タイトルへ戻るボタンイベント
//---------------------------------------------------------------------------
void SlaveScene::BtnEventAdvertise(Ref *pSender, ui::Widget::TouchEventType type)
{
    switch( type ){
        case ui::Widget::TouchEventType::BEGAN:
        case ui::Widget::TouchEventType::MOVED:
            break;
        case ui::Widget::TouchEventType::CANCELED:
            break;
        case ui::Widget::TouchEventType::ENDED:
        {
            // ボタンオブジェクトの取得
            ui::Button * pBtn = (ui::Button*)this->getChildByName(OBJNAME_BTNADVERTISE);

            if( m_bAdvertising == true ){
                // アドバタイズの停止
                BluetoothDriver::AdvertiseStop();
                // フラグの切り替え
                m_bAdvertising = false;
                // 表示テキストの変更
                pBtn->setTitleText("アドバタイズ開始");
            }
            else{
                // アドバタイズ開始指示
                BluetoothDriver::AdvertiseStart( );
                // フラグの切り替え
                m_bAdvertising = true;
                // 表示テキストの変更
                pBtn->setTitleText("アドバタイズ停止");
            }
        }
            break;
        default:
            break;
    }
}

//---------------------------------------------------------------------------
// 開始デリゲート
//---------------------------------------------------------------------------
void SlaveScene::editBoxEditingDidBegin(ui::EditBox * pEdit)
{
    
}

//---------------------------------------------------------------------------
// 終了デリゲート
//---------------------------------------------------------------------------
void SlaveScene::editBoxEditingDidEnd(ui::EditBox * pEdit)
{
    std::string inputStr = pEdit->getText();
    BluetoothDriver::SetPeriToCentData(inputStr);
}

//---------------------------------------------------------------------------
// 変更検出デリゲート
//---------------------------------------------------------------------------
void SlaveScene::editBoxTextChanged(ui::EditBox * pEdit, const std::string& text)
{
    
}

//---------------------------------------------------------------------------
// リターン検出デリゲート
//---------------------------------------------------------------------------
void SlaveScene::editBoxReturn(ui::EditBox * pEdit)
{
    
}
