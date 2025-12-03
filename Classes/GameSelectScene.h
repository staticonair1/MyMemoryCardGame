// Classes/GameSelectScene.h

#ifndef __GAME_SELECT_SCENE_H__
#define __GAME_SELECT_SCENE_H__

#include "cocos2d.h"

class GameSelectScene : public cocos2d::Scene
{
public:
    // متد ایستا برای ایجاد صحنه
    static cocos2d::Scene* createScene();

    // متد اولیه سازی صحنه
    virtual bool init();

    // متدها برای مدیریت لمس دکمه ها
    void menuGame1Callback(cocos2d::Ref* pSender);
    void menuGame2Callback(cocos2d::Ref* pSender);
    void menuGame3Callback(cocos2d::Ref* pSender);

    // ماکرو برای ایجاد نمونه کلاس
    CREATE_FUNC(GameSelectScene);
};

#endif // __GAME_SELECT_SCENE_H__