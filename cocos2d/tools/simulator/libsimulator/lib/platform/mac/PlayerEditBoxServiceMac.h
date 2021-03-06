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

//
//  EditBoxServiceMac.h
//  player
//

#ifndef __player__EditBoxServiceMac__
#define __player__EditBoxServiceMac__

#include "PlayerEditBoxServiceProtocol.h"


@interface EditBoxServiceImplMac : NSObject <NSTextFieldDelegate>
{
    NSTextField* textField_;
    void* editBox_;
    BOOL editState_;
    NSMutableDictionary* placeholderAttributes_;
}

@property(nonatomic, retain) NSTextField* textField;
@property(nonatomic, retain) NSMutableDictionary* placeholderAttributes;
@property(nonatomic, readonly, getter = isEditState) BOOL editState;
@property(nonatomic, assign) void* editBox;

-(id) initWithFrame: (NSRect) frameRect editBox: (void*) editBox;
-(void) doAnimationWhenKeyboardMoveWithDuration:(float)duration distance:(float)distance;
-(void) setPosition:(NSPoint) pos;
-(void) setContentSize:(NSSize) size;
-(void) visit;
-(void) openKeyboard;
-(void) closeKeyboard;

@end


PLAYER_NS_BEGIN
class PlayerEditBoxServiceMac : public PlayerEditBoxServiceProtocol
{
public:
    PlayerEditBoxServiceMac();
    virtual ~PlayerEditBoxServiceMac();
    
    // overwrite
    virtual void showSingleLineEditBox(const cocos2d::Rect &rect) ;
    virtual void showMultiLineEditBox(const cocos2d::Rect &rect)  ;
    virtual void hide() ;
    
    virtual void setText(const std::string &text);
    virtual void setFont(const std::string &name, int size);
    virtual void setFontColor(const cocos2d::Color3B &color);
    
    virtual void setFormator(int formator);
private:
    void show();
    
private:
    EditBoxServiceImplMac*  _sysEdit;
};

PLAYER_NS_END

#endif
