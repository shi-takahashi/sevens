//
//  AI1.cpp
//  Sevens
//
//  Created by shinnichirou.takahashi on 2018/09/16.
//
//

#include "AI1.h"
#include "Manager.h"

AI1* AI1::create()
{
    AI1* p = new AI1();
    if (!(p && p->init()))
        CC_SAFE_DELETE(p);
    return p;
}

bool AI1::init()
{
    return true;
}

Card* AI1::choiceCard(std::vector<Card*> hand_card_list)
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
        int number_diff_max = -1;
        Card* return_card = nullptr;
        for (Card* enable_card : enable_card_list) {
            if (enable_card->isJoker()) continue;
            for (Card* hand_card : hand_card_list) {
                if (hand_card->mark != enable_card->mark) {
                    continue;
                }
                if (enable_card->number < 7) {
                    if (hand_card->number > enable_card->number) {
                        continue;
                    }
                } else {
                    if (hand_card->number < enable_card->number) {
                        continue;
                    }
                }
                if (abs(enable_card->number - hand_card->number) > number_diff_max) {
                    number_diff_max = abs((enable_card->number - hand_card->number));
                    return_card = enable_card;
                }
            }
        }
        return return_card;
    }
    
    int number_diff_max = -1;
    for (Card* enable_card : enable_card_list_with_joker) {
        for (Card* hand_card : hand_card_list) {
            if (hand_card->mark != enable_card->mark) {
                continue;
            }
            if (enable_card->number < 7) {
                if (hand_card->number > enable_card->number) {
                    continue;
                }
            } else {
                if (hand_card->number < enable_card->number) {
                    continue;
                }
            }
            if (abs(enable_card->number - hand_card->number) > number_diff_max) {
                number_diff_max = abs((enable_card->number - hand_card->number));
                joker_card->pair_card = enable_card;
            }
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
