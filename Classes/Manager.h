//
//  Manager.h
//  Sevens
//
//  Created by shinnichirou.takahashi on 2018/09/09.
//
//

#ifndef Manager_h
#define Manager_h

#include "Card.h"
#include "Character.h"

class Manager : public cocos2d::Ref
{
public:
    std::vector<Card*> table_cards;
    
    static Manager* getManager();
    Manager() {};
    ~Manager();
    bool init();
    void setCharacters(std::vector<Character*>&);
    void dealCard(std::vector<Card*> cards);
    void start();
    void process(Character* performer, Command command, Card* result_card = nullptr);
    void end();
    std::vector<Card*> askTargetCards();
    std::vector<Card*> askTargetCardsWithJoker();
    
private:
    static Manager* manager;
    std::vector<Character*> characters;
    Character* next_charcter;
    
    Card* findTargetMarkCard(Mark, int*, size_t);
    bool isHighLink(Mark);
    bool isLowLink(Mark);
    void updateCardData(Character*, Command, Card*);
    bool advanceNextCharacter();
    void migrateControlToNext();
    void setRank(Character*);
};

#endif /* Manager_h */
