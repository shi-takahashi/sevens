//
//  NameEdit.h
//  Sevens
//
//  Created by shinnichirou.takahashi on 2018/10/17.
//
//

#ifndef NameEdit_h
#define NameEdit_h

#define kModalLayerPriority -129
#define CLOSE_BUTTON_TAG 1
#define DECISION_BUTTON_TAG 2

#include <stdio.h>
#include "cocos2d.h"
#include "Common.h"
//#include "extensions/cocos-ext.h"
#include "ui/CocosGUI.h"

USING_NS_CC;

class NameEdit : public cocos2d::Layer , public cocos2d::ui::EditBoxDelegate
{
public:
    CREATE_FUNC(NameEdit);
    virtual bool init();
    // touch
    virtual bool onTouchBegan(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
    virtual void onTouchEnded(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
    virtual void onTouchMoved(cocos2d::Touch* pTouch, cocos2d::Event* pEvent);
    void buttonTouched(Ref* pSender, ui::Widget::TouchEventType type);

private:
    ui::EditBox * editBox;
    
    virtual void editBoxEditingDidBegin(cocos2d::ui::EditBox* editBox);
    virtual void editBoxEditingDidEnd(cocos2d::ui::EditBox* editBox);
    virtual void editBoxTextChanged(cocos2d::ui::EditBox* editBox, const std::string& text);
    virtual void editBoxReturn(cocos2d::ui::EditBox* editBox);
};

#endif /* NameEdit_h */
