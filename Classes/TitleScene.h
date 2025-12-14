//
// Created by user on 2016/10/29.
//

#ifndef SEVENS_TITLESCENE_H
#define SEVENS_TITLESCENE_H

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include "Common.h"
#include "GameScene.h"
#include "NameEdit.h"
#include "RankingDialog.h"
#include "ReportDialog.h"
#include "OptionDialog.h"

USING_NS_CC;

class TitleScene : public cocos2d::Layer
{
public:
    static Scene* createScene();
    virtual bool init();
    CREATE_FUNC(TitleScene);
    void touchEvent(Ref* pSender, ui::Widget::TouchEventType type);
    void nameButtonTouched(Ref* pSender, ui::Widget::TouchEventType type);
    void rankingButtonTouched(Ref* pSender, ui::Widget::TouchEventType type);
    void resultButtonTouched(Ref* pSender, ui::Widget::TouchEventType type);
    void optionButtonTouched(Ref* pSender, ui::Widget::TouchEventType type);
    void finishButtonTouched(Ref* pSender, ui::Widget::TouchEventType type);
    
private:
    virtual void onEnterTransitionDidFinish();
};

#endif //SEVENS_TITLESCENE_H
