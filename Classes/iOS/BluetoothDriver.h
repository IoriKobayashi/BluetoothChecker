//
//  BluetoothDriver.h
//  Bluetooth通信において、CocosとObjective-Cを繋ぐ
//
//  Created by Iori Kobayashi on 21/3/29.
#ifndef __BLUETOOTH_DRIVER_H__
#define __BLUETOOTH_DRIVER_H__

#define MAX_PERIPHERAL_INFO (50)    // 最大50まで
// キャラクタリスティック定数
#define SERVICE_UUID            ("0123")
#define CHARACTERISTIC_UUID     ("ABCD")

// Bluetooth情報構造体
typedef struct{
    std::string name;   //デバイス名称
    // 名称以外に必要になる可能性があるため構造体としておく
}BT_INFO;

// 接続状態列挙体
typedef enum{
    DISCONNECT = 0,     // 未接続/切断
    CONNECTING,         // 接続試行中
    CONNECT_OK,         // 接続完了
    CONNECT_NG          // 接続失敗
}BT_CONNECT;

class BluetoothDriver
{
public:
    // cocos向け
    static void ScanStart();        // スキャン開始
    static void ScanStop();         // スキャン停止
    static void Connect( int );     // 接続開始
    static void DisConnect( );       // 切断
    static void Write( unsigned char* );    // 書き込み
    static void ReadRequest();      // 読み込み開始要求
    static std::string Read();     // 読み込み
    
    static void AdvertiseStart();   // アドバタイズの開始
    static void AdvertiseStop();    // アドバタイズの停止
    static void SetPeriToCentData( std::string );   // ペリフェラルからセントラルに送るデータの設定
    static void SetPeriToCentDataWithNotify( std::string );
    
    // 接続結果の取得
    static BT_CONNECT IsConnect(){ return sm_conResult; };
    // ペリフェラル情報配列の取得
    static int GetPeripheralInfo(BT_INFO** info){
        *info = sm_periInfo;
        return sm_infoVal;
    }
    
    // Object-C向け
    static void FindPeripheral( std::string );      // ペリフェラル発見
    static void SetConnectResult( BT_CONNECT );     // 接続結果通知
    static void SetReadData( std::string );         // 読み込みデータ設定
    static std::string GetPeriToCentData();         // ペリフェラルからセントラルに送るデータ
    
private:
    static BT_INFO sm_periInfo[MAX_PERIPHERAL_INFO];    // ペリフェラル情報配列
    static int sm_infoVal;                              // ペリフェラル情報数
    static BT_CONNECT sm_conResult;                     // 接続結果
    static std::string sm_periToCent;                   // ペリフェラルからセントラルに送るデータ
    static std::string sm_readDataFromPeri;             // ペリフェラルから受けた読み込みデータ
};

#endif
