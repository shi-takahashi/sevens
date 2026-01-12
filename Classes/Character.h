//
//  Character.h
//  Sevens
//
//  Created by shinnichirou.takahashi on 2018/09/08.
//
//

#ifndef Character_h
#define Character_h

#include "audio/include/AudioEngine.h"
#include "Hands.h"

USING_NS_CC;

// セリフの種類
enum class MessageType {
    PASS,
    RANK_1,
    RANK_2,
    RANK_3,
    RANK_4
};

class Character : public cocos2d::Ref
{
public:
    static const int TYPE_PLAYER;
    static const int TYPE_NON_PLAYER;

    int character_id;
    Sprite* thumbnail;
    Hands* hands;
    int rank;
    int pass;
    Label* label_rank;
    Label* label_pass;
    Label* label_message;
    
    Character(int character_id, const std::string& filename = "");
    ~Character();
    virtual bool init();
    virtual int getType() { return 0; };
    virtual std::vector<Card*> process(Command command, Card* designated_card = nullptr);
    bool isFinish();
    bool isPlayer();
    void showPopupMessage(MessageType type);

private:
    virtual Card* choiceCard() { return nullptr; };
    std::vector<Card*> extractCardList(Command, Card* designated_card = nullptr);
    void runAction(Command, std::vector<Card*>);
    void runActionPutCard(Command, std::vector<Card*>);
    void runActionTakeCard(std::vector<Card*>);
    void runActionPass();
};

#endif /* Character_h */
