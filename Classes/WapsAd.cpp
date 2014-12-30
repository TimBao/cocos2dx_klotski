#include "WapsAd.h"

WapsAd::WapsAd(){}
WapsAd::~WapsAd(){}

void WapsAd::showAd(int adTag)
{
#if (CC_TARGET_PLATFORM == CC_PLATFORM_ANDROID)
    JniMethodInfo showAd;

    bool isHave = JniHelper::getStaticMethodInfo(showAd,"com/tim/game/klotski/klotski","showAdStatic", "(I)V");

    if (!isHave)
    {
        CCLog("jni:showAdStatic is null");
    } else
    {
        showAd.env->CallStaticVoidMethod(showAd.classID, showAd.methodID,adTag);
    }
#endif
}
