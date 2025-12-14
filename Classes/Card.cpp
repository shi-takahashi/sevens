//
// Created by user on 2016/10/29.
//

#include "Card.h"

Card* Card::create(Mark suit, int number)
{
    Card* p = new Card();
    if (!(p && p->init(suit, number)))
        CC_SAFE_DELETE(p);
    return p;
}

void Card::deleteCardFromVector(std::vector<Card*>& vector_card, Card* delete_card)
{
    for (auto itr = vector_card.begin(); itr != vector_card.end(); itr++) {
        if (*itr == delete_card) {
            vector_card.erase(itr);
            break;
        }
    }
}

bool Card::existCardInVector(std::vector<Card*>& vector_card, Card* exist_card)
{
    for (auto itr = vector_card.begin(); itr != vector_card.end(); itr++) {
        if (*itr == exist_card) {
            return true;
        }
    }
    
    return false;
}

std::vector<Card*> Card::findCardByMark(std::vector<Card*> find_target_card_list, Mark mark)
{
    std::vector<Card*> found_card_list {};
    for (Card* card : find_target_card_list) {
        if (mark != card->mark) continue;
        found_card_list.emplace_back(card);
    }
    return found_card_list;
}

std::vector<Card*> Card::findCardByNumber(std::vector<Card*> find_target_card_list, int number)
{
    std::vector<Card*> found_card_list {};
    for (Card* card : find_target_card_list) {
        if (number != card->number) continue;
        found_card_list.emplace_back(card);
    }
    return found_card_list;
}

std::vector<Card*> Card::findCardByMarkAndNumber(std::vector<Card*> find_target_card_list, Mark mark, int number)
{
    std::vector<Card*> found_card_list {};
    for (Card* card : find_target_card_list) {
        if (mark != card->mark) continue;
        if (number != card->number) continue;
        found_card_list.emplace_back(card);
    }
    return found_card_list;
}

bool Card::existMarkCard(std::vector<Card*> card_list, Mark mark)
{
    return Card::findCardByMark(card_list, mark).size() > 0;
}

bool Card::existNumberCard(std::vector<Card*> card_list, int number)
{
    return Card::findCardByNumber(card_list, number).size() > 0;
}

bool Card::existMarkAndNumberCard(std::vector<Card*> card_list, Mark mark, int number)
{
    return Card::findCardByMarkAndNumber(card_list, mark, number).size() > 0;
}

std::vector<Card*> Card::getDuplicateCardList(std::vector<Card*> card_list_1, std::vector<Card*> card_list_2)
{
    std::vector<Card*> duplicate_card_list = {};
    for (Card* card1 : card_list_1) {
        for (Card* card2 : card_list_2) {
            if (card1 == card2) {
                duplicate_card_list.emplace_back(card1);
                break;
            }
        }
    }
    return duplicate_card_list;
}

Card::Card()
{
}

Card::~Card()
{
    CC_SAFE_RELEASE_NULL(this->card_image);
}

bool Card::init(Mark mark, int number)
{
    this->mark      = mark;
    this->number    = number;
    
    this->background_card_image_file_name = "back_card.png";
    
    switch (mark) {
        case Mark::JOKER: {
            this->foreground_card_image_file_name = "joker.png";
            break;
        }
        default:
            std::map<Mark, std::string> mark_str_map = {
                {Mark::SPADE,   "s"},
                {Mark::HEART,   "h"},
                {Mark::DIAMOND, "d"},
                {Mark::CLUB,    "c"},
            };
            this->foreground_card_image_file_name = mark_str_map[mark] + StringUtils::format("%d.png", number);
            break;
    }
    
    this->card_image = Sprite::create(this->background_card_image_file_name);
    CC_SAFE_RETAIN(this->card_image);
    
    this->board_card_position = Vec2(
                                     230 + ((CARD_WIDTH * CARD_SCALE_BOARD) * ((number - 1) + 0.5)),
                                     712 - ((CARD_HEIGHT * CARD_SCALE_BOARD) * (((int)mark - 1) + 0.5))
                                     );
    
    for (int iOrder = 0; iOrder < 14; iOrder++) {
        this->player_character_card_position[iOrder] = Vec2(
                                                       230.0f + ((CARD_WIDTH * CARD_SCALE_PLAYER) * (iOrder + 1) * 0.47f),
                                                       8.0f + (CARD_HEIGHT * CARD_SCALE_PLAYER) * 0.5f
                                                       );
    }
    
    for (int iChara = 1; iChara < 4; iChara++) {
        for (int iOrder = 0; iOrder < 14; iOrder++) {
            this->non_player_character_card_position[iChara][iOrder] = Vec2(
                                                                            35.0f + iOrder * 13.0f,
                                                                            546.0f - 222.0f * (iChara - 1)
                                                                            );
        }
    }
    
    this->proxy_card = nullptr;
    this->pair_card = nullptr;
    
    return true;
}

bool Card::isJoker() {
    return this->mark == Mark::JOKER;
}
