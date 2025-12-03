// در MemoryCard.cpp
#include "MemoryCard.h"
#include "cocos2d.h"

USING_NS_CC;

// متغیرهایی برای نگهداری بافت‌ها در سطح کلاس
static cocos2d::Texture2D* s_frontTexture = nullptr;
static cocos2d::Texture2D* s_backTexture = nullptr;

MemoryCard* MemoryCard::createCard(int cardID, const std::string& frontImage, const std::string& backImage)
{
    // بارگذاری بافت‌ها یک بار و ذخیره در Cache (اگر قبلاً بارگذاری نشده باشند)
    s_frontTexture = Director::getInstance()->getTextureCache()->addImage(frontImage);
    s_backTexture = Director::getInstance()->getTextureCache()->addImage(backImage);

    // اگر یکی از بافت‌ها بارگذاری نشد، خطا بده
    if (!s_frontTexture || !s_backTexture) {
        CCLOGERROR("Failed to load textures for cards!");
        return nullptr;
    }

    // ایجاد یک نمونه با فراخوانی init (که در آن Sprite::initWithTexture فراخوانی می‌شود)
    MemoryCard* card = new (std::nothrow) MemoryCard();
    if (card && card->initWithTexture(s_backTexture)) // <--- از initWithTexture استفاده کنید
    {
        card->autorelease();
        card->_cardID = cardID;
        card->_backTexture = s_backTexture;
        card->_frontTexture = s_frontTexture;

        card->setAnchorPoint(Vec2(0.5f, 0.5f));

        // حالا که بافت در init تنظیم شده، getContentSize() معتبر است!
        return card;
    }
    CC_SAFE_DELETE(card);
    return nullptr;
}

// init را تغییر دهید تا از Sprite::initWithTexture فراخوانی نشود، چون createCard خودش این کار را می‌کند
bool MemoryCard::init()
{
    // چون createCard مستقیماً initWithTexture را فراخوانی می‌کند، 
    // این init نیازی به فراخوانی Sprite::init() ندارد یا باید فقط برگرداند true
    return true;
}

void MemoryCard::flipToFront(float duration) {
    if (getIsFlipped()) {
        return;
    }
    setIsFlipped(true);

    auto orbit1 = cocos2d::OrbitCamera::create(duration / 2, 1, 0, 0, 90, 0, 0);
    auto setFrontTexture = cocos2d::CallFunc::create([this]() {
        this->setTexture(_frontTexture);
    });
    auto orbit2 = cocos2d::OrbitCamera::create(duration / 2, 1, 0, -90, 90, 0, 0);

    this->runAction(cocos2d::Sequence::create(orbit1, setFrontTexture, orbit2, nullptr));
}

void MemoryCard::flipToBack(float duration) {
    if (!getIsFlipped()) {
        return;
    }
    setIsFlipped(false);

    auto orbit1 = cocos2d::OrbitCamera::create(duration / 2, 1, 0, 0, 90, 0, 0);
    auto setBackTexture = cocos2d::CallFunc::create([this]() {
        this->setTexture(_backTexture);
    });
    auto orbit2 = cocos2d::OrbitCamera::create(duration / 2, 1, 0, -90, 90, 0, 0);

    this->runAction(cocos2d::Sequence::create(orbit1, setBackTexture, orbit2, nullptr));
}

void MemoryCard::removeCard() {
    auto fadeOut = cocos2d::FadeOut::create(0.5f);
    auto remove = cocos2d::RemoveSelf::create();
    this->runAction(cocos2d::Sequence::create(fadeOut, remove, nullptr));
}
