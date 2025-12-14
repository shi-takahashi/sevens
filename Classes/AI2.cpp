//
//  AI2.cpp
//  Sevens
//
//  Created by shinnichirou.takahashi on 2018/09/16.
//
//

#include "AI2.h"
#include "Manager.h"

AI2* AI2::create()
{
    AI2* p = new AI2();
    if (!(p && p->init()))
        CC_SAFE_DELETE(p);
    return p;
}

bool AI2::init()
{
    return true;
}

Card* AI2::choiceCard(std::vector<Card*> hand_card_list)
{
    Card* joker_card = nullptr;
    std::vector<Card*> joker_card_list = Card::findCardByMark(hand_card_list, Mark::JOKER);
    if (!joker_card_list.empty()) {
        joker_card = joker_card_list.at(0);
    }
    
    std::vector<Card*> target_card_list = Manager::getManager()->askTargetCards();
    std::vector<Card*> enable_card_list = Card::getDuplicateCardList(hand_card_list, target_card_list);
    
    std::vector<Card*> enable_card_list_with_joker = {};
    for (Card* wk_card : Card::getDuplicateCardList(hand_card_list, Manager::getManager()->askTargetCardsWithJoker())) {
        if (Card::existMarkAndNumberCard(enable_card_list, wk_card->mark, wk_card->number - 1)) continue;
        if (Card::existMarkAndNumberCard(enable_card_list, wk_card->mark, wk_card->number + 1)) continue;
        enable_card_list_with_joker.emplace_back(wk_card);
    }
    
    if (enable_card_list.empty() || (enable_card_list.size() == 1 && enable_card_list.at(0)->isJoker())) {
        return nullptr;
    }
    
    if (!joker_card || enable_card_list_with_joker.empty()) {
        Card* return_card = nullptr;
        for (Card* enable_card : enable_card_list) {
            if (enable_card->isJoker()) continue;
            if (!return_card || (abs(enable_card->number - 7) > abs(return_card->number - 7))) {
                return_card = enable_card;
            }
        }
        return return_card;
    }
    
    joker_card->pair_card = enable_card_list_with_joker.at(0);
    for (Card* enable_card_with_joker : enable_card_list_with_joker) {
        if (abs(enable_card_with_joker->number - 7) > abs(joker_card->pair_card->number - 7)) {
            joker_card->pair_card = enable_card_with_joker;
        }
    }
    
    std::vector<Card*> proxy_card_minus_list = Card::findCardByMarkAndNumber(target_card_list, joker_card->pair_card->mark, joker_card->pair_card->number - 1);
    std::vector<Card*> proxy_card_plus_list = Card::findCardByMarkAndNumber(target_card_list, joker_card->pair_card->mark, joker_card->pair_card->number + 1);
    if (!proxy_card_minus_list.empty()) {
        joker_card->proxy_card = proxy_card_minus_list.at(0);
    } else if (!proxy_card_plus_list.empty()) {
        joker_card->proxy_card = proxy_card_plus_list.at(0);
    }
    
    return joker_card;
}
