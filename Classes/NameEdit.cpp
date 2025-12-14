//
//  NameEdit.cpp
//  Sevens
//
//  Created by shinnichirou.takahashi on 2018/10/17.
//
//


#include "NameEdit.h"
#include "NetRanking.h"

USING_NS_CC;

using namespace ui;

bool NameEdit::init()
{
    if (!Layer::init())
    {
        return false;
    }

    // event listener
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(NameEdit::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(NameEdit::onTouchEnded, this);
    listener->onTouchMoved = CC_CALLBACK_2(NameEdit::onTouchMoved, this);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    this->getEventDispatcher()->setPriority(listener,kModalLayerPriority);
    
    Size visibleSize { Director::getInstance()->getVisibleSize() };
    Sprite* bgSprite { Sprite::create("name_edit_bg.png") };
    bgSprite->setPositionX(visibleSize.width / 2);
    bgSprite->setPositionY((visibleSize.height - bgSprite->getContentSize().height) + bgSprite->getContentSize().height / 2 - 20);
    this->addChild(bgSprite);
    
    ui::Button* closeButton { ui::Button::create("close_icon.png") };
    closeButton->setPosition(Vec2(460, 280));
    closeButton->addTouchEventListener(CC_CALLBACK_2(NameEdit::buttonTouched, this));
    closeButton->setTag(CLOSE_BUTTON_TAG);
    bgSprite->addChild(closeButton);

    Label* item1 = Label::createWithSystemFont("", "ariel", 40);
    item1->setAnchorPoint(Vec2(0, 0));
    item1->setPosition(Vec2(20, 240));
    item1->setColor(Color3B(0, 0, 0));
    item1->setString("名前を入力して下さい");
    bgSprite->addChild(item1, 0);
    
    editBox = EditBox::create(Size(400, 80), Scale9Sprite::create("name_edit_box.png"));
    editBox->setFont("HiraKaku-W3", 40);
    editBox->setPlaceHolder("");
    editBox->setFontColor(Color4B(0, 0, 0, 255));
    editBox->setMaxLength(9);
    editBox->setText(global_user_name.c_str());
    editBox->setReturnType(EditBox::KeyboardReturnType::DONE);
    editBox->setInputMode(EditBox::InputMode::SINGLE_LINE);
    editBox->setPosition(Vec2(40,150));
    editBox->setAnchorPoint(Vec2::ANCHOR_BOTTOM_LEFT);
    editBox->setDelegate(this);
    bgSprite->addChild(editBox);
    
    Label* item2 = Label::createWithSystemFont("", "ariel", 32);
    item2->setAnchorPoint(Vec2(0, 0));
    item2->setPosition(Vec2(40, 100));
    item2->setColor(Color3B(0, 0, 0));
    item2->setString("※9文字以内");
    bgSprite->addChild(item2, 0);
    
    ui::Button* decisionButton { ui::Button::create("decision_button.png") };
    decisionButton->setAnchorPoint(Vec2(0, 0));
    decisionButton->setPosition(Vec2(176, 18));
    decisionButton->addTouchEventListener(CC_CALLBACK_2(NameEdit::buttonTouched, this));
    decisionButton->setTag(DECISION_BUTTON_TAG);
    bgSprite->addChild(decisionButton);

    editBox->touchDownAction(NULL, cocos2d::ui::Widget::TouchEventType::ENDED);
    
    return true;
}

bool NameEdit::onTouchBegan(Touch* pTouch, Event* pEvent){
    return true;
}

void NameEdit::onTouchMoved(Touch* pTouch, Event* pEvent){
}

void NameEdit::onTouchEnded(Touch* pTouch, Event* pEvent){
}

void NameEdit::editBoxEditingDidBegin(EditBox *editBox) {
}

void NameEdit::editBoxEditingDidEnd(EditBox *editBox) {
}

void NameEdit::editBoxTextChanged(EditBox *editBox, const std::string& text) {
}

void NameEdit::editBoxReturn(EditBox *editBox) {
}

void NameEdit::buttonTouched(Ref* pSender, ui::Widget::TouchEventType type)
{
    ui::Button* button {static_cast<ui::Button*>(pSender)};

    switch(type)
    {
        case ui::Widget::TouchEventType::BEGAN:
        {
            switch (button->getTag())
            {
                case DECISION_BUTTON_TAG:
                {
                    int playerWinCount = UserDefault::getInstance()->getIntegerForKey("playerWinCount", 0);
                    int playerLoseCount0 = UserDefault::getInstance()->getIntegerForKey("playerLoseCount0", 0);
                    int playerLoseCount1 = UserDefault::getInstance()->getIntegerForKey("playerLoseCount1", 0);
                    int playerLoseCount2 = UserDefault::getInstance()->getIntegerForKey("playerLoseCount2", 0);
                    int playerLoseCount = playerLoseCount0 + playerLoseCount1 + playerLoseCount2;
                    global_user_name = editBox->getText();
                    if (global_user_name.empty()) {
                        global_user_name = USER_NAME_NONE;
                    }
                    UserDefault::getInstance()->setStringForKey("userName", global_user_name);
                    NetRanking::getNetRanking()->setMyRank(global_user_id, global_user_name, playerWinCount, playerLoseCount,
                                                           [](int user_id) {
                                                               if (global_user_id == 0) {
                                                                   global_user_id = user_id;
                                                                   UserDefault::getInstance()->setIntegerForKey("userId", global_user_id);
                                                               }
                                                           },
                                                           [](){}
                                                           );
                    this->removeFromParentAndCleanup(true);
                    break;
                }
                    
                default:
                    break;
            }
            break;
        }
            
        case ui::Widget::TouchEventType::ENDED:
        {
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
