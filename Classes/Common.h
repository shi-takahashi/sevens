//
// Created by user on 2016/10/29.
//

#ifndef SEVENS_COMMON_H
#define SEVENS_COMMON_H

#define SETTING_OFF 0
#define SETTING_ON 1
#define SETTING_SLOW 1
#define SETTING_NORMAL 2
#define SETTING_QUICK 3
#define SETTING_NO_USE 0
#define SETTING_USE 1

#define USER_ID_NONE 0
#define USER_NAME_NONE "unknown"

#include <string>

extern int config_sound_setting;
extern int config_speed_setting;
extern int config_use_joker;
extern int config_use_tunnel;
extern int config_use_net;

extern int global_user_id;
extern std::string global_user_name;

constexpr int CARD_WIDTH { 200 };
constexpr int CARD_HEIGHT { 300 };
constexpr float CARD_SCALE_BOARD { 0.4f };
constexpr float CARD_SCALE_PLAYER { 0.6f };
constexpr float CARD_SCALE_NPC { 0.3f };
constexpr int PLAYER_COUNT { 4 };

enum class Mark
{
    SPADE = 1,
    HEART = 2,
    DIAMOND = 3,
    CLUB = 4,
    JOKER = 99,
};

enum class Command
{
    PUT_NORMAL  = 1,
    PUT_ALL     = 2,
    PUT_SEVENS  = 3,
    PUT_IT      = 4,
    TAKE_IT     = 5,
};

#endif //SEVENS_COMMON_H
