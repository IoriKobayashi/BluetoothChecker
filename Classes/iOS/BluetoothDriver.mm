//
//  BluetoothDriver.mm
//  Bluetooth通信において、CocosとObjective-Cを繋ぐ
//
//  Created by Iori Kobayashi on 21/3/29.
#include "BluetoothDriver.h"
#import "RootViewController.h"

BT_INFO BluetoothDriver::sm_periInfo[MAX_PERIPHERAL_INFO];
int BluetoothDriver::sm_infoVal = 0;
BT_CONNECT BluetoothDriver::sm_conResult = DISCONNECT;
unsigned char BluetoothDriver::sm_readData[512] = {};
int BluetoothDriver::sm_nCmdVal = 0;
std::string BluetoothDriver::sm_periToCent = "";

//--------------------------------------
// スキャン開始
//--------------------------------------
void BluetoothDriver::ScanStart( void )
{
    // ルートのインスタンス
    RootViewController* con = [RootViewController getInstance];
    
    // スキャン開始
    [con scanStart];
    
    // ペリフェラル情報数のクリア
    sm_infoVal = 0;
    
    // 読み込みデータのクリア
    sm_readData[0] = 0;
}

//--------------------------------------
// スキャン停止
//--------------------------------------
void BluetoothDriver::ScanStop( void )
{
    // ルートのインスタンス
    RootViewController* con = [RootViewController getInstance];
    // スキャン停止
    [con scanStop];
}

//--------------------------------------
// 接続開始
//--------------------------------------
void BluetoothDriver::Connect( int idx )
{
    // ルートのインスタンス
    RootViewController* con = [RootViewController getInstance];
    
    if ( idx > sm_infoVal ){
        return;
    }
    
    // 接続状態の更新
    sm_conResult = CONNECTING;
    // 接続開始
    [con connect:idx];
}

//--------------------------------------
// 切断
//--------------------------------------
void BluetoothDriver::DisConnect( void )
{
    // ルートのインスタンス
    RootViewController* con = [RootViewController getInstance];
    
    [con disConnect];
    // 接続ステータスはデリゲート側から上書きする
}

//--------------------------------------
// 書き込み
//--------------------------------------
void BluetoothDriver::Write( unsigned char* msg )
{
    // ルートのインスタンス
    RootViewController* con = [RootViewController getInstance];
    
    [con write:msg];
}

//--------------------------------------
// 読み込み
//--------------------------------------
int BluetoothDriver::Read( unsigned char* msg )
{
    // 読み込みデータがないなら即終了
    if( sm_nCmdVal == 0 ) {
        return 0;
    }
    
    memcpy(msg, sm_readData, sm_nCmdVal);
    
    int nCmdVal = sm_nCmdVal;
    // クリア
    sm_readData[0] = 0;
    sm_nCmdVal = 0;
    
    return nCmdVal;
}

//--------------------------------------
// アドバタイズの開始
//--------------------------------------
void BluetoothDriver::AdvertiseStart( void )
{
    // ルートのインスタンス
    RootViewController* con = [RootViewController getInstance];
    // アドバタイズを開始する
    [con advertiseStart];
}

//--------------------------------------
// アドバタイズの停止
//--------------------------------------
void BluetoothDriver::AdvertiseStop( void )
{
    // ルートのインスタンス
    RootViewController* con = [RootViewController getInstance];
    // アドバタイズを停止する
    [con advertiseStop];
}

//--------------------------------------
// ペリフェラルからセントラルに送るデータ（セントラルからのRead）
//--------------------------------------
void BluetoothDriver::SetPeriToCentData( std::string str )
{
    // 送信データは上書きする
    sm_periToCent = str;
}

//------------------------------------------------------------------------------
//--------------------------------------
// ペリフェラル発見
//--------------------------------------
void BluetoothDriver::FindPeripheral( std::string name )
{
    // ペリフェラル情報保存上限以下？
    if( sm_infoVal < MAX_PERIPHERAL_INFO ) {
        // 名前の登録
        sm_periInfo[sm_infoVal].name = name;
        // ペリフェラル情報登録数
        sm_infoVal++;
    }
}

//--------------------------------------
// 接続結果通知
//--------------------------------------
void BluetoothDriver::SetConnectResult( BT_CONNECT con )
{
    sm_conResult = con;
}

//--------------------------------------
// 読み込みデータの設定
//--------------------------------------
void BluetoothDriver::SetReadData(unsigned char *msg)
{

}

//--------------------------------------
// ペリフェラルからセントラルに送るデータの取得
//--------------------------------------
std::string BluetoothDriver::GetPeriToCentData(void)
{
    return sm_periToCent;
}
