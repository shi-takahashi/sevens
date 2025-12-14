//
//  AI1.h
//  Sevens
//
//  Created by shinnichirou.takahashi on 2018/09/16.
//
//

#ifndef AI1_h
#define AI1_h

#include "AIBase.h"

USING_NS_CC;

class AI1 : public AIBase
{
public:
    static AI1* create();
    Card* choiceCard(std::vector<Card*> hand_cards);
    
protected:
    
private:
    AI1() {};
    ~AI1() {};
    bool init();
    
};

#endif /* AI1_h */
