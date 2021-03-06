/****************************************************************************
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

#include <ui/UITextField.h>
#include <ui/UIButton.h>
#include "HelloWorldScene.h"
#include "SimpleAudioEngine.h"
#include "BluetoothDriver.h"

USING_NS_CC;

Scene* HelloWorld::createScene()
{
    return HelloWorld::create();
}

// Print useful error message instead of segfaulting when files are not there.
static void problemLoading(const char* filename)
{
    printf("Error while loading: %s\n", filename);
    printf("Depending on how you compiled you might have to add 'Resources/' in front of filenames in HelloWorldScene.cpp\n");
}

// on "init" you need to initialize your instance
bool HelloWorld::init()
{
    //////////////////////////////
    // 1. super init first
    if ( !Scene::init() )
    {
        return false;
    }

    // ???????????????
    m_bScan = false;

    this->setPosition( Vec2(0.0f, 0.0f) );

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    Sprite *back = Sprite::create("white.png");
    back->setColor(Color3B::BLACK);
    back->setPosition(Vec2(visibleSize.width/2, visibleSize.height/2));
    back->setContentSize(visibleSize);
    this->addChild(back);

    // button
    ui::Button* pBtnScan = ui::Button::create("white.png");
    pBtnScan->setScale9Enabled(true);
    pBtnScan->setContentSize( Size( 100.0f, 30.0f ) );
    pBtnScan->setTitleColor(Color3B::BLACK);
    pBtnScan->setTitleText("Scan Start.");
    pBtnScan->setTitleFontSize( 16.0f );
    pBtnScan->setPosition( Vec2(visibleSize.width / 2, visibleSize.height / 10 * 9) );
    pBtnScan->addTouchEventListener( CC_CALLBACK_2( HelloWorld::BtnEventScanStart, this ));
    this->addChild(pBtnScan, 0, "ScanBtn");
    
    // 3??????
    for(int nCnt = 0; nCnt < 3; nCnt++){
        // Text
        ui::Text * pBTName = ui::Text::create("Non.", "arial.ttf", 16.0f);
        pBTName->setColor(Color3B::WHITE);
        pBTName->setPosition(Vec2( visibleSize.width / 4, visibleSize.height / 10 * ( 8 - nCnt ) ));
        this->addChild( pBTName, 0, StringUtils::format("Text%d", nCnt) );
        
        // Btn
        ui::Button * pBtnConnect = ui::Button::create("white.png");
        pBtnConnect->setScale9Enabled(true);
        pBtnConnect->setContentSize(Size( 100.0f, 30.0f ));
        pBtnConnect->setTitleText("Connect");
        pBtnConnect->setTitleColor(Color3B::GRAY);
        pBtnConnect->setTitleFontSize(16.0f);
        pBtnConnect->setPosition(Vec2( visibleSize.width / 4 * 3, visibleSize.height / 10 * ( 8 - nCnt ) ));
        pBtnConnect->setTag(nCnt);
        pBtnConnect->setEnabled(false);
        pBtnConnect->addTouchEventListener( CC_CALLBACK_2( HelloWorld::BtnEventScanStart, this ));
        this->addChild(pBtnConnect, 0, StringUtils::format("BtnConnect%d", nCnt) );
    }
    // ???????????????
    m_bScan = false;

    return true;
}

//---------------------------------------------------------------------------
//!    @brief        ???????????????????????????????????????
//!    @note
//---------------------------------------------------------------------------
void HelloWorld::BtnEventScanStart(Ref *pSender, ui::Widget::TouchEventType type){
    switch( type ){
        case ui::Widget::TouchEventType::BEGAN:
        case ui::Widget::TouchEventType::MOVED:
            break;
        case ui::Widget::TouchEventType::CANCELED:
            break;
        case ui::Widget::TouchEventType::ENDED:
        {
            // ????????????????????????????????????
            ui::Button * pBtn = (ui::Button*)this->getChildByName("ScanBtn");
            
            if( m_bScan == true ){
                // ?????????????????????
                BluetoothDriver::ScanStop();
                // ????????????????????????
                m_bScan = false;
                // ???????????????????????????
                pBtn->setTitleText("Scan Start.");
                // ???????????????????????????
                this->unschedule(schedule_selector(HelloWorld::UpdateScan));
            }
            else{
                // ????????????????????????
                BluetoothDriver::ScanStart( );
                // ????????????????????????
                m_bScan = true;
                // ???????????????????????????
                pBtn->setTitleText("Scan Stop.");
                // ???????????????????????????
                this->schedule(schedule_selector(HelloWorld::UpdateScan), 1.0f);
            }
        }
            break;
        default:
            break;
    }
}

//---------------------------------------------------------------------------
//!    @brief        ???????????????????????????
//!    @note
//---------------------------------------------------------------------------
void HelloWorld::BtnEventConnect(Ref *pSender, ui::Widget::TouchEventType type)
{
    switch( type ){
        case ui::Widget::TouchEventType::BEGAN:
        case ui::Widget::TouchEventType::MOVED:
            break;
        case ui::Widget::TouchEventType::CANCELED:
            break;
        case ui::Widget::TouchEventType::ENDED:
        {
            // ????????????????????????????????????
            ui::Button * pBtn = dynamic_cast<ui::Button*>(pSender);
            // ???????????????
            int nTag = pBtn->getTag();
        }
            break;
        default:
            break;
    }
}

//---------------------------------------------------------------------------
//!    @brief        ?????????????????????????????????
//!    @note
//---------------------------------------------------------------------------
void HelloWorld::UpdateScan(float fTime)
{
    // ????????????
    static int nFinded = 0;     // ???????????????
    int nPeriVal = 0;           // ?????????????????????
    BT_INFO *info;              // ????????????????????????
    
    // ?????????????????????????????????
    nPeriVal = BluetoothDriver::GetPeripheralInfo(&info);
    
    // ??????????????????????????????
    if( nPeriVal == 0 ){
        // ???????????????????????????????????????????????????
        if( nFinded >= nPeriVal ){
            return;
        }
        
        for( int nCnt = nFinded; nCnt < nPeriVal; nCnt++ ){
            // ???????????????3?????????
            if( nCnt > 2 ){
                break;
            }
            
            // ?????????????????????????????????
            ui::Text * pText = (ui::Text*)this->getChildByName( StringUtils::format("Text%d", nCnt) );
            pText->setString(info[nCnt].name);
            
            // ????????????????????????
            ui::Button * pBtn = (ui::Button*)this->getChildByName( StringUtils::format("BtnConnect%d", nCnt) );
            pBtn->setTitleColor(Color3B::BLACK);
            pBtn->setEnabled(true);
        }
        
        // ????????????????????????
        nFinded = nPeriVal;
    }
}
