//
//  NativeCodeLauncher.m
//  Sevens
//
//  Created by shinnichirou.takahashi on 2017/04/08.
//
//

#import <Foundation/Foundation.h>
#include "NativeCodeLauncher.h"

void NativeCodeLauncher::advertise()
{
}

int NativeCodeLauncher::getPreferencesInt(std::string key)
{
    return cocos2d::UserDefault::getInstance()->getIntegerForKey(key.c_str(), 0);
}

void NativeCodeLauncher::putPreferencesInt(std::string key, int value)
{
    cocos2d::UserDefault::getInstance()->setIntegerForKey(key.c_str(), value);
}


