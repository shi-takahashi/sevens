//
// Created by user on 2016/11/06.
//

#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include "CocosHelper.h"
#include "GameScene.h"

#define CLASS_NAME "org/cocos2dx/cpp/AppActivity"

void CocosHelper::launchInterstitial()
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "launchInterstitial", "()V")) {
        t.env->CallStaticVoidMethod(t.classID, t.methodID);
        t.env->DeleteLocalRef(t.classID);
    }
}

int CocosHelper::getPreferencesInt(std::string key)
{
    int ret = 0;
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "getPreferencesInt", "(Ljava/lang/String;)I")) {
        jstring jkey = t.env->NewStringUTF(key.c_str());
        ret = t.env->CallStaticIntMethod(t.classID, t.methodID, jkey);
        t.env->DeleteLocalRef(jkey);
        t.env->DeleteLocalRef(t.classID);
    }
    return ret;
}

void CocosHelper::putPreferencesInt(std::string key, int value)
{
    cocos2d::JniMethodInfo t;
    if (cocos2d::JniHelper::getStaticMethodInfo(t, CLASS_NAME, "putPreferencesInt", "(Ljava/lang/String;I)V")) {
        jstring jkey = t.env->NewStringUTF(key.c_str());
        t.env->CallStaticVoidMethod(t.classID, t.methodID, jkey, value);
        t.env->DeleteLocalRef(jkey);
        t.env->DeleteLocalRef(t.classID);
    }
}

void Java_org_cocos2dx_cpp_AppActivity_resumeNext()
{
    GameScene::getGameScene()->resumeNext();
}