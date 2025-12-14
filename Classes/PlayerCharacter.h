//
//  PlayerCharacter.h
//  Sevens
//
//  Created by shinnichirou.takahashi on 2018/09/09.
//
//

#ifndef PlayerCharacter_h
#define PlayerCharacter_h

#include "ui/CocosGUI.h"
#include "Character.h"

USING_NS_CC;

class PlayerCharacter : public Character
{
public:
    int streak_cur;
    int streak_max;
    bool is_touch_enabled;
    Card* touched_card;
    ui::Button* button_pass;
    
    static PlayerCharacter* create(int character_id, const std::string& filename = "");
    PlayerCharacter(int character_id, const std::string& filename) : Character(character_id, filename) {};
    ~PlayerCharacter() {};
    bool init() override;
    int getType() override;
    void sortCards();
    void setTouchEnabled(bool flg);
    
private:
    Card* choiceCard() override;

};

#endif /* PlayerCharacter_h */
