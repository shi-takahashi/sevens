//
//  NonPlayerCharacter.cpp
//  Sevens
//
//  Created by shinnichirou.takahashi on 2018/09/09.
//
//

#include "NonPlayerCharacter.h"
#include "GameScene.h"
#include "Manager.h"

NonPlayerCharacter* NonPlayerCharacter::create(int character_id, const std::string& filename)
{
    NonPlayerCharacter* p = new NonPlayerCharacter(character_id, filename);
    if (!(p && p->init()))
        CC_SAFE_DELETE(p);
    return p;
}

bool NonPlayerCharacter::init()
{
    if (!Character::init()) {
        return false;
    }
    
    this->thumbnail->setPosition(Vec2(68.0f, 653.0f - 222.0f * (this->character_id - 1)));
    this->label_rank->setPosition(Vec2(30.0f, 520.0f - 222.0f * (this->character_id - 1)));
    this->label_pass->setPosition(Vec2(160.0f, 595.0f - 222.0f * (this->character_id - 1)));
    this->label_message->setPosition(Vec2(125.0f, 655.0f - 222.0f * (this->character_id - 1)));
    
    return true;
}

int NonPlayerCharacter::getType()
{
    return Character::TYPE_NON_PLAYER;
}

void NonPlayerCharacter::setAI(AIBase* ai)
{
    this->ai = ai;
    this->ai->retain();
}

Card* NonPlayerCharacter::choiceCard()
{
    return this->ai->choiceCard(this->hands->getCards());
}

std::vector<Card*> NonPlayerCharacter::process(Command command, Card* designated_card)
{
    std::vector<Card*> pull_out_cards = Character::process(command, designated_card);
    
    if (pull_out_cards.size() < 1) {
        return {};
    }
    
    float game_speed = GameScene::getGameScene()->getGameSpeed();
    float delay_time = 1 / game_speed;
    
    std::vector<Card*> new_my_cards = {};
    
    for (Card* wk_card : this->hands->getCards()) {
        if (Card::existCardInVector(pull_out_cards, wk_card)) {
            continue;
        }
        new_my_cards.emplace_back(wk_card);
    }
    
    for (int order = 0; order < new_my_cards.size(); order++) {
        Card* move_card = new_my_cards.at(order);

        Vec2 new_pos = move_card->non_player_character_card_position[this->character_id][order];
        
        if (move_card->card_image->getPositionX() == new_pos.x && move_card->card_image->getPositionY() == new_pos.y) {
            continue;
        }
        
        DelayTime* wait = DelayTime::create(delay_time);
        Spawn* action_main = Spawn::create(MoveTo::create(0.3 / game_speed, new_pos),
                                           NULL);
        CallFunc* action_after = CallFunc::create([move_card, order]() {
            move_card->card_image->setGlobalZOrder(order);
        });
        move_card->card_image->runAction(Sequence::create(wait,
                                                            action_main,
                                                            action_after,
                                                            NULL));
    }
    
    return pull_out_cards;
}
