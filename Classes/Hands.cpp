//
// Created by user on 2016/10/29.
//

#include <algorithm>
#include "Hands.h"

#define CARDS_SIZE (sizeof(this->cards) / sizeof(Card*))

Hands* Hands::create()
{
    Hands* p = new Hands();
    if (!(p && p->init())) {
        CC_SAFE_DELETE(p);
    }
    return p;
}

bool Hands::init()
{
    for (int i = 0; i < CARDS_SIZE; i++) {
        this->cards[i] = nullptr;
    }
    
    return true;
}

int Hands::count()
{
    int count = 0;
    
    for (int i = 0; i < CARDS_SIZE; i++) {
        if (this->cards[i]) {
            count++;
        }
    }
    
    return count;
}

bool Hands::empty()
{
    return this->count() == 0;
}

bool Hands::jokerOnly()
{
    for (int i = 0; i < CARDS_SIZE; i++) {
        if (this->cards[i] && !this->cards[i]->isJoker()) {
            return false;
        }
    }
    
    return true;
}

std::vector<Card*> Hands::getCards()
{
    std::vector<Card*> vector_cards;
    
    for (int i = 0; i < CARDS_SIZE; i++) {
        if (this->cards[i]) {
            vector_cards.emplace_back(this->cards[i]);
        }
    }
    
    return vector_cards;
}

int Hands::getCardIndex(Card* card)
{
     for (int i = 0; i < CARDS_SIZE; i++) {
        if (this->cards[i] == card) {
            return i;
        }
    }
    
    return -1;
}

void Hands::pushCard(Card* push_card, int index)
{
    if (index >= 0) {
        this->cards[index] = push_card;
    } else {
        for (int i = 0; i < CARDS_SIZE; i++) {
            if (!this->cards[i]) {
                this->cards[i] = push_card;
                break;
            }
        }
    }
}

void Hands::eraseCard(Card* erase_card, bool is_fill_order)
{
    for (int i = 0; i < CARDS_SIZE; i++) {
        if (this->cards[i] == erase_card) {
            if (is_fill_order) {
                for (int j = i + 1; j < CARDS_SIZE; j++) {
                    this->cards[j - 1] = this->cards[j];
                    this->cards[j] = nullptr;
                }
            } else {
                this->cards[i] = nullptr;
            }
            break;
        }
    }
}

void Hands::clear()
{
    for (int i = 0; i < CARDS_SIZE; i++) {
        this->cards[i] = nullptr;
    }
}
