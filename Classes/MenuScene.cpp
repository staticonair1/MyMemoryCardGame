#include "MenuScene.h"
#include "GameScene.h"
#include "AboutScene.h"
#include "GameSelectScene.h"

USING_NS_CC;

Scene* MenuScene::createScene()
{
    return MenuScene::create();
}

bool MenuScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    // ۱. پس‌زمینه
    auto background = LayerColor::create(Color4B(30, 30, 50, 255));
    this->addChild(background, 0);
    
    // ۲. عنوان بازی
    auto titleLabel = Label::createWithTTF("Memory Match Game", "fonts/Marker Felt.ttf", 48);
    titleLabel->setPosition(origin.x + visibleSize.width / 2, 
                            origin.y + visibleSize.height - 100);
    this->addChild(titleLabel, 1);
    
    // ۳. ایجاد دکمه‌ها
    
    // دکمه Play
    auto playItem = MenuItemFont::create("Play", CC_CALLBACK_1(MenuScene::onPlay, this));
    playItem->setFontSize(36);
    playItem->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 + 50));
    
    // دکمه About Me
    auto aboutItem = MenuItemFont::create("About Me", CC_CALLBACK_1(MenuScene::onAbout, this));
    aboutItem->setFontSize(36);
    aboutItem->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2));

    // دکمه Quit
    auto quitItem = MenuItemFont::create("Quit", CC_CALLBACK_1(MenuScene::onQuit, this));
    quitItem->setFontSize(36);
    quitItem->setPosition(Vec2(visibleSize.width / 2, visibleSize.height / 2 - 50));

    // ایجاد منو و اضافه کردن دکمه‌ها
    auto menu = Menu::create(playItem, aboutItem, quitItem, nullptr);
    menu->setPosition(Vec2::ZERO); // موقعیت دهی توسط خود آیتم‌ها انجام شده است
    this->addChild(menu, 1);

    return true;
}

void MenuScene::onPlay(Ref* sender)
{
    // *** تغییر اصلی در این تابع است ***

    // ۱. ایجاد صحنه جدید انتخاب بازی
    auto scene = GameSelectScene::createScene();

    // ۲. انتقال به صحنه انتخاب بازی به جای شروع مستقیم بازی اول
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
}
void MenuScene::onQuit(Ref* sender)
{
    // بستن کامل بازی
    Director::getInstance()->end();
    
    #if (CC_TARGET_PLATFORM == CC_PLATFORM_IOS)
        exit(0);
    #endif
}

void MenuScene::onAbout(Ref* sender)
{
    // انتقال به صحنه‌ی About Me
    auto scene = AboutScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
}