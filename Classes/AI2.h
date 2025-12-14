//
//  AI2.h
//  Sevens
//
//  Created by shinnichirou.takahashi on 2018/09/16.
//
//

#ifndef AI2_h
#define AI2_h

#include "AIBase.h"

USING_NS_CC;

class AI2 : public AIBase
{
public:
    static AI2* create();
    Card* choiceCard(std::vector<Card*> hand_cards);
    
protected:
    
private:
    AI2() {};
    ~AI2() {};
    bool init();
    
};

#endif /* AI2_h */
