#ifndef __MENU_SCENE_H__
#define __MENU_SCENE_H__

#include "cocos2d.h"

class MenuScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();

    virtual bool init();
    
    // توابع فراخوانی برای دکمه‌ها
    void onPlay(cocos2d::Ref* sender);
    void onQuit(cocos2d::Ref* sender);
    void onAbout(cocos2d::Ref* sender);

    // ماکروی استاندارد Cocos2d-x
    CREATE_FUNC(MenuScene);
};

#endif // __MENU_SCENE_H__