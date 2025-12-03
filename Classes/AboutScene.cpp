#include "AboutScene.h"
#include "MenuScene.h" // برای بازگشت

USING_NS_CC;

Scene* AboutScene::createScene()
{
    return AboutScene::create();
}

bool AboutScene::init()
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

    // ۲. متن About Me (شما می‌توانید این متن را در آینده تغییر دهید)
    auto aboutText = "Creator:Morteza Hasanzadeh\n"
                     "cocos2dx/NDK/c++/Pythone27/gradle7.4all"
                     "keep in touch with me:Telegram:@Coachmorteza";
                     
    auto textLabel = Label::createWithTTF(aboutText, "fonts/Marker Felt.ttf", 24);
    textLabel->setPosition(origin.x + visibleSize.width / 2, 
                           origin.y + visibleSize.height / 2);
    // تراز متن را روی مرکز تنظیم کنید
    textLabel->setAlignment(TextHAlignment::CENTER);
    this->addChild(textLabel, 1);

    // ۳. دکمه Go Back to Menu
    auto backItem = MenuItemFont::create("Go Back to Menu", CC_CALLBACK_1(AboutScene::onGoToMenu, this));
    backItem->setFontSize(30);
    backItem->setPosition(Vec2(visibleSize.width / 2, 80)); // پایین صفحه

    auto menu = Menu::create(backItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 1);

    return true;
}

void AboutScene::onGoToMenu(Ref* sender)
{
    // بازگشت به صحنه‌ی منو
    auto scene = MenuScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
}