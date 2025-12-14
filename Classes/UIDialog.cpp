//
// Created by user on 2016/11/05.
//

#include "UIDialog.h"
USING_NS_CC;

UIDialog* UIDialog::create(std::string title,
                           std::string content,
                           std::string content_right,
                           std::vector<UIDialogButton*> buttons)
{
    auto dialog = new UIDialog();

    dialog->init();
    dialog->view(title,content,content_right,buttons);
    dialog->autorelease();
    return dialog;

}

#define kModalLayerPriority -1

bool UIDialog::init()
{
    if (!Layer::init())
        return false;

    _uiLayer = Layer::create();
    _uiLayer->setGlobalZOrder(101);
    addChild(_uiLayer);
    
    return true;
}

void UIDialog::view(std::string title,
          std::string content,
          std::string content_right,
          std::vector<UIDialogButton*> buttons){

    Size winSize = Director::getInstance()->getWinSize();

    // 背景作成
    auto s = Sprite::create();
    s->setTextureRect(Rect(0,0,winSize.width*2,winSize.height*2));
    s->setColor(Color3B::BLACK);
    s->setOpacity(128); // 透明度
    s->setGlobalZOrder(102);
    _uiLayer->addChild(s);

    // モーダルレイヤ化
    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(true);
    listener->onTouchBegan = [](Touch *touch,Event*event)->bool{
        return true;
    };
    this->getEventDispatcher()->addEventListenerWithSceneGraphPriority(listener, this);
    this->getEventDispatcher()->setPriority(listener, kModalLayerPriority);

    int size = buttons.size() - 1;
    if (title != "") size++;
    if (content != "") size++;

    float mergin = 120.0f;
    float height = mergin * size / 2;

    Vector<MenuItem*> item;
    if (title != ""){
//        auto* titleLabel = Label::createWithSystemFont(title, "Arial", 28,Size::ZERO,TextHAlignment::CENTER,TextVAlignment::CENTER);
        auto* titleLabel = Label::createWithSystemFont(title, "Arial", 38,Size::ZERO,TextHAlignment::CENTER,TextVAlignment::CENTER);
        titleLabel->setGlobalZOrder(102);
        titleLabel->setPosition(Point(winSize.width/2,winSize.height/2+height));
        _uiLayer->addChild(titleLabel);
        height -= mergin;
    }

    if (content != ""){
//        auto* contentLabel = Label::createWithSystemFont(content, "Arial", 28,Size::ZERO,TextHAlignment::CENTER,TextVAlignment::CENTER);
        auto* contentLabel = Label::createWithSystemFont(content, "Arial", 38,Size::ZERO,TextHAlignment::LEFT,TextVAlignment::CENTER);
        contentLabel->setGlobalZOrder(102);
        contentLabel->setPosition(Point(winSize.width/2-150,winSize.height/2+height));
        _uiLayer->addChild(contentLabel);
        if (content_right != ""){
            //        auto* contentLabel = Label::createWithSystemFont(content, "Arial", 28,Size::ZERO,TextHAlignment::CENTER,TextVAlignment::CENTER);
            auto* contentLabel = Label::createWithSystemFont(content_right, "Arial", 38,Size::ZERO,TextHAlignment::CENTER,TextVAlignment::CENTER);
            contentLabel->setGlobalZOrder(102);
            contentLabel->setPosition(Point(winSize.width/2+150,winSize.height/2+height));
            _uiLayer->addChild(contentLabel);
        }
        height -= mergin;
    }

    int tag = 1;
    for (auto b : buttons){
        auto* labelBtnLabel = Label::createWithSystemFont(b->name(), "Arial", 48);
        labelBtnLabel->setGlobalZOrder(103);
        auto* itemLabel = MenuItemLabel::create(labelBtnLabel, b->action());
        itemLabel->setGlobalZOrder(103);

        // ボタンのサイズ
        //log("%f %f",itemLabel->getPosition().x, itemLabel->getPosition().y);
        itemLabel->setPosition(Vec2(0.0f, height));
        if (b->tag() != 1) tag = b->tag(); // 指定があれば指定タグ
        itemLabel->setTag(tag);
        item.pushBack(itemLabel);
        height -= mergin;
        tag++; // 普段は加算
    }
     //メニューを作成
    auto* menu = Menu::createWithArray(item);
    _uiLayer->addChild(menu);
    menu->setGlobalZOrder(102);
}

void UIDialog::close()
{
    Action* action = RemoveSelf::create();
    runAction(action);
}
