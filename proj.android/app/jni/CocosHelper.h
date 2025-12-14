//
// Created by user on 2016/11/06.
//

#ifndef SEVENS_COCOSHELPER_H
#define SEVENS_COCOSHELPER_H

#include <jni.h>
#include "cocos2d.h"
#include <unistd.h>
#ifndef _Included_org_cocos2dx_cpp_AppActivity
#define _Included_org_cocos2dx_cpp_AppActivity

class CocosHelper {
public:
    static void launchInterstitial();
    static int getPreferencesInt(std::string key);
    static void putPreferencesInt(std::string key, int value);
};

extern "C" {

JNIEXPORT void JNICALL Java_org_cocos2dx_cpp_AppActivity_resumeNext();

}

#endif

#endif //SEVENS_NATIVECODELAUNCHER_H
