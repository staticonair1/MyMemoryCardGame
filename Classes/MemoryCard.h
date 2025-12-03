#ifndef __MEMORY_CARD_H__
#define __MEMORY_CARD_H__

#include "cocos2d.h"

class MemoryCard : public cocos2d::Sprite
{
public:
    static MemoryCard* createCard(int cardID, const std::string& frontImage, const std::string& backImage);
    bool init() override;

    // ویژگی‌های کارت
    CC_SYNTHESIZE_READONLY(int, _cardID, CardID); // شناسه کارت برای مقایسه
    CC_SYNTHESIZE(bool, _isFlipped, IsFlipped);    // وضعیت: رو است یا پشت

    void flipToFront(float duration = 0.2f);
    void flipToBack(float duration = 0.2f);
    void removeCard(); // برای حذف پس از پیدا شدن جفت

private:
    cocos2d::Texture2D* _frontTexture;
    cocos2d::Texture2D* _backTexture;
};

#endif // __MEMORY_CARD_H__