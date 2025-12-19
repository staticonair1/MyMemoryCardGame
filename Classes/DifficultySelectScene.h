
#ifndef __DIFFICULTY_SELECT_SCENE_H__
#define __DIFFICULTY_SELECT_SCENE_H__

#include "cocos2d.h"

class DifficultySelectScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init() override;
    CREATE_FUNC(DifficultySelectScene);

private:
    void onDifficultySelected(cocos2d::Ref* sender, int difficulty);
    void onGoToMenu(cocos2d::Ref* sender);
};

#endif // __DIFFICULTY_SELECT_SCENE_H__
