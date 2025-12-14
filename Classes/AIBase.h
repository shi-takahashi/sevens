//
//  AIBase.h
//  Sevens
//
//  Created by shinnichirou.takahashi on 2018/09/16.
//
//

#ifndef AIBase_h
#define AIBase_h

#include "Card.h"

USING_NS_CC;

class AIBase : public cocos2d::Ref
{
public:
    virtual Card* choiceCard(std::vector<Card*> hand_cards);
    
protected:
    AIBase() {};
    ~AIBase() {};
    
private:
    bool init();
    
};

#endif /* AIBase_h */
