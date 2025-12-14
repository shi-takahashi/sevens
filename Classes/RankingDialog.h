//
//  RankingDialog.h
//  Sevens
//
//  Created by shinnichirou.takahashi on 2018/10/17.
//
//

#ifndef RankingDialog_h
#define RankingDialog_h

#define kModalLayerPriority -129
#define CLOSE_BUTTON_TAG 1

#include <stdio.h>
#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Common.h"

USING_NS_CC;

class RankingDialog : public Layer
{
public:
    CREATE_FUNC(RankingDialog);
    virtual bool init();
    
    // touch
    virtual bool onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
    virtual void onTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
    virtual void onTouchMoved(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
    
    void buttonTouched(Ref* pSender, ui::Widget::TouchEventType type);
    
private:
    
};

#endif /* RankingDialog_h */
