// Classes/GameSelectScene.cpp

#include "GameSelectScene.h"
#include "GameScene.h"      // کلاس بازی کارتی شما (بازی اول)
#include "GridGame.h"       // کلاس بازی مربع ها (بازی دوم)
#include "ui/CocosGUI.h"    // نیاز برای استفاده از دکمه‌های Cocos2d-x UI

USING_NS_CC;

// **************************************************
// ✅ ۱. پیاده سازی تابع گمشده createScene()
// **************************************************
Scene* GameSelectScene::createScene()
{
    return GameSelectScene::create();
}

// **************************************************
// ✅ ۲. پیاده سازی تابع گمشده init() و ساخت دکمه ها
// **************************************************
bool GameSelectScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();

    // پس زمینه ساده
    auto background = LayerColor::create(Color4B(40, 60, 80, 255));
    this->addChild(background, 0);

    // عنوان
    auto titleLabel = Label::createWithSystemFont("بازی خود را انتخاب کنید", "Arial", 54);
    titleLabel->setPosition(visibleSize.width / 2, visibleSize.height - 100);
    this->addChild(titleLabel, 1);

    // --- ایجاد منوی اصلی ---

    // دکمه بازی ۱
    auto item1 = MenuItemFont::create("Play Game 1 (Card Match)", CC_CALLBACK_1(GameSelectScene::menuGame1Callback, this));
    item1->setFontSize(40);

    // دکمه بازی ۲
    auto item2 = MenuItemFont::create("Play Game 2 (Yellow Squares)", CC_CALLBACK_1(GameSelectScene::menuGame2Callback, this));
    item2->setFontSize(40);

    // دکمه بازی ۳
    auto item3 = MenuItemFont::create("Play Game 3 (Future Game)", CC_CALLBACK_1(GameSelectScene::menuGame3Callback, this));
    item3->setFontSize(40);

    // ایجاد منو و چیدمان عمودی
    auto menu = Menu::create(item1, item2, item3, nullptr);
    menu->alignItemsVerticallyWithPadding(40);
    menu->setPosition(visibleSize.width / 2, visibleSize.height / 2);
    this->addChild(menu, 1);

    return true;
}

// **************************************************
// ✅ ۳. پیاده سازی توابع کال‌بک برای انتقال صحنه
// **************************************************
void GameSelectScene::menuGame1Callback(Ref* pSender)
{
    // انتقال به بازی کارتی موجود
    auto scene = GameScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
}

void GameSelectScene::menuGame2Callback(Ref* pSender)
{
    // انتقال به بازی جدید مربع های زردشونده
    auto scene = GridGame::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
}

void GameSelectScene::menuGame3Callback(Ref* pSender)
{
    // بازی سوم - فعلا یک پیام نمایش می دهیم
    CCLOG("Game 3 Selected - To be implemented!");
}