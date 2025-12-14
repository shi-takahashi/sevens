//
//  Manager.cpp
//  Sevens
//
//  Created by shinnichirou.takahashi on 2018/09/09.
//
//

#include "Manager.h"
#include "GameScene.h"
#include "PlayerCharacter.h"
//#include "Util.h"
#include "NetRanking.h"

Manager* Manager::manager = nullptr;

Manager* Manager::getManager()
{
    if (!manager) {
        manager = new Manager();
        if (!(manager && manager->init()))
            CC_SAFE_DELETE(manager);
    }
    return manager;
}

Manager::~Manager()
{
    manager = nullptr;
}

bool Manager::init()
{
    this->table_cards.clear();
    return true;
}

void Manager::setCharacters(std::vector<Character*>& characters)
{
    this->characters = characters;
}

void Manager::dealCard(std::vector<Card*> cards)
{
    std::random_device rd;
    std::mt19937 engine(rd());
    
    std::shuffle(cards.begin(), cards.end(), engine);
    
    for (int card_idx = 0; card_idx < cards.size(); card_idx++) {
        Card* card = cards.at(card_idx);
        int char_idx = (card_idx + 1) % this->characters.size();
        this->characters.at(char_idx)->hands->pushCard(card);
        int char_card_idx = this->characters.at(char_idx)->hands->getCardIndex(card);
        if (this->characters.at(char_idx)->isPlayer()) {
            card->card_image->setPosition(card->player_character_card_position[char_card_idx]);
            card->card_image->setScale(CARD_SCALE_PLAYER);
            card->card_image->setTexture(card->foreground_card_image_file_name);
        } else {
            card->card_image->setPosition(card->non_player_character_card_position[char_idx][char_card_idx]);
            card->card_image->setScale(CARD_SCALE_NPC);
            card->card_image->setTexture(card->background_card_image_file_name);
        }
    }
}

void Manager::start()
{
    for (Character* character : this->characters) {
        character->process(Command::PUT_SEVENS);
    }
}

void Manager::process(Character* performer, Command command, Card* result_card)
{
    this->updateCardData(performer, command, result_card);
    
    switch (command)
    {
        case Command::PUT_NORMAL:
        {
            if (!result_card && performer->pass > 3) {
                performer->process(Command::PUT_ALL);
                return;
            }
            if (result_card) {
                if (performer->hands->empty()) {
                    this->setRank(performer);
                }
                if (result_card->isJoker()) {
                    for (Character* character : this->characters) {
                        character->process(Command::PUT_IT, result_card->proxy_card);
                    }
                    result_card->proxy_card = nullptr;
                    return;
                }
                for (Card* card : GameScene::getGameScene()->game_cards) {
                    if (card->isJoker()) {
                        if (result_card == card->pair_card) {
                            card->pair_card = nullptr;
                            return;
                        }
                    }
                }
                
            }
            this->advanceNextCharacter();
            break;
        }

        case Command::PUT_ALL:
        {
            if (performer->hands->empty())
            {
                this->setRank(performer);
                if (config_sound_setting == SETTING_ON && performer->isPlayer())
                {
                    cocos2d::experimental::AudioEngine::play2d("se_lose.mp3");
                }
                this->advanceNextCharacter();
            }
            break;
        }
            
        case Command::PUT_SEVENS:
        {
            if (result_card->mark == Mark::DIAMOND && result_card->number == 7)
            {
                this->next_charcter = performer;
            }
            if ((Card::findCardByNumber(this->table_cards, 7)).size() == 4)
            {
                this->migrateControlToNext();
            }
            break;
        }
            
        case Command::PUT_IT:
        {
            performer->process(Command::TAKE_IT, (Card::findCardByMark(this->table_cards, Mark::JOKER)).at(0));
            break;
        }
            
        case Command::TAKE_IT:
        {
            this->advanceNextCharacter();
            break;
        }
            
        default:
            break;
    }
}

void Manager::updateCardData(Character* character, Command command, Card* card)
{
    switch (command) {
        case Command::PUT_NORMAL:
        case Command::PUT_SEVENS:
        case Command::PUT_IT:
        {
            if (!card) break;
            character->hands->eraseCard(card, !character->isPlayer());
            this->table_cards.emplace_back(card);
            break;
        }
            
        case Command::PUT_ALL:
        {
            if (card)
            {
                character->hands->eraseCard(card, !character->isPlayer());
                this->table_cards.emplace_back(card);
            }
            if (command == Command::PUT_ALL && character->hands->jokerOnly())
            {
                character->hands->clear();
            }
            break;
        }
            
        case Command::TAKE_IT:
        {
            if (!card) break;
            if (character->isPlayer())
            {
                character->hands->pushCard(card, 13);
            }
            else
            {
                character->hands->pushCard(card);
            }
            Card::deleteCardFromVector(this->table_cards, card);
            break;
        }
            
        default:
            break;
    }
}

bool Manager::advanceNextCharacter()
{
    std::vector<Character*> no_finish_character_list {};
    
    for (Character* wk_character : this->characters)
    {
        if (!wk_character->isFinish())
        {
            no_finish_character_list.emplace_back(wk_character);
        }
    }
    
    if (no_finish_character_list.size() == 1)
    {
        this->setRank(no_finish_character_list.at(0));
        this->end();
        return false;
    }
    
    Character* current_character = this->next_charcter;
    
    for (Character* wk_character : no_finish_character_list)
    {
        if (wk_character->character_id > current_character->character_id)
        {
            this->next_charcter = wk_character;
            break;
        }
    }
    
    if (this->next_charcter == current_character)
    {
        this->next_charcter = no_finish_character_list.at(0);
    }
    
    this->migrateControlToNext();
    
    return true;
}

void Manager::migrateControlToNext()
{
    if (this->next_charcter->isPlayer())
    {
        (static_cast<PlayerCharacter*>(this->next_charcter))->setTouchEnabled(true);
    }
    else
    {
        this->next_charcter->process(Command::PUT_NORMAL);
    }
}

void Manager::end()
{
    // ユーザーデータを読み込む
    int playerWinCount = UserDefault::getInstance()->getIntegerForKey("playerWinCount", 0);
    int playerLoseCount0 = UserDefault::getInstance()->getIntegerForKey("playerLoseCount0", 0);
    int playerLoseCount1 = UserDefault::getInstance()->getIntegerForKey("playerLoseCount1", 0);
    int playerLoseCount2 = UserDefault::getInstance()->getIntegerForKey("playerLoseCount2", 0);
    int playerStreak = UserDefault::getInstance()->getIntegerForKey("playerStreak", 0);
    int playerCurStreak = UserDefault::getInstance()->getIntegerForKey("playerCurStreak", 0);
    int npcWinCount0 = UserDefault::getInstance()->getIntegerForKey("npcWinCount0", 0);
    int npcLoseCount0 = UserDefault::getInstance()->getIntegerForKey("npcLoseCount0", 0);
    int npcWinCount1 = UserDefault::getInstance()->getIntegerForKey("npcWinCount1", 0);
    int npcLoseCount1 = UserDefault::getInstance()->getIntegerForKey("npcLoseCount1", 0);
    int npcWinCount2 = UserDefault::getInstance()->getIntegerForKey("npcWinCount2", 0);
    int npcLoseCount2 = UserDefault::getInstance()->getIntegerForKey("npcLoseCount2", 0);
    
    // 成績を更新
    int* p_count[][5] = {
        {nullptr, &playerWinCount,  &playerLoseCount0,  &playerLoseCount1,  &playerLoseCount2},
        {nullptr, &npcWinCount0,    &npcLoseCount0,     &npcLoseCount0,     &npcLoseCount0},
        {nullptr, &npcWinCount1,    &npcLoseCount1,     &npcLoseCount1,     &npcLoseCount1},
        {nullptr, &npcWinCount2,    &npcLoseCount2,     &npcLoseCount2,     &npcLoseCount2},
    };
    for (Character* character : this->characters) {
        (*p_count[character->character_id][character->rank])++;
        if (character->isPlayer()) {
            if (character->rank == 1) {
                playerCurStreak++;
                if (playerCurStreak > playerStreak) {
                    playerStreak = playerCurStreak;
                }
            } else {
                playerCurStreak = 0;
            }
        }
    }
    
    // ユーザーデータを保存する
    UserDefault::getInstance()->setIntegerForKey("playerWinCount", playerWinCount);
    UserDefault::getInstance()->setIntegerForKey("playerLoseCount0", playerLoseCount0);
    UserDefault::getInstance()->setIntegerForKey("playerLoseCount1", playerLoseCount1);
    UserDefault::getInstance()->setIntegerForKey("playerLoseCount2", playerLoseCount2);
    UserDefault::getInstance()->setIntegerForKey("playerStreak", playerStreak);
    UserDefault::getInstance()->setIntegerForKey("playerCurStreak", playerCurStreak);
    UserDefault::getInstance()->setIntegerForKey("npcWinCount0", npcWinCount0);
    UserDefault::getInstance()->setIntegerForKey("npcLoseCount0", npcLoseCount0);
    UserDefault::getInstance()->setIntegerForKey("npcWinCount1", npcWinCount1);
    UserDefault::getInstance()->setIntegerForKey("npcLoseCount1", npcLoseCount1);
    UserDefault::getInstance()->setIntegerForKey("npcWinCount2", npcWinCount2);
    UserDefault::getInstance()->setIntegerForKey("npcLoseCount2", npcLoseCount2);
    
    int playerLoseCount = playerLoseCount0 + playerLoseCount1 + playerLoseCount2;
    NetRanking::getNetRanking()->setMyRank(global_user_id, global_user_name, playerWinCount, playerLoseCount,
                                           [](int user_id) {
                                               if (global_user_id == 0) {
                                                   global_user_id = user_id;
                                                   UserDefault::getInstance()->setIntegerForKey("userId", global_user_id);
                                               }
                                           });
    
    std::string user_name = "あなた";
    if (!global_user_name.empty() && global_user_name != USER_NAME_NONE) {
        user_name = global_user_name;
    }
    
    std::string text = StringUtils::format("%s", user_name.c_str());
    if (playerCurStreak > 1) {
        text += "\n";
    }
    std::string npc_name[] = {"ぱんださん", "わんちゃん", "うさぴょん"};
    text += StringUtils::format("\n%s\n%s\n%s\n",
                                npc_name[0].c_str(),
                                npc_name[1].c_str(),
                                npc_name[2].c_str()
                                );

    std::string text_right = StringUtils::format("%5d勝%5d敗", playerWinCount, playerLoseCount0 + playerLoseCount1 + playerLoseCount2);
    if (playerCurStreak > 1) {
        text_right += StringUtils::format("\n(%2d連勝中)", playerCurStreak);
    }
    text_right += StringUtils::format("\n%5d勝%5d敗\n%5d勝%5d敗\n%5d勝%5d敗\n",
                                npcWinCount0, npcLoseCount0,
                                npcWinCount1, npcLoseCount1,
                                npcWinCount2, npcLoseCount2
                                );
    
    GameScene::getGameScene()->showResultDialog(text, text_right);
}

Card* Manager::findTargetMarkCard(Mark mark, int* number_list, size_t number_list_size)
{
    Card* ret = nullptr;
    
    for (int i = 0; i < number_list_size; i++) {
        bool b_find = false;
        int number = number_list[i];
        for (Card* card : this->table_cards) {
            if (card->mark == mark && card->number == number) {
                b_find = true;
                break;
            }
        }
        if (!b_find) {
            for (Card* card : GameScene::getGameScene()->game_cards) {
                if (card->mark == mark && card->number == number) {
                    ret = card;
                    break;
                }
            }
            break;
        }
    }
    
    return ret;
}

bool Manager::isLowLink(Mark mark)
{
    int lower_number_list[] = {6, 5, 4, 3, 2, 1};
    Card* lower_card = this->findTargetMarkCard(mark, lower_number_list, sizeof(lower_number_list) / sizeof(int));
    if (!lower_card) {
        return true;
    }
    return false;
}

bool Manager::isHighLink(Mark mark)
{
    int higher_number_list[] = {8, 9, 10, 11, 12, 13};
    Card* higher_card = this->findTargetMarkCard(mark, higher_number_list, sizeof(higher_number_list) / sizeof(int));
    if (!higher_card) {
        return true;
    }
    return false;
}

std::vector<Card*> Manager::askTargetCards()
{
    std::vector<Card*> target_cards {};
    
    for (Mark mark : {Mark::SPADE, Mark::HEART, Mark::DIAMOND, Mark::CLUB}) {
        if (config_use_tunnel) {
            if (!this->isHighLink(mark)) {
                int lower_number_list[] = {6, 5, 4, 3, 2, 1, 13, 12, 11, 10, 9, 8};
                Card* lower_card = this->findTargetMarkCard(mark, lower_number_list, sizeof(lower_number_list) / sizeof(int));
                if (lower_card) {
                    target_cards.emplace_back(lower_card);
                }
            }
            if (!this->isLowLink(mark)) {
                int higher_number_list[] = {8, 9, 10, 11, 12, 13, 1, 2, 3, 4, 5, 6};
                Card* higher_card = this->findTargetMarkCard(mark, higher_number_list, sizeof(higher_number_list) / sizeof(int));
                if (higher_card) {
                    target_cards.emplace_back(higher_card);
                }
            }
        } else {
            int lower_number_list[] = {6, 5, 4, 3, 2, 1};
            Card* lower_card = this->findTargetMarkCard(mark, lower_number_list, sizeof(lower_number_list) / sizeof(int));
            if (lower_card) {
                target_cards.emplace_back(lower_card);
            }
            int higher_number_list[] = {8, 9, 10, 11, 12, 13};
            Card* higher_card = this->findTargetMarkCard(mark, higher_number_list, sizeof(higher_number_list) / sizeof(int));
            if (higher_card) {
                target_cards.emplace_back(higher_card);
            }
        }
    }
    
    for (Card* card : GameScene::getGameScene()->game_cards) {
        if (card->isJoker()) {
            target_cards.emplace_back(card);
            break;
        }
    }
    
    return target_cards;
}

std::vector<Card*> Manager::askTargetCardsWithJoker()
{
    std::vector<Card*> target_cards_with_joker = {};
    
    for (Card* target_card : this->askTargetCards()) {
        if (target_card->isJoker()) {
            continue;
        }
        Mark mark = target_card->mark;
        for (int number : {target_card->number - 1, target_card->number + 1}) {
            bool b_find = false;
            for (Card* card : this->table_cards) {
                if (card->mark == mark && card->number == number) {
                    b_find = true;
                    break;
                }
            }
            if (!b_find) {
                for (Card* card : GameScene::getGameScene()->game_cards) {
                    if (card->mark == mark && card->number == number) {
                        target_cards_with_joker.emplace_back(card);
                        break;
                    }
                }
                break;
            }
        }
    }
    
    return target_cards_with_joker;
}

void Manager::setRank(Character* character)
{
    int rank[4] = {-1, -1, -1, -1};
    for (Character* wk_character : this->characters) {
        if (wk_character->rank != 0) {
            rank[wk_character->rank - 1] = wk_character->character_id;
        }
    }
    
    std::vector<int> rank_index;
    character->pass <= 3 ? rank_index = {0, 1, 2, 3} : rank_index = {3, 2, 1, 0};
    for (int i : rank_index) {
        if (rank[i] == -1) {
            character->rank = i + 1;
            character->label_rank->setString(StringUtils::format("%d位", character->rank));
            if (config_sound_setting == SETTING_ON) {
                if (character->isPlayer() && character->rank == 1) {
                    cocos2d::experimental::AudioEngine::play2d("se_clear.mp3");
                }
            }
            break;
        }
    }
}
