//
// Created by user on 2016/10/29.
//

#ifndef SEVENS_CARD2_H
#define SEVENS_CARD2_H

#define PLAYER_CHARACTER_CARD_POSITION_Y this->player_character->hands->getCards().at(0)->player_character_card_position[0].y

#include "cocos2d.h"
#include "Common.h"

USING_NS_CC;

class Card : public cocos2d::Ref
{
public:
    Mark        mark;
    int         number;
    Sprite*     card_image;
    std::string background_card_image_file_name;
    std::string foreground_card_image_file_name;
    Vec2        board_card_position;
    Vec2        player_character_card_position[14];
    Vec2        non_player_character_card_position[4][14];
    Card*       proxy_card;
    Card*       pair_card;
    
    static Card* create(Mark mark, int number);
    static void deleteCardFromVector(std::vector<Card*>&, Card*);
    static bool existCardInVector(std::vector<Card*>&, Card*);
    static std::vector<Card*> findCardByMark(std::vector<Card*>, Mark);
    static std::vector<Card*> findCardByNumber(std::vector<Card*>, int);
    static std::vector<Card*> findCardByMarkAndNumber(std::vector<Card*>, Mark, int);
    static bool existMarkCard(std::vector<Card*>, Mark);
    static bool existNumberCard(std::vector<Card*>, int);
    static bool existMarkAndNumberCard(std::vector<Card*>, Mark, int);
    static std::vector<Card*> getDuplicateCardList(std::vector<Card*>, std::vector<Card*>);
    Card();
    ~Card();
    bool init(Mark suit, int number);
    bool isJoker();
    
private:
    
};

#endif //SEVENS_CARD2_H
