//
// Created by user on 2016/10/29.
//

#include "TitleScene.h"
#include "audio/include/AudioEngine.h"

Scene* TitleScene::createScene()
{
    Scene* scene { Scene::create() };
    Layer* layer { TitleScene::create() };
    scene->addChild(layer);
    return scene;
}

bool TitleScene::init()
{
    if (!Layer::init())
    {
        return false;
    }
    
    config_sound_setting = UserDefault::getInstance()->getIntegerForKey("soundSetting", SETTING_ON);
    config_speed_setting = UserDefault::getInstance()->getIntegerForKey("speedSetting", SETTING_NORMAL);
    config_use_joker = UserDefault::getInstance()->getIntegerForKey("useJoker", SETTING_USE);
    config_use_tunnel = UserDefault::getInstance()->getIntegerForKey("useTunnel", SETTING_NO_USE);
    config_use_net = UserDefault::getInstance()->getIntegerForKey("useNet", SETTING_USE);
    
    global_user_id = UserDefault::getInstance()->getIntegerForKey("userId", USER_ID_NONE);
    global_user_name = UserDefault::getInstance()->getStringForKey("userName");
    
    Size visibleSize { Director::getInstance()->getVisibleSize() };

    Sprite* bgSprite { Sprite::create("title_bg.png") };
    bgSprite->setPosition(visibleSize / 2);
    this->addChild(bgSprite);

    Sprite* logoSprite { Sprite::create("title_logo.png") };
    logoSprite->setPosition(Vec2(640.0f, 450.0f));
    this->addChild(logoSprite);

    ui::Button* startButton { ui::Button::create("title_start.png") };
    startButton->setPosition(Vec2(640.0f, 150.0f));
    startButton->addTouchEventListener(CC_CALLBACK_2(TitleScene::touchEvent, this));
    this->addChild(startButton);
    
    ui::Button* nameButton { ui::Button::create("name_button.png") };
    nameButton->setPosition(Vec2(1194.0f, 398.0f));
    nameButton->addTouchEventListener(CC_CALLBACK_2(TitleScene::nameButtonTouched, this));
    this->addChild(nameButton);
    
    ui::Button* rankingButton { ui::Button::create("ranking_button.png") };
    rankingButton->setPosition(Vec2(1194.0f, 312.0f));
    rankingButton->addTouchEventListener(CC_CALLBACK_2(TitleScene::rankingButtonTouched, this));
    this->addChild(rankingButton);
    
    ui::Button* resultButton { ui::Button::create("report_button.png") };
    resultButton->setPosition(Vec2(1194.0f, 226.0f));
    resultButton->addTouchEventListener(CC_CALLBACK_2(TitleScene::resultButtonTouched, this));
    this->addChild(resultButton);
    
    ui::Button* optionButton { ui::Button::create("option_button.png") };
    optionButton->setPosition(Vec2(1194.0f, 140.0f));
    optionButton->addTouchEventListener(CC_CALLBACK_2(TitleScene::optionButtonTouched, this));
    this->addChild(optionButton);
    
    ui::Button* finishButton { ui::Button::create("finish_button.png") };
    finishButton->setPosition(Vec2(1194.0f, 54.0f));
    finishButton->addTouchEventListener(CC_CALLBACK_2(TitleScene::finishButtonTouched, this));
    this->addChild(finishButton);
    
    if (config_sound_setting == SETTING_ON) {
        cocos2d::experimental::AudioEngine::play2d("se_title.wav", true);
    }
    
    return true;
}

void TitleScene::onEnterTransitionDidFinish()
{
    if (global_user_name.empty()) {
        NameEdit* option { NameEdit::create() };
        this->addChild(option);
    }
}

void TitleScene::touchEvent(Ref* pSender, ui::Widget::TouchEventType type)
{
    switch(type)
    {
        case ui::Widget::TouchEventType::BEGAN:
        {
            if (config_sound_setting == SETTING_ON) {
                cocos2d::experimental::AudioEngine::stopAll();
            }
            
            Scene* scene { GameScene::createScene() };
            TransitionFade* fade { TransitionFade::create(0.4f, scene) };
            Director::getInstance()->replaceScene(fade);
            break;
        }

        default:
            break;
    }
}

void TitleScene::nameButtonTouched(Ref* pSender, ui::Widget::TouchEventType type)
{
    switch(type)
    {
        case ui::Widget::TouchEventType::BEGAN:
        {
            NameEdit* option { NameEdit::create() };
            this->addChild(option);
            break;
        }
            
        default:
            break;
    }
}

void TitleScene::rankingButtonTouched(Ref* pSender, ui::Widget::TouchEventType type)
{
    switch(type)
    {
        case ui::Widget::TouchEventType::BEGAN:
        {
            RankingDialog* report { RankingDialog::create() };
            this->addChild(report);
            break;
        }
            
        default:
            break;
    }
}

void TitleScene::resultButtonTouched(Ref* pSender, ui::Widget::TouchEventType type)
{
    switch(type)
    {
        case ui::Widget::TouchEventType::BEGAN:
        {
            ReportDialog* report { ReportDialog::create() };
            this->addChild(report);
            break;
        }
        
        default:
            break;
    }
}

void TitleScene::optionButtonTouched(Ref* pSender, ui::Widget::TouchEventType type)
{
    switch(type)
    {
        case ui::Widget::TouchEventType::BEGAN:
        {
            OptionDialog* option { OptionDialog::create() };
            this->addChild(option);
            break;
        }
            
        default:
            break;
    }
}

void TitleScene::finishButtonTouched(Ref* pSender, ui::Widget::TouchEventType type)
{
    switch(type)
    {
        case ui::Widget::TouchEventType::BEGAN:
        {
            Director::getInstance()->end();
            break;
        }
            
        default:
            break;
    }
}
