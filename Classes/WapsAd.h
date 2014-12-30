#ifndef __WAPSAD_H__
#define __WAPSAD_H__

#ifdef ANDROID_NDK
#define JNI
#endif

#ifdef JNI
#include <jni.h>
#include "platform/android/jni/JniHelper.h"
#include <android/log.h>
#endif

#include "cocos2d.h"

USING_NS_CC;

class WapsAd
{
public:
    WapsAd();
    virtual ~WapsAd();
    static void showAd(int adTag);
};

#endif //__WAPSAD_H__