//
//  OptionDialog.hpp
//  Sevens
//
//  Created by shinnichirou.takahashi on 2017/04/12.
//
//

#ifndef OptionDialog_h
#define OptionDialog_h

//#define kModalLayerPriority kCCMenuHandlerPriority-1
#define kModalLayerPriority -129

#define CLOSE_BUTTON_TAG 1
#define SOUND_BUTTON_ON_TAG 2
#define SOUND_BUTTON_OFF_TAG 3
#define SPEED_BUTTON_SLOW_TAG 4
#define SPEED_BUTTON_NORMAL_TAG 5
#define SPEED_BUTTON_QUICK_TAG 6
#define JOKER_BUTTON_ON_TAG 7
#define JOKER_BUTTON_OFF_TAG 8
#define TUNNEL_BUTTON_ON_TAG 9
#define TUNNEL_BUTTON_OFF_TAG 10
#define NET_BUTTON_ON_TAG 11
#define NET_BUTTON_OFF_TAG 12

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "audio/include/AudioEngine.h"
#include "Common.h"

USING_NS_CC;

class OptionDialog : public Layer
{
public:
    CREATE_FUNC(OptionDialog);
    virtual bool init();
    
    // touch
    virtual bool onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
    virtual void onTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
    virtual void onTouchMoved(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);

    void buttonTouched(Ref* pSender, ui::Widget::TouchEventType type);

private:
    ui::Button* soundButtonOn;
    ui::Button* soundButtonOff;
    ui::Button* speedButtonSlow;
    ui::Button* speedButtonNormal;
    ui::Button* speedButtonQuick;
    ui::Button* jokerButtonOn;
    ui::Button* jokerButtonOff;
    ui::Button* tunnelButtonOn;
    ui::Button* tunnelButtonOff;
    ui::Button* netButtonOn;
    ui::Button* netButtonOff;
    
    void soundSettingDisplayChange();
    void speedSettingDisplayChange();
    void jokerSettingDisplayChange();
    void tunnelSettingDisplayChange();
    void netSettingDisplayChange();
    
};

#endif /* OptionDialog_h */
