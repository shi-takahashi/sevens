//
//  NonPlayerCharacter.h
//  Sevens
//
//  Created by shinnichirou.takahashi on 2018/09/09.
//
//

#ifndef NonPlayerCharacter_h
#define NonPlayerCharacter_h

#include "AIBase.h"
#include "Character.h"

USING_NS_CC;

class NonPlayerCharacter : public Character
{
public:
    static NonPlayerCharacter* create(int character_id, const std::string& filename = "");
    NonPlayerCharacter(int character_id, const std::string& filename) : Character(character_id, filename) {};
    ~NonPlayerCharacter() {};
    bool init() override;
    int getType() override;
    void setAI(AIBase* ai);
    std::vector<Card*> process(Command command, Card* designated_card = nullptr) override;

private:
    AIBase* ai;

    Card* choiceCard() override;
    
};

#endif /* NonPlayerCharacter_h */
