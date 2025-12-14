//
//  NativeCodeLauncher.cpp
//  Sevens
//
//  Created by shinnichirou.takahashi on 2017/04/08.
//
//

#include "CocosHelper.h"
#include "NativeCodeLauncher.h"

void NativeCodeLauncher::advertise() {
    CocosHelper::launchInterstitial();
}

int NativeCodeLauncher::getPreferencesInt(std::string key)
{
    return CocosHelper::getPreferencesInt(key);
}

void NativeCodeLauncher::putPreferencesInt(std::string key, int value)
{
    CocosHelper::putPreferencesInt(key, value);
}