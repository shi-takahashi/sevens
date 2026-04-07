//
//  ReportDialog.cpp
//  Sevens
//
//  Created by shinnichirou.takahashi on 2017/04/12.
//
//

#include "ReportDialog.h"

USING_NS_CC;

bool ReportDialog::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    // event listener
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(ReportDialog::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(ReportDialog::onTouchEnded, this);
    listener->onTouchMoved = CC_CALLBACK_2(ReportDialog::onTouchMoved, this);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    this->getEventDispatcher()->setPriority(listener,kModalLayerPriority);
    
    Size visibleSize { Director::getInstance()->getVisibleSize() };

    // 半透明オーバーレイ（背景を暗くする）
    auto overlay = Sprite::create();
    overlay->setTextureRect(Rect(0, 0, visibleSize.width, visibleSize.height));
    overlay->setColor(Color3B::BLACK);
    overlay->setOpacity(160);
    overlay->setPosition(visibleSize / 2);
    this->addChild(overlay);

    Sprite* bgSprite { Sprite::create("report_bg.png") };
    bgSprite->setPosition(visibleSize / 2);
    bgSprite->setVisible(true);
    this->addChild(bgSprite);

    ui::Button* closeButton { ui::Button::create("close_icon.png") };
    closeButton->setPosition(Vec2(748, 412));
    closeButton->addTouchEventListener(CC_CALLBACK_2(ReportDialog::buttonTouched, this));
    closeButton->setTag(CLOSE_BUTTON_TAG);
    bgSprite->addChild(closeButton);
    
    Label* item1 = Label::createWithSystemFont("", "ariel", 32);
    item1->setAnchorPoint(Vec2(0, 0));
    item1->setPosition(Vec2(30, 350));
    item1->setColor(Color3B(255, 250, 220));
    item1->setString("通算成績");
    bgSprite->addChild(item1, 0);
    
    Label* item2 = Label::createWithSystemFont("", "ariel", 32);
    item2->setAnchorPoint(Vec2(0, 0));
    item2->setPosition(Vec2(30, 250));
    item2->setColor(Color3B(255, 250, 220));
    item2->setString("通算勝率");
    bgSprite->addChild(item2, 0);
    
    Label* item3 = Label::createWithSystemFont("", "ariel", 32);
    item3->setAnchorPoint(Vec2(0, 0));
    item3->setPosition(Vec2(30, 150));
    item3->setColor(Color3B(255, 250, 220));
    item3->setString("連勝記録");
    bgSprite->addChild(item3, 0);
    
    int playerWinCount = UserDefault::getInstance()->getIntegerForKey("playerWinCount", 0);
    int playerLoseCount0 = UserDefault::getInstance()->getIntegerForKey("playerLoseCount0", 0);
    int playerLoseCount1 = UserDefault::getInstance()->getIntegerForKey("playerLoseCount1", 0);
    int playerLoseCount2 = UserDefault::getInstance()->getIntegerForKey("playerLoseCount2", 0);

    int playerStreak = UserDefault::getInstance()->getIntegerForKey("playerStreak", 0);
    
    int sumCount = playerWinCount + playerLoseCount0 + playerLoseCount1 + playerLoseCount2;
    float rate = 0;
    
    if (sumCount > 0) {
        rate = (float)playerWinCount / (float)sumCount * 100;
    }
    
    Label* result = Label::createWithSystemFont("", "ariel", 32);
    result->setAnchorPoint(Vec2(0, 0));
    result->setPosition(Vec2(180, 350));
    result->setColor(Color3B(255, 250, 220));
    result->setString(StringUtils::format("%d勝%d敗(%d/%d/%d/%d)",
                                          playerWinCount, playerLoseCount0 + playerLoseCount1 + playerLoseCount2,
                                          playerWinCount, playerLoseCount0, playerLoseCount1,playerLoseCount2));
    bgSprite->addChild(result, 0);
    
    Label* percent = Label::createWithSystemFont("", "ariel", 32);
    percent->setAnchorPoint(Vec2(0, 0));
    percent->setPosition(Vec2(180, 250));
    percent->setColor(Color3B(255, 250, 220));
    percent->setString(StringUtils::format("%.2f", rate) + "%");
    bgSprite->addChild(percent, 0);
    
    Label* streak = Label::createWithSystemFont("", "ariel", 32);
    streak->setAnchorPoint(Vec2(0, 0));
    streak->setPosition(Vec2(180, 150));
    streak->setColor(Color3B(255, 250, 220));
    streak->setString(StringUtils::format("%d", playerStreak));
    bgSprite->addChild(streak, 0);

    return true;
}

bool ReportDialog::onTouchBegan(Touch* pTouch, Event* pEvent){
    return true;
}

void ReportDialog::onTouchMoved(Touch* pTouch, Event* pEvent){
    
}

void ReportDialog::onTouchEnded(Touch* pTouch, Event* pEvent){
    
}

void ReportDialog::buttonTouched(Ref* pSender, ui::Widget::TouchEventType type)
{
    switch(type)
    {
    case ui::Widget::TouchEventType::ENDED:
    {
        ui::Button* button {dynamic_cast<ui::Button*>(pSender)};
        switch (button->getTag())
        {
            case CLOSE_BUTTON_TAG:
                this->removeFromParentAndCleanup(true);
                break;
                
            default:
                break;
        }
        
    }
    
    default:
        break;
    }
}
