//
// Created by user on 2016/10/29.
//

#ifndef SEVENS_HANDS_H
#define SEVENS_HANDS_H

#include "Card.h"

class Hands : public cocos2d::Ref
{
public:
    static Hands* create();
    bool init();
    int count();
    bool empty();
    bool jokerOnly();
    std::vector<Card*> getCards();
    int getCardIndex(Card*);
    void pushCard(Card*, int index = -1);
    void eraseCard(Card*, bool is_fill_order = true);
    void clear();
    
private:
    Card* cards[15];
    
    Hands() {};
    ~Hands() {};
};

#endif //SEVENS_HANDS_H
