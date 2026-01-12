//
//  Character.cpp
//  Sevens
//
//  Created by shinnichirou.takahashi on 2018/09/08.
//
//

#include "Character.h"
#include "GameScene.h"
#include "Manager.h"
#include <map>
//#include "Util.h"

const int Character::TYPE_PLAYER        = 1;
const int Character::TYPE_NON_PLAYER    = 2;

Character::Character(int character_id, const std::string& filename)
{
    this->character_id = character_id;
    if (filename == "") {
        this->thumbnail = nullptr;
    } else {
        this->thumbnail = Sprite::create(filename);
        CC_SAFE_RETAIN(this->thumbnail);
    }
    this->hands = Hands::create();
}

Character::~Character()
{
    
    CC_SAFE_RELEASE_NULL(this->thumbnail);
}

bool Character::init()
{
    this->hands->init();
    
    this->rank = 0;
    this->pass = 0;
    
    this->label_rank = Label::createWithSystemFont("", "ariel", 48);
    this->label_rank->setAnchorPoint(Vec2(0, 0));
    this->label_rank->setColor(Color3B(255, 255, 255));
    this->label_rank->setGlobalZOrder(20);
    
    this->label_pass = Label::createWithSystemFont(StringUtils::format("%d", this->pass), "ariel", 48);
    this->label_pass->setAnchorPoint(Vec2(0, 0));
    this->label_pass->setColor(Color3B(255, 255, 255));
    
    this->label_message = Label::createWithSystemFont("", "ariel", 48);
    this->label_message->setAnchorPoint(Vec2(0, 0));
    this->label_message->setColor(Color3B(255, 255, 255));
    
    return true;
}

std::vector<Card*> Character::process(Command command, Card* designated_card)
{
    std::vector<Card*> target_card_list = this->extractCardList(command, designated_card);
    this->runAction(command, target_card_list);
    return target_card_list;
}

std::vector<Card*> Character::extractCardList(Command command, Card* designated_card)
{
    std::vector<Card*> target_card_list = {};
    
    switch (command)
    {            
        case Command::PUT_NORMAL:
        {
            Card* choice_card = this->choiceCard();
            if (choice_card)
            {
                target_card_list.emplace_back(choice_card);
                if (choice_card->isJoker())
                {
                    target_card_list.emplace_back(choice_card->pair_card);
                }
            }
            break;
        }
            
        case Command::PUT_ALL:
        {
            for (Card* card : this->hands->getCards())
            {
                if (card->isJoker()) continue;
                target_card_list.emplace_back(card);
            }
            break;
        }
            
        case Command::PUT_SEVENS:
        {
            target_card_list = Card::findCardByNumber(this->hands->getCards(), 7);
            break;
        }
            
        case Command::PUT_IT:
        {
            // 引数をdesignated_cardにしたい
            target_card_list = Card::findCardByMarkAndNumber(this->hands->getCards(), designated_card->mark, designated_card->number);
            break;
        }
            
        case Command::TAKE_IT:
        {
            target_card_list.emplace_back(designated_card);
            break;
        }
            
        default:
            break;
            
    }
    
    return target_card_list;
}

void Character::runAction(Command command, std::vector<Card*> target_card_list)
{
    switch (command)
    {
        case Command::PUT_NORMAL:
        {
            if (target_card_list.empty())
            {
                this->pass++;
                this->runActionPass();
            }
            else
            {
                this->runActionPutCard(command, target_card_list);
            }
            break;
        }

        case Command::PUT_ALL:
        {
            if (target_card_list.empty())
            {
                Manager::getManager()->process(this, command, nullptr);
            } else
            {
                this->runActionPutCard(command, target_card_list);
            }
            break;
        }

        case Command::PUT_SEVENS:
        {
            this->runActionPutCard(command, target_card_list);
            break;
        }
            
        case Command::PUT_IT:
        {
            this->runActionPutCard(command, target_card_list);
            break;
        }
            
        case Command::TAKE_IT:
        {
            this->runActionTakeCard(target_card_list);
            break;
        }
            
        default:
            break;
    }
}

void Character::runActionPutCard(Command command, std::vector<Card*> put_card_list)
{
    float game_speed = GameScene::getGameScene()->getGameSpeed();
    float delay_time = 1 / game_speed;
    
    if (command == Command::PUT_ALL || (command == Command::PUT_NORMAL && this->getType() == Character::TYPE_PLAYER)) {
        delay_time = 0;
    }
    
    for (int card_index = 0; card_index < put_card_list.size(); card_index++) {
        Card* put_card = put_card_list.at(card_index);
        
        DelayTime* wait = DelayTime::create(delay_time);
        
        CallFunc* action_before = CallFunc::create([put_card]() {
            put_card->card_image->setGlobalZOrder(99);
            put_card->card_image->setTexture(Director::getInstance()->getTextureCache()->getTextureForKey(put_card->foreground_card_image_file_name));
            if (config_sound_setting == SETTING_ON) {
                cocos2d::experimental::AudioEngine::play2d("se_move.wav");
            }
        });
        
        Card* move_card = put_card;
        if (put_card->isJoker()) {
            move_card = put_card->proxy_card;
        }
        Vec2 pos = Vec2(230 + ((CARD_WIDTH * CARD_SCALE_BOARD) * ((move_card->number - 1) + 0.5)),
                        712 - ((CARD_HEIGHT * CARD_SCALE_BOARD) * (((int)move_card->mark - 1) + 0.5))
                        );
        Spawn* action_main = Spawn::create(MoveTo::create(0.3 / game_speed, pos),
                                           ScaleTo::create(0.3 / game_speed, CARD_SCALE_BOARD, CARD_SCALE_BOARD),
                                           NULL);
        
        DelayTime* wait2 = DelayTime::create(0.3 / game_speed);
        
        CallFunc* action_after = CallFunc::create([this, command, put_card]() {
            if (put_card->isJoker()) {
                put_card->card_image->setGlobalZOrder(0);
            } else {
                put_card->card_image->setGlobalZOrder(1);
            }
            Manager::getManager()->process(this, command, put_card);
        });
        
        put_card->card_image->runAction(Sequence::create(wait,
                                                            action_before,
                                                            action_main,
                                                            wait2,
                                                            action_after,
                                                            NULL));
    }
}

void Character::runActionTakeCard(std::vector<Card*> take_card_list)
{
    float game_speed = GameScene::getGameScene()->getGameSpeed();
    float delay_time = 1 / game_speed;
    
    for (int card_index = 0; card_index < take_card_list.size(); card_index++) {
        Card* take_card = take_card_list.at(card_index);

        DelayTime* wait = DelayTime::create(delay_time);
        
        CallFunc* action_before = CallFunc::create([take_card]() {
            take_card->card_image->setGlobalZOrder(99);
            if (config_sound_setting == SETTING_ON) {
                cocos2d::experimental::AudioEngine::play2d("se_move.wav");
            }
        });
        
        int order;
        Vec2 pos;
        float card_scale;
        if (this->getType() == Character::TYPE_PLAYER) {
            order = 13;
            pos = take_card->player_character_card_position[order];
            card_scale = CARD_SCALE_PLAYER;
        } else {
            order = this->hands->count();
            pos = take_card->non_player_character_card_position[this->character_id][order];
            card_scale = CARD_SCALE_NPC;
        }
        Spawn* action_main = Spawn::create(MoveTo::create(0.3 / game_speed, pos),
                                           ScaleTo::create(0.3 / game_speed, card_scale, card_scale),
                                           NULL);
        
        CallFunc* action_after = CallFunc::create([this, take_card, order]() {
            take_card->card_image->setGlobalZOrder(order);
            if (this->getType() == Character::TYPE_NON_PLAYER) {
                take_card->card_image->setTexture(Director::getInstance()->getTextureCache()->getTextureForKey(take_card->background_card_image_file_name));
            }
            Manager::getManager()->process(this, Command::TAKE_IT, take_card);
        });
        
        take_card->card_image->runAction(Sequence::create(wait,
                                                            action_before,
                                                            action_main,
                                                            action_after,
                                                            NULL));
    }
}

void Character::runActionPass()
{
    float game_speed = GameScene::getGameScene()->getGameSpeed();
    float delay_time = (getType() == Character::TYPE_PLAYER) ? 0 : (1 / game_speed);

    DelayTime* wait = DelayTime::create(delay_time);

    CallFunc* action_main = CallFunc::create([this]() {
        // プレイヤーの場合のみ「パス」を表示
        if (this->getType() == Character::TYPE_PLAYER) {
            this->label_message->setString("パス");
        }
        this->label_pass->setString(StringUtils::format("%d", this->pass));
        if (config_sound_setting == SETTING_ON) {
            cocos2d::experimental::AudioEngine::play2d("se_pass.wav");
        }

        // NPCの場合、ポップアップメッセージを表示
        this->showPopupMessage(MessageType::PASS);
    });

    DelayTime* wait2 = DelayTime::create(0.6 / game_speed);
    
    CallFunc* action_after = CallFunc::create([this]() {
        this->label_message->setString("");
        Manager::getManager()->process(this, Command::PUT_NORMAL, nullptr);
    });
    
    this->label_pass->runAction(Sequence::create(wait,
                                                 action_main,
                                                 wait2,
                                                 action_after,
                                                 NULL));
    
}

bool Character::isFinish()
{
    return this->hands->empty();
}

bool Character::isPlayer()
{
    return this->getType() == Character::TYPE_PLAYER;
}

void Character::showPopupMessage(MessageType type)
{
    // NPCの場合のみポップアップを表示
    if (this->getType() != Character::TYPE_NON_PLAYER || !this->thumbnail) {
        return;
    }

    float game_speed = GameScene::getGameScene()->getGameSpeed();
    std::random_device rd;
    std::mt19937 mt(rd());

    // キャラクターごと・状況ごとのセリフ
    std::map<MessageType, std::vector<std::string>> msgs_panda = {
        {MessageType::PASS,   {"むむ…", "計算外…", "想定内", "ふむ…"}},
        {MessageType::RANK_1, {"当然", "計算通り"}},
        {MessageType::RANK_2, {"まあまあ", "悪くない"}},
        {MessageType::RANK_3, {"むむむ…", "次は…"}},
        {MessageType::RANK_4, {"想定外…", "なぜ…"}}
    };
    std::map<MessageType, std::vector<std::string>> msgs_dog = {
        {MessageType::PASS,   {"クゥ〜ン", "ワン！", "キャン…", "くぅ…"}},
        {MessageType::RANK_1, {"ワンワン!", "やった！"}},
        {MessageType::RANK_2, {"ワン！", "まぁまぁ"}},
        {MessageType::RANK_3, {"クゥン…", "うぅ…"}},
        {MessageType::RANK_4, {"キャイン", "しょぼん"}}
    };
    std::map<MessageType, std::vector<std::string>> msgs_rabbit = {
        {MessageType::PASS,   {"あれ？", "えへへ", "うーん", "ぴょん"}},
        {MessageType::RANK_1, {"やったー", "ぴょん！"}},
        {MessageType::RANK_2, {"えへへ", "まあまあ"}},
        {MessageType::RANK_3, {"あれれ？", "うーん"}},
        {MessageType::RANK_4, {"えーん", "しくしく"}}
    };

    // レアセリフ（パス時のみ、5%の確率）
    std::map<int, std::string> rare_msgs = {
        {1, "笹食べたい"},
        {2, "散歩行く？"},
        {3, "月が綺麗"}
    };

    std::string popup_msg;

    // レア判定（パス時のみ）
    bool is_rare = false;
    if (type == MessageType::PASS) {
        std::uniform_int_distribution<int> rare_dist(1, 100);
        is_rare = (rare_dist(mt) <= 5);
    }

    if (is_rare) {
        popup_msg = rare_msgs[this->character_id];
    } else {
        std::vector<std::string>* msgs = nullptr;
        switch (this->character_id) {
            case 1: msgs = &msgs_panda[type]; break;
            case 2: msgs = &msgs_dog[type]; break;
            case 3: msgs = &msgs_rabbit[type]; break;
        }
        if (msgs && !msgs->empty()) {
            std::uniform_int_distribution<int> dist(0, msgs->size() - 1);
            popup_msg = (*msgs)[dist(mt)];
        }
    }

    if (popup_msg.empty()) return;

    // 背景付きポップアップを作成
    auto popupNode = Node::create();

    auto label = Label::createWithSystemFont(popup_msg, "Arial", 24);
    label->setColor(Color3B::BLACK);
    label->setAnchorPoint(Vec2(0, 0.5f));
    Size labelSize = label->getContentSize();

    float padding = 8.0f;
    auto bg = DrawNode::create();
    Vec2 origin(-padding, -labelSize.height/2 - padding);
    Vec2 dest(labelSize.width + padding, labelSize.height/2 + padding);
    bg->drawSolidRect(origin, dest, Color4F::WHITE);
    bg->drawRect(origin, dest, Color4F(0.3f, 0.3f, 0.3f, 1.0f));
    bg->setGlobalZOrder(100);
    label->setGlobalZOrder(101);
    popupNode->addChild(bg);
    popupNode->addChild(label);

    Vec2 pos = this->thumbnail->getPosition() + Vec2(60, 30);
    popupNode->setPosition(pos);
    popupNode->setOpacity(0);
    popupNode->setCascadeOpacityEnabled(true);
    GameScene::getGameScene()->addChild(popupNode, 50);

    auto fadeIn = FadeIn::create(0.15f);
    auto stay = DelayTime::create(0.8f / game_speed);
    auto fadeOut = FadeOut::create(0.15f);
    auto remove = RemoveSelf::create();
    popupNode->runAction(Sequence::create(fadeIn, stay, fadeOut, remove, nullptr));
}
