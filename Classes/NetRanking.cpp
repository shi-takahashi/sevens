//
//  NetRanking.cpp
//  Sevens
//
//  Created by shinnichirou.takahashi on 2018/10/16.
//
//

#include "Common.h"
#include "NetRanking.h"
#include "network/HttpClient.h"
#include "picojson.h"

USING_NS_CC;
using namespace cocos2d::network;

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

void NetRanking::getRanking(int user_id, std::function<void(std::vector<std::map<std::string, std::string>> ranking_data)> success, std::function<void(void)> cancel)
{
    if (config_use_net == SETTING_OFF) {
        if (cancel) {
            cancel();
        }
        return;
    }
    
    auto request = new HttpRequest;
    request->setUrl("https://x723.xsrv.jp/sevens/get_ranking.php");
    request->setRequestType(HttpRequest::Type::POST);
    request->setResponseCallback([this, success, cancel](HttpClient* client, HttpResponse* response) {
//        log("responseCode:%ld %s", response->getResponseCode(), response->getHttpRequest()->getUrl());
        if (response->isSucceed()) {
            // json.
            std::vector<char> *buffer = response->getResponseData();
            const char *data = reinterpret_cast<char *>(&(buffer->front()));
            picojson::value v;
            std::string error;
            picojson::parse(v, data, data + strlen(data), &error);
            if (error.empty())
            {
                std::vector<std::map<std::string, std::string>> ranking_data = {};
                picojson::array& array = v.get<picojson::array>();
                for (picojson::array::iterator it = array.begin(); it != array.end(); it++)
                {
                    picojson::object& obj = it->get<picojson::object>();
                    std::string &ranking = obj["ranking"].get<std::string>();
                    std::string &name = obj["name"].get<std::string>();
                    std::string &win = obj["win"].get<std::string>();
                    std::string &is_my_data = obj["is_my_data"].get<std::string>();
                    std::map<std::string, std::string> map = {
                        {"ranking",     ranking},
                        {"name",        name},
                        {"win",         win},
                        {"is_my_data",  is_my_data},
                    };
                    ranking_data.emplace_back(map);
                }
                success(ranking_data);
            }
        }
        else {
//            log("HttpRequest failed");
            if (cancel) {
                cancel();
            }
        }
    });
    
    char dataStr[256];
    sprintf(dataStr, "id=%d", user_id);
    std::string data = std::string(dataStr);
    const char* postData = data.c_str();
    request->setRequestData(postData, strlen(postData));
    auto client = HttpClient::getInstance();
    client->enableCookies(NULL);
    client->send(request);
}

void NetRanking::getMyRank(int user_id, std::function<void(int ranking)> success, std::function<void(void)> cancel)
{
    if (config_use_net == SETTING_OFF || user_id == 0) {
        if (cancel) {
            cancel();
        }
        return;
    }
    
    auto request = new HttpRequest;
    request->setUrl("https://x723.xsrv.jp/sevens/get_my_rank.php");
    request->setRequestType(HttpRequest::Type::POST);
    request->setResponseCallback([this, success, cancel](HttpClient* client, HttpResponse* response) {
//        log("responseCode:%ld %s", response->getResponseCode(), response->getHttpRequest()->getUrl());
        if (response->isSucceed()) {
            // json.
            std::vector<char> *buffer = response->getResponseData();
            const char *data = reinterpret_cast<char *>(&(buffer->front()));
            picojson::value v;
            std::string error;
            picojson::parse(v, data, data + strlen(data), &error);
            if (error.empty())
            {
                picojson::object& obj = v.get<picojson::object>();
                std::string &ranking = obj["ranking"].get<std::string>();
                success(atoi(ranking.c_str()));
            }
        }
        else {
//            log("HttpRequest failed");
            if (cancel) {
                cancel();
            }
        }
    });

    char dataStr[256];
    sprintf(dataStr, "id=%d", user_id);
    std::string data = std::string(dataStr);
    const char* postData = data.c_str();
    request->setRequestData(postData, strlen(postData));
    auto client = HttpClient::getInstance();
    client->enableCookies(NULL);
    client->send(request);
}

void NetRanking::setMyRank(int id, std::string name, int win, int lose, std::function<void(int id)> success, std::function<void(void)> cancel)
{
    if (config_use_net == SETTING_OFF) {
        if (cancel) {
            cancel();
        }
        return;
    }

    auto request = new HttpRequest;
    request->setUrl("https://x723.xsrv.jp/sevens/set_my_rank.php");
    request->setRequestType(HttpRequest::Type::POST);
    request->setResponseCallback([this, success, cancel](HttpClient* client, HttpResponse* response) {
//        log("responseCode:%ld %s", response->getResponseCode(), response->getHttpRequest()->getUrl());
        if (response->isSucceed()) {
            // json.
            std::vector<char> *buffer = response->getResponseData();
            const char *data = reinterpret_cast<char *>(&(buffer->front()));
            picojson::value v;
            std::string error;
            picojson::parse(v, data, data + strlen(data), &error);
            if (error.empty())
            {
                picojson::object& obj = v.get<picojson::object>();
                std::string &id = obj["id"].get<std::string>();
                success(atoi(id.c_str()));
            }
        }
        else {
//            log("HttpRequest failed");
            if (cancel) {
                cancel();
            }
        }
    });
    
    char dataStr[256];
    if (id > 0) {
        sprintf(dataStr, "id=%d&name=%s&win=%d&lose=%d", id, name.c_str(), win, lose);
    } else {
        sprintf(dataStr, "name=%s&win=%d&lose=%d", name.c_str(), win, lose);
    }
    std::string data = std::string(dataStr);
    const char* postData = data.c_str();
    request->setRequestData(postData, strlen(postData));
    auto client = HttpClient::getInstance();
    client->enableCookies(NULL);
    client->send(request);
}
