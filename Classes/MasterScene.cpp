//******************************************************************************
//  MasterScene.cpp
//  マスター通信シーン
//
//  Created by Iori Kobayashi on 21/3/29.
//******************************************************************************
#include "MasterScene.h"
#include "TitleScene.h"
#include "BluetoothDriver.h"

//---------------------------------------------------------------------------
// define定義
//---------------------------------------------------------------------------
#define MAX_SCANDEVICE  (10)
#define OBJNAME_BTNSCAN  "BtnScan"
#define OBJNAME_TEXTDEVICE(id)  StringUtils::format("TextDevice%d", id)
#define OBJNAME_BTNDEVICE(id)   StringUtils::format("BtnDevice%d", id)
#define OBJNAME_BTNDISCONNECT   "BtnDisconnect"
#define OBJNAME_BTNREAD         "BtnRead"
#define OBJNAME_TEXTREAD        "TextRead"

USING_NS_CC;

//---------------------------------------------------------------------------
// シーンのクリエイト
//---------------------------------------------------------------------------
Scene* MasterScene::createScene()
{
    return MasterScene::create();
}

//---------------------------------------------------------------------------
// 初期化処理
//---------------------------------------------------------------------------
bool MasterScene::init()
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
    m_bScaning = false;     // 最初はスキャン停止状態
    m_nFoundPeri = 0;       // ペリフェラル数なし

    // 背景スプライトの作成
    Sprite *back = Sprite::create( "white.png" );
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
    pBtnReturn->addTouchEventListener( CC_CALLBACK_2( MasterScene::BtnEventReturn, this ));
    this->addChild( pBtnReturn, 0 );

    // タイトルテキストの作成
    ui::Text *pTextTitle = ui::Text::create( "Master通信モード", "mplus-2c-medium.ttf", 16.0f );
    pTextTitle->setColor( Color3B::WHITE );
    pTextTitle->setPosition( Vec2(visibleSize.width / 2, visibleSize.height / 20 * 18) );
    this->addChild( pTextTitle );

    // スキャン開始ボタン
    ui::Button* pBtnScan = ui::Button::create( "white.png" );
    pBtnScan->setScale9Enabled( true );
    pBtnScan->setContentSize( Size(100.0f, 25.0f) );
    pBtnScan->setTitleColor( Color3B::BLACK );
    pBtnScan->setTitleText( "スキャン開始" );
    pBtnScan->setTitleFontSize( 16.0f );
    pBtnScan->setPosition( Vec2(visibleSize.width / 2, visibleSize.height / 20 * 16) );
    pBtnScan->addTouchEventListener( CC_CALLBACK_2( MasterScene::BtnEventScan, this ));
    this->addChild( pBtnScan, 0, OBJNAME_BTNSCAN );

    for( int nCnt = 0; nCnt < MAX_SCANDEVICE; nCnt++ ){
        // 検出デバイス名表示テキスト
        ui::Text *pTextBt = ui::Text::create( "----", "mplus-2c-medium.ttf", 10.0f );
        pTextBt->setColor( Color3B::WHITE );
        pTextBt->setAnchorPoint( Vec2(0.0f, 0.5f) );
        pTextBt->setPosition( Vec2(visibleSize.width / 8, visibleSize.height / 20 * (14 - nCnt)) );
        pTextBt->setContentSize( Size(200.0f, 20.0f) );
        pTextBt->setTextAreaSize( Size(200.0f, 20.0f) );
        pTextBt->setTextHorizontalAlignment(TextHAlignment::LEFT);
        this->addChild( pTextBt, 0, OBJNAME_TEXTDEVICE(nCnt) );

        // 接続ボタン
        ui::Button *pBtnConnect = ui::Button::create( "white.png" );
        pBtnConnect->setScale9Enabled( true );
        pBtnConnect->setContentSize( Size(50.0f, 20.0f) );
        pBtnConnect->setTitleColor( Color3B::GRAY );
        pBtnConnect->setTitleText( "接続" );
        pBtnConnect->setTitleFontSize( 10.0f );
        pBtnConnect->setPosition( Vec2(visibleSize.width / 8 * 7, pTextBt->getPosition().y) );
        pBtnConnect->setEnabled( false );
        pBtnConnect->addTouchEventListener( CC_CALLBACK_2( MasterScene::BtnEventConnect, this ));
        pBtnConnect->setTag(nCnt);
        this->addChild( pBtnConnect, 0, OBJNAME_BTNDEVICE(nCnt) );
    }

    // 読み込みボタン
    ui::Button* pBtnRead = ui::Button::create( "white.png" );
    pBtnRead->setScale9Enabled( true );
    pBtnRead->setContentSize( Size(80.0f, 25.0f) );
    pBtnRead->setTitleColor( Color3B::GRAY );
    pBtnRead->setTitleText( "読み込み" );
    pBtnRead->setTitleFontSize( 16.0f );
    pBtnRead->setPosition( Vec2(visibleSize.width / 4, visibleSize.height / 20 * 4) );
    pBtnRead->addTouchEventListener( CC_CALLBACK_2( MasterScene::BtnEventRead, this ));
    pBtnRead->setEnabled( false );
    this->addChild( pBtnRead, 0, OBJNAME_BTNREAD );
    
    // 切断ボタン
    ui::Button* pBtnDiscon = ui::Button::create( "white.png" );
    pBtnDiscon->setScale9Enabled( true );
    pBtnDiscon->setContentSize( Size(80.0f, 25.0f) );
    pBtnDiscon->setTitleColor( Color3B::GRAY );
    pBtnDiscon->setTitleText( "切断" );
    pBtnDiscon->setTitleFontSize( 16.0f );
    pBtnDiscon->setPosition( Vec2(visibleSize.width / 4 * 3, visibleSize.height / 20 * 4) );
    pBtnDiscon->addTouchEventListener( CC_CALLBACK_2( MasterScene::BtnEventDisconnect, this ));
    pBtnDiscon->setEnabled( false );
    this->addChild( pBtnDiscon, 0, OBJNAME_BTNDISCONNECT );
    
    // 読み込みデータ表示
    ui::Text * pTextRead = ui::Text::create( "Read Data:", "mplus-2c-medium.ttf", 10.0f );
    pTextRead->setPosition( Vec2(visibleSize.width / 2, visibleSize.height / 20 * 2) );
    pTextRead->setColor( Color3B::WHITE );
    pTextRead->setContentSize( Size(300.0f, 20.0f) );
    pTextRead->setTextAreaSize( Size(300.0f, 20.0f) );
    pTextRead->setTextHorizontalAlignment(TextHAlignment::CENTER);
    this->addChild( pTextRead, 0, OBJNAME_TEXTREAD );

    return true;
}

//---------------------------------------------------------------------------
// スキャン中更新処理
//---------------------------------------------------------------------------
void MasterScene::UpdateScan(float fTime)
{
    // 変数宣言
    int nPeriVal = 0;           // ペリフェラル数
    BT_INFO *info;              // ペリフェラル情報
    
    // ペリフェラル情報の取得
    nPeriVal = BluetoothDriver::GetPeripheralInfo(&info);
    
    // 一つ以上見つかった？
    if( nPeriVal > 0 ){
        // すでに見つかった数と一緒なら抜ける
        if( m_nFoundPeri >= nPeriVal ){
            return;
        }
        
        for( int nCnt = m_nFoundPeri; nCnt < nPeriVal; nCnt++ ){
            if( nCnt >= MAX_SCANDEVICE ){
                break;
            }
            
            // ペリフェラル名称の設定
            ui::Text * pText = (ui::Text*)this->getChildByName( OBJNAME_TEXTDEVICE(nCnt) );
            pText->setString(info[nCnt].name);
            
            // 接続ボタンの停止
            ui::Button * pBtn = (ui::Button*)this->getChildByName( OBJNAME_BTNDEVICE(nCnt) );
            pBtn->setTitleColor(Color3B::BLACK);
            pBtn->setEnabled(true);
        }
        
        // 発見済の数値更新
        m_nFoundPeri = nPeriVal;
    }
}

//---------------------------------------------------------------------------
// 接続中中更新処理
//---------------------------------------------------------------------------
void MasterScene::UpdateConnect(float fTime)
{
    static BT_CONNECT oldStatus = DISCONNECT;       // 直前の接続状態
    // 接続ステータスの取得
    BT_CONNECT btStatus = BluetoothDriver::IsConnect();

    // 接続試行中？
    if( btStatus == CONNECTING ){
        oldStatus = btStatus;
    }
    // 切断？
    else if( btStatus == DISCONNECT ){
        // 切断時は各種ボタンの無効化など設定が必要
        // 読み込みボタン
        ui::Button * pBtnRead = (ui::Button*)this->getChildByName( OBJNAME_BTNREAD );
        pBtnRead->setEnabled( false );
        pBtnRead->setTitleColor( Color3B::GRAY );
        // 切断ボタン
        ui::Button * pBtnDisCon = (ui::Button*)this->getChildByName( OBJNAME_BTNDISCONNECT );
        pBtnDisCon->setEnabled( false );
        pBtnDisCon->setTitleColor( Color3B::GRAY );
        // 更新を止める
        this->unschedule( schedule_selector(MasterScene::UpdateConnect) );
        
    }
    else if( btStatus == CONNECT_OK ){
        // 直前まで試行中だった？
        if( oldStatus == CONNECTING ){
            // 接続完了に切り替わったタイミングなのでボタンを有効化しておく
            // 読み込みボタン
            ui::Button * pBtnRead = (ui::Button*)this->getChildByName( OBJNAME_BTNREAD );
            pBtnRead->setEnabled( true );
            pBtnRead->setTitleColor( Color3B::BLACK );
            // 切断ボタン
            ui::Button * pBtnDisCon = (ui::Button*)this->getChildByName( OBJNAME_BTNDISCONNECT );
            pBtnDisCon->setEnabled( true );
            pBtnDisCon->setTitleColor( Color3B::BLUE );
        }
        // 読み込み要求を送る
        //BluetoothDriver::ReadRequest();
        // マスターから指示せず切断することがあるため、接続完了しても更新は止めない
    }

    // 直前の接続状態を更新して終わる
    oldStatus = btStatus;
}

//---------------------------------------------------------------------------
// タイトルへ戻るボタンイベント
//---------------------------------------------------------------------------
void MasterScene::BtnEventReturn(Ref *pSender, ui::Widget::TouchEventType type)
{
    switch( type ){
        case ui::Widget::TouchEventType::BEGAN:
        case ui::Widget::TouchEventType::MOVED:
            break;
        case ui::Widget::TouchEventType::CANCELED:
            break;
        case ui::Widget::TouchEventType::ENDED:
        {
            // スキャンを停止しておく
            BluetoothDriver::ScanStop();
            // 切断しておく
            BluetoothDriver::DisConnect();

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
// スキャンボタンイベント
//---------------------------------------------------------------------------
void MasterScene::BtnEventScan(Ref *pSender, ui::Widget::TouchEventType type)
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
            ui::Button * pBtn = (ui::Button*)this->getChildByName( OBJNAME_BTNSCAN );
            
            if( m_bScaning == true ){
                // スキャンの停止
                BluetoothDriver::ScanStop();
                // フラグの切り替え
                m_bScaning = false;
                // 表示テキストの変更
                pBtn->setTitleText( "スキャン開始" );
                // 定期更新処理の停止
                this->unschedule( schedule_selector(MasterScene::UpdateScan) );
            }
            else{
                // スキャン開始指示
                BluetoothDriver::ScanStart( );
                // フラグの切り替え
                m_bScaning = true;
                // 表示テキストの変更
                pBtn->setTitleText( "スキャン停止" );
                // 定期更新処理の追加
                this->schedule( schedule_selector(MasterScene::UpdateScan), 1.0f );
            }
        }
            break;
        default:
            break;
    }
}

//---------------------------------------------------------------------------
// 接続ボタンイベント
//---------------------------------------------------------------------------
void MasterScene::BtnEventConnect(Ref *pSender, ui::Widget::TouchEventType type)
{
    switch( type ){
        case ui::Widget::TouchEventType::BEGAN:
        case ui::Widget::TouchEventType::MOVED:
            break;
        case ui::Widget::TouchEventType::CANCELED:
            break;
        case ui::Widget::TouchEventType::ENDED:
        {
            if( m_bScaning == true ){
                // スキャンを停止しておく
                BluetoothDriver::ScanStop();
                // スキャン中更新処理の停止
                this->unschedule( schedule_selector(MasterScene::UpdateScan) );
            }

            // ボタンの取得
            ui::Button * pBtn = static_cast<ui::Button*>(pSender);
            // タグ番号のデバイスを指定して接続(接続結果はここではわからない)
            BluetoothDriver::Connect( pBtn->getTag() );
            // 接続中更新処理を設定する
            this->schedule( schedule_selector(MasterScene::UpdateConnect), 1.0f );
        }
            break;
        default:
            break;
    }
}

//---------------------------------------------------------------------------
// 読み込みボタンイベント
//---------------------------------------------------------------------------
void MasterScene::BtnEventRead(Ref *pSender, ui::Widget::TouchEventType type)
{
    switch( type ){
        case ui::Widget::TouchEventType::BEGAN:
        case ui::Widget::TouchEventType::MOVED:
            break;
        case ui::Widget::TouchEventType::CANCELED:
            break;
        case ui::Widget::TouchEventType::ENDED:
        {
            // ドライバから読み込みデータの取得
            std::string readData = BluetoothDriver::Read();
            ui::Text * pText = (ui::Text*)this->getChildByName(OBJNAME_TEXTREAD);
            pText->setText( StringUtils::format("Read Data:%s", readData.c_str()) );
        }
            break;
        default:
            break;
    }
}

//---------------------------------------------------------------------------
// 切断ボタンイベント
//---------------------------------------------------------------------------
void MasterScene::BtnEventDisconnect(Ref *pSender, ui::Widget::TouchEventType type)
{
    switch( type ){
        case ui::Widget::TouchEventType::BEGAN:
        case ui::Widget::TouchEventType::MOVED:
            break;
        case ui::Widget::TouchEventType::CANCELED:
            break;
        case ui::Widget::TouchEventType::ENDED:
        {
            // 切断する（未接続状態の呼び出しは問題なし）
            BluetoothDriver::DisConnect();
            // 更新処理を止める
            this->unschedule( schedule_selector(MasterScene::UpdateConnect) );
            // ボタンを無効化しておく
            // 読み込みボタン
            ui::Button * pBtnRead = (ui::Button*)this->getChildByName( OBJNAME_BTNREAD );
            pBtnRead->setEnabled( false );
            pBtnRead->setTitleColor( Color3B::GRAY );
            // 切断ボタン
            ui::Button * pBtnDisCon = (ui::Button*)this->getChildByName( OBJNAME_BTNDISCONNECT );
            pBtnDisCon->setEnabled( false );
            pBtnDisCon->setTitleColor( Color3B::GRAY );
        }
            break;
        default:
            break;
    }
}
