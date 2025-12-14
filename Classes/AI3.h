//
//  AI3.h
//  Sevens
//
//  Created by shinnichirou.takahashi on 2018/09/16.
//
//

#ifndef AI3_h
#define AI3_h

#include "AIBase.h"

USING_NS_CC;

class AI3 : public AIBase
{
public:
    static AI3* create();
    Card* choiceCard(std::vector<Card*> hand_cards);
    
protected:
    
private:
    AI3() {};
    ~AI3() {};
    bool init();
    
};

#endif /* AI3_h */
