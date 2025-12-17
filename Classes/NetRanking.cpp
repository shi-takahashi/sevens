//
//  NetRanking.cpp
//  Sevens
//
//  Created by shinnichirou.takahashi on 2018/10/16.
//  Modified for Firebase Firestore integration
//

#include "Common.h"
#include "NetRanking.h"
#include "picojson.h"

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
#include "platform/android/jni/JniHelper.h"
#endif

USING_NS_CC;

NetRanking* NetRanking::net_ranking = nullptr;

NetRanking* NetRanking::getNetRanking()
{
    if (!net_ranking) {
        net_ranking = new NetRanking();
        if (!(net_ranking && net_ranking->init()))
            CC_SAFE_DELETE(net_ranking);
    }
    return net_ranking;
}

bool NetRanking::init()
{
    return true;
}

void NetRanking::getRanking(const std::string& odId,
                            std::function<void(std::vector<std::map<std::string, std::string>> ranking_data)> success,
                            std::function<void(void)> cancel)
{
    if (config_use_net == SETTING_OFF) {
        if (cancel) {
            cancel();
        }
        return;
    }

    _getRankingSuccessCallback = success;
    _getRankingCancelCallback = cancel;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniHelper::callStaticVoidMethod("org/cocos2dx/cpp/AppActivity", "getRanking", odId);
#else
    // For other platforms, just call cancel
    if (cancel) {
        cancel();
    }
#endif
}

void NetRanking::getMyRank(const std::string& odId,
                           std::function<void(int ranking)> success,
                           std::function<void(void)> cancel)
{
    if (config_use_net == SETTING_OFF || odId.empty()) {
        if (cancel) {
            cancel();
        }
        return;
    }

    _getMyRankSuccessCallback = success;
    _getMyRankCancelCallback = cancel;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniHelper::callStaticVoidMethod("org/cocos2dx/cpp/AppActivity", "getMyRank", odId);
#else
    if (cancel) {
        cancel();
    }
#endif
}

void NetRanking::setMyRank(const std::string& odId,
                           const std::string& name,
                           int win,
                           int rank2,
                           int rank3,
                           int rank4,
                           int streak,
                           std::function<void(std::string odId)> success,
                           std::function<void(void)> cancel)
{
    if (config_use_net == SETTING_OFF) {
        if (cancel) {
            cancel();
        }
        return;
    }

    _setMyRankSuccessCallback = success;
    _setMyRankCancelCallback = cancel;

#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniHelper::callStaticVoidMethod("org/cocos2dx/cpp/AppActivity", "setMyRank", odId, name, win, rank2, rank3, rank4, streak);
#else
    if (cancel) {
        cancel();
    }
#endif
}

// Callbacks from Java

void NetRanking::onRankingLoaded(const std::string& jsonData)
{
    if (!_getRankingSuccessCallback) return;

    std::vector<std::map<std::string, std::string>> ranking_data;

    picojson::value v;
    std::string error;
    picojson::parse(v, jsonData.begin(), jsonData.end(), &error);

    if (error.empty() && v.is<picojson::array>())
    {
        picojson::array& array = v.get<picojson::array>();
        for (auto& item : array)
        {
            if (!item.is<picojson::object>()) continue;
            picojson::object& obj = item.get<picojson::object>();

            std::map<std::string, std::string> map;
            map["ranking"] = obj["ranking"].is<std::string>() ? obj["ranking"].get<std::string>() : "";
            map["name"] = obj["name"].is<std::string>() ? obj["name"].get<std::string>() : "";
            map["win"] = obj["win"].is<std::string>() ? obj["win"].get<std::string>() : "";
            map["is_my_data"] = obj["is_my_data"].is<std::string>() ? obj["is_my_data"].get<std::string>() : "0";

            ranking_data.emplace_back(map);
        }
    }

    // Schedule callback on main thread
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, ranking_data]() {
        if (_getRankingSuccessCallback) {
            _getRankingSuccessCallback(ranking_data);
            _getRankingSuccessCallback = nullptr;
            _getRankingCancelCallback = nullptr;
        }
    });
}

void NetRanking::onRankingLoadFailed()
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([this]() {
        if (_getRankingCancelCallback) {
            _getRankingCancelCallback();
        }
        _getRankingSuccessCallback = nullptr;
        _getRankingCancelCallback = nullptr;
    });
}

void NetRanking::onMyRankLoaded(int rank)
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, rank]() {
        if (_getMyRankSuccessCallback) {
            _getMyRankSuccessCallback(rank);
        }
        _getMyRankSuccessCallback = nullptr;
        _getMyRankCancelCallback = nullptr;
    });
}

void NetRanking::onMyRankLoadFailed()
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([this]() {
        if (_getMyRankCancelCallback) {
            _getMyRankCancelCallback();
        }
        _getMyRankSuccessCallback = nullptr;
        _getMyRankCancelCallback = nullptr;
    });
}

void NetRanking::onScoreSaved(const std::string& odId)
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([this, odId]() {
        if (_setMyRankSuccessCallback) {
            _setMyRankSuccessCallback(odId);
        }
        _setMyRankSuccessCallback = nullptr;
        _setMyRankCancelCallback = nullptr;
    });
}

void NetRanking::onScoreSaveFailed()
{
    Director::getInstance()->getScheduler()->performFunctionInCocosThread([this]() {
        if (_setMyRankCancelCallback) {
            _setMyRankCancelCallback();
        }
        _setMyRankSuccessCallback = nullptr;
        _setMyRankCancelCallback = nullptr;
    });
}

// JNI callbacks from Java
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
extern "C" {

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_onRankingLoaded(JNIEnv* env, jclass clazz, jstring jsonData)
{
    const char* jsonDataStr = env->GetStringUTFChars(jsonData, nullptr);
    std::string data(jsonDataStr);
    env->ReleaseStringUTFChars(jsonData, jsonDataStr);

    NetRanking::getNetRanking()->onRankingLoaded(data);
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_onRankingLoadFailed(JNIEnv* env, jclass clazz)
{
    NetRanking::getNetRanking()->onRankingLoadFailed();
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_onMyRankLoaded(JNIEnv* env, jclass clazz, jint rank)
{
    NetRanking::getNetRanking()->onMyRankLoaded(static_cast<int>(rank));
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_onMyRankLoadFailed(JNIEnv* env, jclass clazz)
{
    NetRanking::getNetRanking()->onMyRankLoadFailed();
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_onScoreSaved(JNIEnv* env, jclass clazz, jstring odId)
{
    const char* odIdStr = env->GetStringUTFChars(odId, nullptr);
    std::string id(odIdStr);
    env->ReleaseStringUTFChars(odId, odIdStr);

    NetRanking::getNetRanking()->onScoreSaved(id);
}

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_onScoreSaveFailed(JNIEnv* env, jclass clazz)
{
    NetRanking::getNetRanking()->onScoreSaveFailed();
}

}
#endif
