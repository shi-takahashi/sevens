//
//  RankingDialog.cpp
//  Sevens
//
//  Created by shinnichirou.takahashi on 2018/10/17.
//
//

#include "RankingDialog.h"
#include "NetRanking.h"

USING_NS_CC;

bool RankingDialog::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    // event listener
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(RankingDialog::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(RankingDialog::onTouchEnded, this);
    listener->onTouchMoved = CC_CALLBACK_2(RankingDialog::onTouchMoved, this);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    this->getEventDispatcher()->setPriority(listener,kModalLayerPriority);
    
    Size visibleSize { Director::getInstance()->getVisibleSize() };
    Sprite* bgSprite { Sprite::create("report_bg.png") };
    bgSprite->setPosition(visibleSize / 2);
    bgSprite->setVisible(false);
    this->addChild(bgSprite);
    
    ui::Button* closeButton { ui::Button::create("close_icon.png") };
    closeButton->setPosition(Vec2(748, 412));
    closeButton->addTouchEventListener(CC_CALLBACK_2(RankingDialog::buttonTouched, this));
    closeButton->setTag(CLOSE_BUTTON_TAG);
    bgSprite->addChild(closeButton);
    
    NetRanking::getNetRanking()->getRanking(
                                            global_user_id,
                                            [this, bgSprite](std::vector<std::map<std::string, std::string>> ranking) {
                                                int i = 0;
                                                for (std::map<std::string, std::string> map : ranking) {
//                                                    log("ranking:%s name:%s win:%s", map["ranking"].c_str(), map["name"].c_str(), map["win"].c_str());
                                                    i++;
                                                    Color3B color = Color3B(255, 255, 255);
                                                    if (map["is_my_data"] == "1") {
                                                        color = Color3B(255, 0, 0);
                                                    }
                                                    Label* item1 = Label::createWithSystemFont("", "ariel", 34);
                                                    item1->setAnchorPoint(Vec2(1, 0));
                                                    item1->setPosition(Vec2(150, 425 - i * 42));
                                                    item1->setColor(color);
                                                    item1->setString((map["ranking"] + "位").c_str());
                                                    bgSprite->addChild(item1, 0);
                                                    
                                                    Label* item2 = Label::createWithSystemFont("", "ariel", 34);
                                                    item2->setAnchorPoint(Vec2(0, 0));
                                                    item2->setPosition(Vec2(200, 425 - i * 42));
                                                    item2->setColor(color);
                                                    item2->setString(map["name"].c_str());
                                                    bgSprite->addChild(item2, 0);
                                                    
                                                    Label* item3 = Label::createWithSystemFont("", "ariel", 34);
                                                    item3->setAnchorPoint(Vec2(1, 0));
                                                    item3->setPosition(Vec2(650, 425 - i * 42));
                                                    item3->setColor(color);
                                                    item3->setString((map["win"] + "勝").c_str());
                                                    bgSprite->addChild(item3, 0);
                                                    
                                                    bgSprite->setVisible(true);
                                                }
                                            },
                                            [bgSprite]() {
                                                Label* item1 = Label::createWithSystemFont("", "ariel", 34);
                                                item1->setAnchorPoint(Vec2(0, 0));
                                                item1->setPosition(Vec2(150, 215));
                                                item1->setColor(Color3B(255, 255, 255));
                                                item1->setString("データを取得できませんでした");
                                                bgSprite->addChild(item1, 0);
                                                bgSprite->setVisible(true);
                                            });
    
    return true;
}

bool RankingDialog::onTouchBegan(Touch* pTouch, Event* pEvent){
    return true;
}

void RankingDialog::onTouchMoved(Touch* pTouch, Event* pEvent){
    
}

void RankingDialog::onTouchEnded(Touch* pTouch, Event* pEvent){
    
}

void RankingDialog::buttonTouched(Ref* pSender, ui::Widget::TouchEventType type)
{
    switch(type)
    {
        case ui::Widget::TouchEventType::ENDED:
        {
            ui::Button* button {static_cast<ui::Button*>(pSender)};
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
