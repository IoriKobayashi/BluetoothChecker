/****************************************************************************
 Copyright (c) 2013      cocos2d-x.org
 Copyright (c) 2013-2016 Chukong Technologies Inc.
 Copyright (c) 2017-2018 Xiamen Yaji Software Co., Ltd.

 http://www.cocos2d-x.org

 Permission is hereby granted, free of charge, to any person obtaining a copy
 of this software and associated documentation files (the "Software"), to deal
 in the Software without restriction, including without limitation the rights
 to use, copy, modify, merge, publish, distribute, sublicense, and/or sell
 copies of the Software, and to permit persons to whom the Software is
 furnished to do so, subject to the following conditions:

 The above copyright notice and this permission notice shall be included in
 all copies or substantial portions of the Software.

 THE SOFTWARE IS PROVIDED "AS IS", WITHOUT WARRANTY OF ANY KIND, EXPRESS OR
 IMPLIED, INCLUDING BUT NOT LIMITED TO THE WARRANTIES OF MERCHANTABILITY,
 FITNESS FOR A PARTICULAR PURPOSE AND NONINFRINGEMENT. IN NO EVENT SHALL THE
 AUTHORS OR COPYRIGHT HOLDERS BE LIABLE FOR ANY CLAIM, DAMAGES OR OTHER
 LIABILITY, WHETHER IN AN ACTION OF CONTRACT, TORT OR OTHERWISE, ARISING FROM,
 OUT OF OR IN CONNECTION WITH THE SOFTWARE OR THE USE OR OTHER DEALINGS IN
 THE SOFTWARE.
 ****************************************************************************/

#import "RootViewController.h"
#import "cocos2d.h"
#import "platform/ios/CCEAGLView-ios.h"
#import <CoreBluetooth/CoreBluetooth.h>
#import <QuartzCore/QuartzCore.h>
#import "BluetoothDriver.h"

// 自らのインスタンス
static RootViewController *selfAddr = nil;

@interface RootViewController ()
<CBCentralManagerDelegate, CBPeripheralDelegate, CBPeripheralManagerDelegate>
{
    BOOL isScanning;
}

// マスター（セントラル）用
@property (nonatomic, strong) CBCentralManager *centralManager;
@property (nonatomic, strong) CBPeripheral *peripheralFromCentral;
@property (nonatomic, strong) NSMutableArray *findPeri;
@property (nonatomic, strong) CBCharacteristic *writeCharacteristic;
@property (nonatomic, strong) CBCharacteristic *readCharacteristic;
// スレーブ（ペリフェラル）用
@property (nonatomic, strong) CBPeripheralManager *peripheralManager;
@property (nonatomic, strong) CBMutableCharacteristic *periCharacteristic;
@end

@implementation RootViewController

/*
 // The designated initializer.  Override if you create the controller programmatically and want to perform customization that is not appropriate for viewDidLoad.
- (id)initWithNibName:(NSString *)nibNameOrNil bundle:(NSBundle *)nibBundleOrNil {
    if ((self = [super initWithNibName:nibNameOrNil bundle:nibBundleOrNil])) {
        // Custom initialization
    }
    return self;
}
*/

// Implement loadView to create a view hierarchy programmatically, without using a nib.
- (void)loadView {
    // Initialize the CCEAGLView
    CCEAGLView *eaglView = [CCEAGLView viewWithFrame: [UIScreen mainScreen].bounds
                                         pixelFormat: (__bridge NSString *)cocos2d::GLViewImpl::_pixelFormat
                                         depthFormat: cocos2d::GLViewImpl::_depthFormat
                                  preserveBackbuffer: NO
                                          sharegroup: nil
                                       multiSampling: cocos2d::GLViewImpl::_multisamplingCount > 0 ? YES : NO
                                     numberOfSamples: cocos2d::GLViewImpl::_multisamplingCount ];
    
    // Enable or disable multiple touches
    [eaglView setMultipleTouchEnabled:NO];
    
    // Set EAGLView as view of RootViewController
    self.view = eaglView;
}

// Implement viewDidLoad to do additional setup after loading the view, typically from a nib.
- (void)viewDidLoad {
    [super viewDidLoad];
    
    // セントラルマネージャのインスタンス
    self.centralManager = [[CBCentralManager alloc] initWithDelegate:self queue:nil];
    // ペリフェラル、マネージャのインスタンス
    self.peripheralManager = [[CBPeripheralManager alloc] initWithDelegate:self queue:nil options:nil];
    // 外部からのアクセス用
    selfAddr = self;
    self.findPeri = [NSMutableArray array];
}

- (void)viewWillAppear:(BOOL)animated {
    [super viewWillAppear:animated];
}

- (void)viewDidDisappear:(BOOL)animated {
    [super viewDidDisappear:animated];
}


// For ios6, use supportedInterfaceOrientations & shouldAutorotate instead
#ifdef __IPHONE_6_0
- (NSUInteger) supportedInterfaceOrientations{
    return UIInterfaceOrientationMaskAllButUpsideDown;
}
#endif

- (BOOL) shouldAutorotate {
    return YES;
}

- (void)didRotateFromInterfaceOrientation:(UIInterfaceOrientation)fromInterfaceOrientation {
    [super didRotateFromInterfaceOrientation:fromInterfaceOrientation];

    auto glview = cocos2d::Director::getInstance()->getOpenGLView();

    if (glview)
    {
        CCEAGLView *eaglview = (__bridge CCEAGLView *)glview->getEAGLView();

        if (eaglview)
        {
            CGSize s = CGSizeMake([eaglview getWidth], [eaglview getHeight]);
            cocos2d::Application::getInstance()->applicationScreenSizeChanged((int) s.width, (int) s.height);
        }
    }
}

//fix not hide status on ios7
- (BOOL)prefersStatusBarHidden {
    return YES;
}

// Controls the application's preferred home indicator auto-hiding when this view controller is shown.
// (better use preferredScreenEdgesDeferringSystemGestures for controlling the home indicator)
- (BOOL)prefersHomeIndicatorAutoHidden {
    return NO;
}

// HOME Indicator need to be tapped twice 
-(UIRectEdge)preferredScreenEdgesDeferringSystemGestures
{
    return UIRectEdgeBottom; 
}

- (void)didReceiveMemoryWarning {
    // Releases the view if it doesn't have a superview.
    [super didReceiveMemoryWarning];

    // Release any cached data, images, etc that aren't in use.
}

#pragma mark - CBCentralManagerDelegate
- (void)centralManagerDidUpdateState:(CBCentralManager *)central {
    // 何もしない
}

//---------------------------------------
// 接続切断時
- (void) centralManager:(CBCentralManager *)central
didDisconnectPeripheral:(CBPeripheral *)peripheral
                  error:(NSError *)error
{
    NSLog(@"切断検知");
    // 接続状態の更新
    BluetoothDriver::SetConnectResult(DISCONNECT);
}

//---------------------------------------
// ペリフェラル発見時
- (void) centralManager:(CBCentralManager *)central
  didDiscoverPeripheral:(CBPeripheral *)peripheral
      advertisementData:(NSDictionary *)advertisementData
                   RSSI:(NSNumber *)RSSI
{
    // 名称付きのものだけ抽出する
    if( peripheral.name != nil ){
        // 重複確認
        if ( [self.findPeri count] > 0 ){
            // 発券中のペリフェラル一覧をを走査
            for ( int nCnt = 0; nCnt < [self.findPeri count]; nCnt++ ){
                // ペリフェラルの取り出し
                CBPeripheral *peri = [self.findPeri objectAtIndex:nCnt];
                // CoreBluetoothではアドレスがわからないらしいのでidentifierで判定する
                if( peripheral.identifier == peri.identifier ){
                    // 一致していればこのペリフェラルは登録しない
                    return;
                }
                
            }
        }
        
        // ローカルネームが存在するならそちらを優先する
        NSString *name = [advertisementData objectForKey:@"kCBAdvDataLocalName"];
        if( name == nil ){
            name = peripheral.name;
        }
        
        NSLog(@"発見したデバイス：%@", name);
        // 配列にペリフェラルを追加
        [self.findPeri addObject:peripheral];
        // ドライバにペリフェラル名を登録
        BluetoothDriver::FindPeripheral( [name UTF8String] );
    }
}

//---------------------------------------
// 接続成功時
- (void) centralManager:(CBCentralManager *)central
   didConnectPeripheral:(CBPeripheral *)peripheral
{
    NSLog(@"接続成功");
    // デリゲートのセット
    peripheral.delegate = self;
    // サービス探索開始
    [peripheral discoverServices:nil];
}

//---------------------------------------
// 接続失敗時
- (void)    centralManager:(CBCentralManager *)central
didFailToConnectPeripheral:(CBPeripheral *)peripheral
                     error:(NSError *)error
{
    NSLog(@"接続失敗");
    // 接続状態の更新
    BluetoothDriver::SetConnectResult(CONNECT_NG);
}

#pragma mark - CBPeripheralDelegate
//---------------------------------------
// サービス発見時
- (void) peripheral:(CBPeripheral *)peripheral
didDiscoverServices:(NSError *)error
{
    if (error) {
        NSLog(@"サービス発見時エラー：%@", error);
        return;
    }
    
    // サービス一覧の取得
    NSArray *services = peripheral.services;
    NSLog(@"%lu 個のサービスを発見。", (unsigned long)services.count);
    
    for( CBService *service in services ) {
        NSLog(@"UUID:%@", service.UUID);
        // 指定のサービスUUIDか？
        if( [service.UUID isEqual:[CBUUID UUIDWithString: @SERVICE_UUID]] ){
            // キャラクタリスティック探索開始
            // 後に複数のキャラクタリスティックを取得することも考えて全捜査する(ただし速度は落ちる)
            [peripheral discoverCharacteristics:nil forService:service];
        }
    }
}

//---------------------------------------
// キャラクタリスティック発見時
- (void)                  peripheral:(CBPeripheral *)peripheral
didDiscoverCharacteristicsForService:(CBService *)service
                               error:(NSError *)error
{
    if (error) {
        NSLog(@"キャラクタリスティック発見時エラー：%@", error);
        return;
    }
    
    // サービスのキャラクタリスティック一覧を取得
    NSArray *characteristics = service.characteristics;
    NSLog(@"%lu 個のキャラクタリスティックを発見。%@", (unsigned long)characteristics.count, characteristics);
    
    for (CBCharacteristic *chara in characteristics) {
        // Readキャラクタリスティックと一致？
        if( [chara.UUID isEqual:[CBUUID UUIDWithString:@CHARACTERISTIC_UUID]] ){
            NSLog(@"READキャラクタリスティック発見");
            // キャラクタリスティックの保存
            self.readCharacteristic = chara;
            // 更新通知を受け取る設定にしておく（受信データ更新デリゲートが勝手に呼ばれるようになる）
            // ただし相手がnotifyを送ってこなければいけないので仕様を決める際に考慮すること
            [peripheral setNotifyValue:YES forCharacteristic:chara];
        }
        
        // Writeなど別キャラクタリスティックを探すときはここに書けばいい
    }
    
    // 接続中？
    if( BluetoothDriver::IsConnect() == CONNECTING ){
        if ( self.readCharacteristic != nil ) {
            // 接続結果の通知
            BluetoothDriver::SetConnectResult(CONNECT_OK);
        }
    }
}

//---------------------------------------
// データ書き込み完了時
// LINBLEはWriteWithoutResponseなので未使用
- (void)            peripheral:(CBPeripheral *)peripheral
didWriteValueForCharacteristic:(CBCharacteristic *)characteristic
                         error:(NSError *)error
{
    if (error) {
        NSLog(@"Write失敗...%@", error);
        return;
    }
    NSLog(@"Write成功");
}

//---------------------------------------
// データ読み込み完了時
- (void)             peripheral:(CBPeripheral *)peripheral
didUpdateValueForCharacteristic:(CBCharacteristic *)characteristic
                          error:(NSError *)error
{
    if (error) {
        NSLog(@"読み込み失敗...%@", error);
        return;
    }
    NSLog(@"読み込み成功");
    
    char* bytePtr = (char*)[characteristic.value bytes];
    std::string str(bytePtr, [characteristic.value length]);

    // ドライバに読み込みデータの登録
    BluetoothDriver::SetReadData(str);
}

#pragma mark - CBPeripheralManager Delegate
//---------------------------------------
// ペリフェラルマネージャの状態変化デリゲート
- (void)peripheralManagerDidUpdateState:(CBPeripheral *)peripheral {
    NSLog(@"ペリフェラル状態：%ld", (long)peripheral.state );
    
    if( peripheral.state == CBPeripheralManagerStatePoweredOn ){
        // ペリフェラルのサービス追加タイミングはCBPeripheralManagerStatePoweredOn以降なのでここで行う
        // テストなので簡略化しているが本来ならUUIDは128ビット必要
        CBUUID *serviceUUID = [CBUUID UUIDWithString:@SERVICE_UUID];
        CBMutableService *service = [[CBMutableService alloc] initWithType:serviceUUID primary:YES];
        
        // キャラクタリスティックの作成
        CBUUID *characteristicUUID = [CBUUID UUIDWithString:@CHARACTERISTIC_UUID];
        // キャラクタリスティックプロパティを準備する（今回はWriteなし）
        CBCharacteristicProperties properties = (CBCharacteristicPropertyRead | CBCharacteristicPropertyNotify);
        // キャラクタリスティックに対してReadのみ付与する
        self.periCharacteristic = [[CBMutableCharacteristic alloc]initWithType:characteristicUUID properties:properties value:nil permissions:CBAttributePermissionsReadable];
        
        // サービスにキャラクタリスティックの紐付けする(今回は一つ)
        service.characteristics = @[self.periCharacteristic];
        
        // ペリフェラルマネージャに対してサービスを紐付けする(完了デリゲート有)
        [self.peripheralManager addService:service];
    }
}

//---------------------------------------
// アドバタイズ開始処理の結果通知
- (void)peripheralManagerDidStartAdvertising:(CBPeripheralManager *)peripheral error:(NSError *)error {
    NSLog(@"アドバタイズ開始成功");
}

//---------------------------------------
// サービス追加結果の結果通知
- (void)peripheralManager:(CBPeripheralManager *)peripheral
            didAddService:(nonnull CBService *)service
                    error:(nullable NSError *)error
{
    if(error){
        // エラー出力
        NSLog(@"サービス追加失敗：%@", error);
    }
    else{
        NSLog(@"サービス追加成功：%@", service);
    }
}

//---------------------------------------
// Readリクエスト通知
- (void)peripheralManager:(CBPeripheralManager *)peripheral
    didReceiveReadRequest:(nonnull CBATTRequest *)request
{
    NSLog(@"Readリクエストを受信");
    
    // リクエストが自分へのリクエストなのかを判定する
    if( [request.characteristic.UUID isEqual:[CBUUID UUIDWithString:@CHARACTERISTIC_UUID]] ){
        // ユーザー側からの送信データの取得
        std::string str = BluetoothDriver::GetPeriToCentData();
        // NSData型に変換する
        NSData *data = [[NSData alloc] initWithBytes:str.data() length:str.length()];
        if( data == nil ){
            data = [@"ReadOK" dataUsingEncoding:NSUTF8StringEncoding];
        }
        // 応答データの設定
        request.value = data;
        // 応答を返す
        [self.peripheralManager respondToRequest:request withResult:CBATTErrorSuccess];
    }
}

//---------------------------------------
// Notify開始リクエスト通知
- (void)    peripheralManager:(CBPeripheralManager *)peripheral
                      central:(nonnull CBCentral *)central
 didSubscribeToCharacteristic:(nonnull CBCharacteristic *)characteristic
{
    NSLog(@"Notify開始リクエスト受信");
}

//---------------------------------------
// Notify停止リクエスト通知
- (void)            peripheralManager:(CBPeripheralManager *)peripheral
                              central:(nonnull CBCentral *)central
     didUnsubscribeFromCharacteristic:(nonnull CBCharacteristic *)characteristic
{
    NSLog(@"Notify停止リクエスト受信");
}

#pragma mark - Bluetooth Driver Action

//---------------------------------------
// インスタンス取得
+ (RootViewController*)getInstance {
    // 自らのインスタンスを返す
    return selfAddr;
}

//---------------------------------------
// スキャン開始
- (void)scanStart {
    // そもそもスキャン中ならスキャンを一旦停止する
    if ( isScanning == true ){
        [self scanStop];
    }
    
    // スキャン開始
    [self.centralManager scanForPeripheralsWithServices:nil options:nil];
    NSLog(@"Scan Start.");
    isScanning = true;
    
    // ペリフェラル情報の削除
    [self.findPeri removeAllObjects];
}

//---------------------------------------
// スキャン停止
- (void) scanStop {
    // そもそもスキャン停止中なら即終了
    if ( isScanning == false ){
        return;
    }
    
    // スキャン停止
    [self.centralManager stopScan];
    NSLog(@"Scan Stop.");
    isScanning = false;
}

//---------------------------------------
// 接続開始
- (void) connect:(int)peripheralIdx {
    // ペリフェラルの取得
    CBPeripheral* peripheral = [self.findPeri objectAtIndex:peripheralIdx];
    // スキャン停止
    [self scanStop];
    // 接続ペリフェラルを別途記憶する
    self.peripheralFromCentral = peripheral;
    // 接続
    [self.centralManager connectPeripheral:peripheral options:nil];
}

//---------------------------------------
// 切断
- (void) disConnect {
    // 接続ペリフェラルがないなら終わる
    if ( self.peripheralFromCentral == nil ){
        return;
    }
    // 切断する
    [self.centralManager cancelPeripheralConnection:self.peripheralFromCentral];
}

//---------------------------------------
// 書き込み
- (void) write:(unsigned char*)writeData {

}

//---------------------------------------
// 読み込み要求
- (void)readRequest {
    [self.peripheralFromCentral readValueForCharacteristic:self.readCharacteristic];
}

//---------------------------------------
// アドバタイズの開始処理
- (void)advertiseStart {
    // アドバタイズメントデータの作成(ローカルネームの準備)
    NSDictionary * advertisementData = @{CBAdvertisementDataLocalNameKey: @"BTChecker Device"};
    // アドバタイズの開始
    [self.peripheralManager startAdvertising:advertisementData];
}

//---------------------------------------
// アドバタイズの停止処理
- (void)advertiseStop {
    // アドバタイズを停止する
    [self.peripheralManager stopAdvertising];
    // アドバタイズ停止については処理完了デリゲートはないのでここで終了
}

//---------------------------------------
// Notify付きデータの設定
- (void)setDataWithNotify:(std::string)str
{
    // NSData型に変換する
    NSData *data = [[NSData alloc] initWithBytes:str.data() length:str.length()];
    if( data == nil ){
        data = [@"ReadOK" dataUsingEncoding:NSUTF8StringEncoding];
    }
    
    self.periCharacteristic.value = data;
    
    // Notify通知を行う
    [self.peripheralManager updateValue:data forCharacteristic:self.periCharacteristic onSubscribedCentrals:nil];
}

@end
