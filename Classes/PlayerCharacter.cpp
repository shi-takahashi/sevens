//
//  PlayerCharacter.cpp
//  Sevens
//
//  Created by shinnichirou.takahashi on 2018/09/09.
//
//

#include "PlayerCharacter.h"

PlayerCharacter* PlayerCharacter::create(int character_id, const std::string& filename)
{
    PlayerCharacter* p = new PlayerCharacter(character_id, filename);
    if (!(p && p->init()))
        CC_SAFE_DELETE(p);
    return p;
}

bool PlayerCharacter::init()
{
    if (!Character::init()) {
        return false;
    }
    
    this->label_rank->setPosition(Vec2(240.0f, 90.0f));
    this->label_pass->setPosition(Vec2(1160.0f, 90.0f));
    this->label_message->setPosition(Vec2(1125.0f, 150.0f));
    
    this->is_touch_enabled = false;
    this->touched_card = nullptr;
    
    this->button_pass = ui::Button::create("pass.png", "pass_on.png", "pass_off.png");
    this->button_pass->setBright(false);
    this->button_pass->setPosition(Vec2(1180.0f, 50.0f));
    this->button_pass->setTitleColor(Color3B::BLACK);
    this->button_pass->setTitleFontName("Arial");
    this->button_pass->setTitleFontSize(30);
    this->button_pass->setTouchEnabled(false);
    
    return true;
}

int PlayerCharacter::getType()
{
    return Character::TYPE_PLAYER;
}

Card* PlayerCharacter::choiceCard()
{
    return this->touched_card;
}

void PlayerCharacter::sortCards()
{
    std::vector<Card*> cards = this->hands->getCards();
    
    sort(cards.begin(),
         cards.end(),
         [](const Card* left, const Card* right) {
             if (left->mark == right->mark) {
                 return left->number < right->number;
             }
             return left->mark < right->mark;
         }
         );
    
    this->hands->clear();
    
    for (int i = 0; i < cards.size(); i++) {
        Card* card = cards.at(i);
        this->hands->pushCard(card);
        card->card_image->setPosition(card->player_character_card_position[i]);
    }
}

void PlayerCharacter::setTouchEnabled(bool flg)
{
    if (flg && this->touched_card) {
        this->touched_card->proxy_card = nullptr;
        this->touched_card->pair_card = nullptr;
        this->touched_card = nullptr;
    }
    
    this->is_touch_enabled = flg;
    this->button_pass->setBright(flg);
    this->button_pass->setTouchEnabled(flg);
}
