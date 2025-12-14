//
//  NetRanking.hpp
//  Sevens
//
//  Created by shinnichirou.takahashi on 2018/10/16.
//
//

#ifndef NetRanking_hpp
#define NetRanking_hpp

#include "cocos2d.h"

class NetRanking : public cocos2d::Ref
{
public:
    static NetRanking* getNetRanking();
    NetRanking() {};
    ~NetRanking() {};
    bool init();
    void getRanking(int user_id, std::function<void(std::vector<std::map<std::string, std::string>> ranking_data)> success, std::function<void(void)> cancel = nullptr);
    void getMyRank(int user_id, std::function<void(int ranking)>, std::function<void(void)> cancel = nullptr);
    void setMyRank(int user_id, std::string name, int win, int lose, std::function<void(int id)>, std::function<void(void)> cancel = nullptr);
    
private:
    static NetRanking* net_ranking;
    
};

#endif /* NetRanking_hpp */
