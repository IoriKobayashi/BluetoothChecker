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


#ifndef __PLAYER_FILE_DIALOG_SERVICE_WIN_H_
#define __PLAYER_FILE_DIALOG_SERVICE_WIN_H_

#include "stdafx.h"
#include "PlayerFileDialogServiceProtocol.h"

PLAYER_NS_BEGIN

class PlayerFileDialogServiceWin : public PlayerFileDialogServiceProtocol
{
public:
    PlayerFileDialogServiceWin(HWND hwnd);

    virtual std::string openFile(const std::string &title,
                                 const std::string &directory,
                                 const std::string &extensions) const;
    virtual std::vector<std::string> openMultiple(const std::string &title,
                                                  const std::string &directory,
                                                  const std::string &extensions) const;
    virtual std::string saveFile(const std::string &title,
                                 const std::string &path) const;
    virtual std::string openDirectory(const std::string &title,
                                      const std::string &directory) const;

protected:
    HWND _hwnd;

    LPTSTR parseExtensions(const std::string &extensions) const;
    vector<std::string> openMultipleInternal(const std::string &title,
                                             const std::string &directory,
                                             const std::string &extensions,
                                             bool isMulti) const;
};

PLAYER_NS_END

#endif // __PLAYER_FILE_DIALOG_SERVICE_WIN_H_
