//
//  NativeCodeLauncher.hpp
//  Sevens
//
//  Created by shinnichirou.takahashi on 2017/04/08.
//
//

#ifndef SEVENS_NATIVECODELAUNCHER_H
#define SEVENS_NATIVECODELAUNCHER_H

#include <stdio.h>

class NativeCodeLauncher
{
public:
    static void advertise();
    static int getPreferencesInt(std::string key);
    static void putPreferencesInt(std::string key, int value);
private:
};


#endif /* SEVENS_NATIVECODELAUNCHER_H */
