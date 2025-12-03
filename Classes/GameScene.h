#ifndef __GAME_SCENE_H__
#define __GAME_SCENE_H__

#include "cocos2d.h"
#include "MemoryCard.h"
#include <vector>

class GameScene : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    CREATE_FUNC(GameScene);

    // ********************************************
    // ✅ جدید: تابع دکمه Go Back to Menu
    // ********************************************
    void onGoToMenu(cocos2d::Ref* sender);

private:
    // ********************************************
    // ✅ جدید: متغیرهای امتیاز و لیبل
    // ********************************************
    int _score = 0;                     // متغیر برای نگهداری امتیاز فعلی
    cocos2d::Label* _scoreLabel = nullptr; // لیبل برای نمایش امتیاز

    // متغیرهای وضعیت بازی
    std::vector<MemoryCard*> _allCards; // لیست تمام کارت‌ها
    MemoryCard* _firstCard = nullptr;  // کارت اول انتخاب شده
    MemoryCard* _secondCard = nullptr; // کارت دوم انتخاب شده
    bool _isProcessing = false; // جلوگیری از کلیک در حین پردازش
    int _matchesFound = 0;      // تعداد جفت‌های پیدا شده

    // توابع اصلی
    void setupGame();
    void createCardGrid(int rows, int cols);
    void cardTapped(MemoryCard* card);
    void checkMatch();

    // توابع جدید برای مدیریت بازی مجدد
    void gameFinished();
    void resetGame();
};

#endif // __GAME_SCENE_H__