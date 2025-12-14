//
//  ReportDialog.hpp
//  Sevens
//
//  Created by shinnichirou.takahashi on 2017/04/12.
//
//

#ifndef ReportDialog_h
#define ReportDialog_h

//#define kModalLayerPriority kCCMenuHandlerPriority-1
#define kModalLayerPriority -129
#define CLOSE_BUTTON_TAG 1

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Common.h"

USING_NS_CC;

class ReportDialog : public Layer
{
public:
    CREATE_FUNC(ReportDialog);
    virtual bool init();
    
    // touch
    virtual bool onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
    virtual void onTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
    virtual void onTouchMoved(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);

    void buttonTouched(Ref* pSender, ui::Widget::TouchEventType type);

private:
    
};

#endif /* ReportDialog_h */
