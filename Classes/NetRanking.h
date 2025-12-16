//
//  NetRanking.hpp
//  Sevens
//
//  Created by shinnichirou.takahashi on 2018/10/16.
//  Modified for Firebase Firestore integration
//

#ifndef NetRanking_hpp
#define NetRanking_hpp

#include "cocos2d.h"
#include <functional>
#include <vector>
#include <map>
#include <string>

class NetRanking : public cocos2d::Ref
{
public:
    static NetRanking* getNetRanking();
    NetRanking() {};
    ~NetRanking() {};
    bool init();

    // Get top 100 rankings
    void getRanking(const std::string& odId,
                    std::function<void(std::vector<std::map<std::string, std::string>> ranking_data)> success,
                    std::function<void(void)> cancel = nullptr);

    // Get my rank
    void getMyRank(const std::string& odId,
                   std::function<void(int ranking)> success,
                   std::function<void(void)> cancel = nullptr);

    // Save my score (returns new odId if first time)
    void setMyRank(const std::string& odId,
                   const std::string& name,
                   int win,
                   int lose,
                   std::function<void(std::string odId)> success,
                   std::function<void(void)> cancel = nullptr);

    // Callbacks from Java (JNI)
    void onRankingLoaded(const std::string& jsonData);
    void onRankingLoadFailed();
    void onMyRankLoaded(int rank);
    void onMyRankLoadFailed();
    void onScoreSaved(const std::string& odId);
    void onScoreSaveFailed();

private:
    static NetRanking* net_ranking;

    // Pending callbacks
    std::function<void(std::vector<std::map<std::string, std::string>>)> _getRankingSuccessCallback;
    std::function<void()> _getRankingCancelCallback;
    std::function<void(int)> _getMyRankSuccessCallback;
    std::function<void()> _getMyRankCancelCallback;
    std::function<void(std::string)> _setMyRankSuccessCallback;
    std::function<void()> _setMyRankCancelCallback;
};

#endif /* NetRanking_hpp */
