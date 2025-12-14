//
//  OptionDialog.cpp
//  Sevens
//
//  Created by shinnichirou.takahashi on 2017/04/12.
//
//

#include "OptionDialog.h"

USING_NS_CC;

bool OptionDialog::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    // event listener
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = CC_CALLBACK_2(OptionDialog::onTouchBegan, this);
    listener->onTouchEnded = CC_CALLBACK_2(OptionDialog::onTouchEnded, this);
    listener->onTouchMoved = CC_CALLBACK_2(OptionDialog::onTouchMoved, this);
    
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    this->getEventDispatcher()->setPriority(listener,kModalLayerPriority);
    
    Size visibleSize { Director::getInstance()->getVisibleSize() };
    Sprite* bgSprite { Sprite::create("option_bg.png") };
    bgSprite->setPosition(visibleSize / 2);
//    bgSprite->setScale(2, 2);
    this->addChild(bgSprite);
    
    ui::Button* closeButton { ui::Button::create("close_icon.png") };
    closeButton->setPosition(Vec2(580, 480));
    closeButton->addTouchEventListener(CC_CALLBACK_2(OptionDialog::buttonTouched, this));
    closeButton->setTag(CLOSE_BUTTON_TAG);
    bgSprite->addChild(closeButton);
    
    Label* soundLabel = Label::createWithSystemFont("", "ariel", 40);
    soundLabel->setAnchorPoint(Vec2(0, 0));
    soundLabel->setPosition(Vec2(15, 380));
    soundLabel->setColor(Color3B(255, 255, 255));
    soundLabel->setString("サウンド");
    bgSprite->addChild(soundLabel, 0);
    
    soundButtonOn = ui::Button::create();
    soundButtonOn->setPosition(Vec2(295.0f, 405));
    soundButtonOn->addTouchEventListener(CC_CALLBACK_2(OptionDialog::buttonTouched, this));
    soundButtonOn->setTag(SOUND_BUTTON_ON_TAG);
    soundButtonOn->setScale(0.5f);
    bgSprite->addChild(soundButtonOn);

    soundButtonOff = ui::Button::create();
    soundButtonOff->setPosition(Vec2(415.0f, 405));
    soundButtonOff->addTouchEventListener(CC_CALLBACK_2(OptionDialog::buttonTouched, this));
    soundButtonOff->setTag(SOUND_BUTTON_OFF_TAG);
    soundButtonOff->setScale(0.5f);
    bgSprite->addChild(soundButtonOff);
    
    Label* speedLabel = Label::createWithSystemFont("", "ariel", 40);
    speedLabel->setAnchorPoint(Vec2(0, 0));
    speedLabel->setPosition(Vec2(15, 300));
    speedLabel->setColor(Color3B(255, 255, 255));
    speedLabel->setString("スピード");
    bgSprite->addChild(speedLabel, 0);
    
    speedButtonSlow = ui::Button::create();
    speedButtonSlow->setPosition(Vec2(295.0f, 325));
    speedButtonSlow->addTouchEventListener(CC_CALLBACK_2(OptionDialog::buttonTouched, this));
    speedButtonSlow->setTag(SPEED_BUTTON_SLOW_TAG);
    speedButtonSlow->setScale(0.5f);
    bgSprite->addChild(speedButtonSlow);
    
    speedButtonNormal = ui::Button::create();
    speedButtonNormal->setPosition(Vec2(415.0f, 325));
    speedButtonNormal->addTouchEventListener(CC_CALLBACK_2(OptionDialog::buttonTouched, this));
    speedButtonNormal->setTag(SPEED_BUTTON_NORMAL_TAG);
    speedButtonNormal->setScale(0.5f);
    bgSprite->addChild(speedButtonNormal);
    
    speedButtonQuick = ui::Button::create();
    speedButtonQuick->setPosition(Vec2(535.0f, 325));
    speedButtonQuick->addTouchEventListener(CC_CALLBACK_2(OptionDialog::buttonTouched, this));
    speedButtonQuick->setTag(SPEED_BUTTON_QUICK_TAG);
    speedButtonQuick->setScale(0.5f);
    bgSprite->addChild(speedButtonQuick);

    Label* jokerLabel = Label::createWithSystemFont("", "ariel", 40);
    jokerLabel->setAnchorPoint(Vec2(0, 0));
    jokerLabel->setPosition(Vec2(15, 220));
    jokerLabel->setColor(Color3B(255, 255, 255));
    jokerLabel->setString("ジョーカー");
    bgSprite->addChild(jokerLabel, 0);
    
    jokerButtonOn = ui::Button::create();
    jokerButtonOn->setPosition(Vec2(295.0f, 245));
    jokerButtonOn->addTouchEventListener(CC_CALLBACK_2(OptionDialog::buttonTouched, this));
    jokerButtonOn->setTag(JOKER_BUTTON_ON_TAG);
    jokerButtonOn->setScale(0.5f);
    bgSprite->addChild(jokerButtonOn);
    
    jokerButtonOff = ui::Button::create();
    jokerButtonOff->setPosition(Vec2(415.0f, 245));
    jokerButtonOff->addTouchEventListener(CC_CALLBACK_2(OptionDialog::buttonTouched, this));
    jokerButtonOff->setTag(JOKER_BUTTON_OFF_TAG);
    jokerButtonOff->setScale(0.5f);
    bgSprite->addChild(jokerButtonOff);

    Label* tunnelLabel = Label::createWithSystemFont("", "ariel", 40);
    tunnelLabel->setAnchorPoint(Vec2(0, 0));
    tunnelLabel->setPosition(Vec2(15, 140));
    tunnelLabel->setColor(Color3B(255, 255, 255));
    tunnelLabel->setString("トンネル");
    bgSprite->addChild(tunnelLabel, 0);
    
    tunnelButtonOn = ui::Button::create();
    tunnelButtonOn->setPosition(Vec2(295.0f, 165));
    tunnelButtonOn->addTouchEventListener(CC_CALLBACK_2(OptionDialog::buttonTouched, this));
    tunnelButtonOn->setTag(TUNNEL_BUTTON_ON_TAG);
    tunnelButtonOn->setScale(0.5f);
    bgSprite->addChild(tunnelButtonOn);
    
    tunnelButtonOff = ui::Button::create();
    tunnelButtonOff->setPosition(Vec2(415.0f, 165));
    tunnelButtonOff->addTouchEventListener(CC_CALLBACK_2(OptionDialog::buttonTouched, this));
    tunnelButtonOff->setTag(TUNNEL_BUTTON_OFF_TAG);
    tunnelButtonOff->setScale(0.5f);
    bgSprite->addChild(tunnelButtonOff);
    
    Label* netLabel = Label::createWithSystemFont("", "ariel", 40);
    netLabel->setAnchorPoint(Vec2(0, 0));
    netLabel->setPosition(Vec2(15, 60));
    netLabel->setColor(Color3B(255, 255, 255));
    netLabel->setString("通信");
    bgSprite->addChild(netLabel, 0);
    
    netButtonOn = ui::Button::create();
    netButtonOn->setPosition(Vec2(295.0f, 85));
    netButtonOn->addTouchEventListener(CC_CALLBACK_2(OptionDialog::buttonTouched, this));
    netButtonOn->setTag(NET_BUTTON_ON_TAG);
    netButtonOn->setScale(0.5f);
    bgSprite->addChild(netButtonOn);
    
    netButtonOff = ui::Button::create();
    netButtonOff->setPosition(Vec2(415.0f, 85));
    netButtonOff->addTouchEventListener(CC_CALLBACK_2(OptionDialog::buttonTouched, this));
    netButtonOff->setTag(NET_BUTTON_OFF_TAG);
    netButtonOff->setScale(0.5f);
    bgSprite->addChild(netButtonOff);
    
    soundSettingDisplayChange();
    speedSettingDisplayChange();
    jokerSettingDisplayChange();
    tunnelSettingDisplayChange();
    netSettingDisplayChange();
    
    return true;
}

bool OptionDialog::onTouchBegan(Touch* pTouch, Event* pEvent){
    return true;
}

void OptionDialog::onTouchMoved(Touch* pTouch, Event* pEvent){
    
}

void OptionDialog::onTouchEnded(Touch* pTouch, Event* pEvent){
    
}

void OptionDialog::soundSettingDisplayChange()
{
    if (config_sound_setting == SETTING_OFF) {
        soundButtonOn->loadTextureNormal("on_invalid.png");
        soundButtonOff->loadTextureNormal("off_valid.png");
    } else {
        soundButtonOn->loadTextureNormal("on_valid.png");
        soundButtonOff->loadTextureNormal("off_invalid.png");
    }
}

void OptionDialog::speedSettingDisplayChange()
{
    if (config_speed_setting == SETTING_SLOW) {
        speedButtonSlow->loadTextureNormal("slow_valid.png");
        speedButtonNormal->loadTextureNormal("normal_invalid.png");
        speedButtonQuick->loadTextureNormal("quick_invalid.png");
    } else if (config_speed_setting == SETTING_QUICK) {
        speedButtonSlow->loadTextureNormal("slow_invalid.png");
        speedButtonNormal->loadTextureNormal("normal_invalid.png");
        speedButtonQuick->loadTextureNormal("quick_valid.png");
    } else {
        speedButtonSlow->loadTextureNormal("slow_invalid.png");
        speedButtonNormal->loadTextureNormal("normal_valid.png");
        speedButtonQuick->loadTextureNormal("quick_invalid.png");
    }
}

void OptionDialog::jokerSettingDisplayChange()
{
    if (config_use_joker == SETTING_NO_USE) {
        jokerButtonOn->loadTextureNormal("on_invalid.png");
        jokerButtonOff->loadTextureNormal("off_valid.png");
    } else {
        jokerButtonOn->loadTextureNormal("on_valid.png");
        jokerButtonOff->loadTextureNormal("off_invalid.png");
    }
}

void OptionDialog::tunnelSettingDisplayChange()
{
    if (config_use_tunnel == SETTING_NO_USE) {
        tunnelButtonOn->loadTextureNormal("on_invalid.png");
        tunnelButtonOff->loadTextureNormal("off_valid.png");
    } else {
        tunnelButtonOn->loadTextureNormal("on_valid.png");
        tunnelButtonOff->loadTextureNormal("off_invalid.png");
    }
}

void OptionDialog::netSettingDisplayChange()
{
    if (config_use_net == SETTING_NO_USE) {
        netButtonOn->loadTextureNormal("on_invalid.png");
        netButtonOff->loadTextureNormal("off_valid.png");
    } else {
        netButtonOn->loadTextureNormal("on_valid.png");
        netButtonOff->loadTextureNormal("off_invalid.png");
    }
}

void OptionDialog::buttonTouched(Ref* pSender, ui::Widget::TouchEventType type)
{
    switch(type)
    {
    case ui::Widget::TouchEventType::ENDED:
    {
        ui::Button* button {static_cast<ui::Button*>(pSender)};
        switch (button->getTag())
        {
            case SOUND_BUTTON_ON_TAG:
            {
                if (config_sound_setting == SETTING_OFF) {
                    config_sound_setting = SETTING_ON;
                    UserDefault::getInstance()->setIntegerForKey("soundSetting", config_sound_setting);
                    soundSettingDisplayChange();
                    cocos2d::experimental::AudioEngine::play2d("se_title.wav", true);
                }
                break;
            }
            case SOUND_BUTTON_OFF_TAG:
            {
                if (config_sound_setting == SETTING_ON) {
                    config_sound_setting = SETTING_OFF;
                    UserDefault::getInstance()->setIntegerForKey("soundSetting", config_sound_setting);
                    soundSettingDisplayChange();
                    cocos2d::experimental::AudioEngine::stopAll();
                }
                break;
            }
            case SPEED_BUTTON_SLOW_TAG:
            {
                if (config_speed_setting != SETTING_SLOW) {
                    config_speed_setting = SETTING_SLOW;
                    UserDefault::getInstance()->setIntegerForKey("speedSetting", config_speed_setting);
                    speedSettingDisplayChange();
                }
                break;
            }
            case SPEED_BUTTON_NORMAL_TAG:
            {
                if (config_speed_setting != SETTING_NORMAL) {
                    config_speed_setting = SETTING_NORMAL;
                    UserDefault::getInstance()->setIntegerForKey("speedSetting", config_speed_setting);
                    speedSettingDisplayChange();
                }
                break;
            }
            case SPEED_BUTTON_QUICK_TAG:
            {
                if (config_speed_setting != SETTING_QUICK) {
                    config_speed_setting = SETTING_QUICK;
                    UserDefault::getInstance()->setIntegerForKey("speedSetting", config_speed_setting);
                    speedSettingDisplayChange();
                }
                break;
            }
            case JOKER_BUTTON_ON_TAG:
            {
                if (config_use_joker == SETTING_NO_USE) {
                    config_use_joker = SETTING_USE;
                    UserDefault::getInstance()->setIntegerForKey("useJoker", config_use_joker);
                    jokerSettingDisplayChange();
                }
                break;
            }
            case JOKER_BUTTON_OFF_TAG:
            {
                if (config_use_joker == SETTING_USE) {
                    config_use_joker = SETTING_NO_USE;
                    UserDefault::getInstance()->setIntegerForKey("useJoker", config_use_joker);
                    jokerSettingDisplayChange();
                }
                break;
            }
            case TUNNEL_BUTTON_ON_TAG:
            {
                if (config_use_tunnel == SETTING_NO_USE) {
                    config_use_tunnel = SETTING_USE;
                    UserDefault::getInstance()->setIntegerForKey("useTunnel", config_use_tunnel);
                    tunnelSettingDisplayChange();
                }
                break;
            }
            case TUNNEL_BUTTON_OFF_TAG:
            {
                if (config_use_tunnel == SETTING_USE) {
                    config_use_tunnel = SETTING_NO_USE;
                    UserDefault::getInstance()->setIntegerForKey("useTunnel", config_use_tunnel);
                    tunnelSettingDisplayChange();
                }
                break;
            }
            case NET_BUTTON_ON_TAG:
            {
                if (config_use_net == SETTING_NO_USE) {
                    config_use_net = SETTING_USE;
                    UserDefault::getInstance()->setIntegerForKey("useNet", config_use_net);
                    netSettingDisplayChange();
                }
                break;
            }
            case NET_BUTTON_OFF_TAG:
            {
                if (config_use_net == SETTING_USE) {
                    config_use_net = SETTING_NO_USE;
                    UserDefault::getInstance()->setIntegerForKey("useNet", config_use_net);
                    netSettingDisplayChange();
                }
                break;
            }
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
