#ifndef __ABOUT_SCENE_H__
#define __ABOUT_SCENE_H__

#include "cocos2d.h" // ✅ ' " " ' و ';' اضافه شد

class AboutScene : public cocos2d::Scene // ✅ ' : ' و ' :: ' اضافه شد
{
public: // ✅ ':' اضافه شد
    static cocos2d::Scene* createScene(); // ✅ '*' و ';' اضافه شد
    virtual bool init(); // ✅ ';' اضافه شد

    void onGoToMenu(cocos2d::Ref* sender); // ✅ '*' و ';' اضافه شد

    CREATE_FUNC(AboutScene); // ✅ ';' اضافه شد
};

#endif // __ABOUT_SCENE_H__