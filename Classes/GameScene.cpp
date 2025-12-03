#include "GameScene.h"
#include "MemoryCard.h"
#include "MenuScene.h" // ✅ اضافه شده برای بازگشت به منو
#include <algorithm>
#include <random>
#include <ctime>

USING_NS_CC;

Scene* GameScene::createScene()
{
    return GameScene::create();
}

bool GameScene::init()
{
    if ( !Scene::init() )
    {
        return false;
    }

    // اگر می‌خواهید امتیاز از اینجا شروع شود، باید 0 شود
    // فرض بر این است که متغیر _score در GameScene.h وجود دارد
    _score = 0;

    setupGame();
    return true;
}

void GameScene::setupGame()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // ۱. ایجاد پس زمینه
    auto background = LayerColor::create(Color4B(30, 30, 50, 255));
    this->addChild(background, 0, 99);

    // **********************************************
    // ✅ جدید: راه‌اندازی لیبل امتیاز در سمت چپ
    // **********************************************
    // فرض بر این است که _scoreLabel در GameScene.h تعریف شده است
    _scoreLabel = Label::createWithTTF(StringUtils::format("Score: %d", _score), "fonts/Marker Felt.ttf", 30);
    if (_scoreLabel)
    {
        // موقعیت در سمت چپ، کمی پایین‌تر از لبه بالایی
        _scoreLabel->setAnchorPoint(Vec2(0, 0.5)); // تراز به چپ
        _scoreLabel->setPosition(origin.x + 30, // حاشیه 30 از سمت چپ
                                 origin.y + visibleSize.height - 40);
        this->addChild(_scoreLabel, 2);
    }

    // **********************************************
    // ✅ جدید: دکمه Go Back to Menu در سمت راست
    // **********************************************
    auto backItem = MenuItemFont::create("Menu", CC_CALLBACK_1(GameScene::onGoToMenu, this));
    backItem->setFontSize(30);
    backItem->setAnchorPoint(Vec2(1, 0.5)); // تراز به راست
    backItem->setPosition(origin.x + visibleSize.width - 30, // حاشیه 30 از سمت راست
                          origin.y + visibleSize.height - 40); // هم‌تراز با امتیاز

    auto menu = Menu::create(backItem, nullptr);
    menu->setPosition(Vec2::ZERO);
    this->addChild(menu, 2);


    // ۲. ایجاد شبکه کارت‌ها
    createCardGrid(4, 4);

    // ۳. افزودن شنونده رویداد لمس
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = [this](Touch* touch, Event* event){
        if (_isProcessing) return false;

        for (auto card : _allCards)
        {
            if (card->getBoundingBox().containsPoint(touch->getLocation()))
            {
                cardTapped(card);
                return true;
            }
        }
        return false;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, this);
}

void GameScene::createCardGrid(int rows, int cols)
{
    int totalPairs = (rows * cols) / 2;
    std::vector<int> cardIDs;

    for (int i = 1; i <= totalPairs; ++i)
    {
        cardIDs.push_back(i);
        cardIDs.push_back(i);
    }

    std::shuffle(cardIDs.begin(), cardIDs.end(), std::default_random_engine(time(0)));

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin(); // برای هماهنگی

    // ۱. اصلاح مقیاس‌گذاری و مرکزیت
    float minPadding = 15;
    float screenMargin = 30;

    float maxCardWidth = (visibleSize.width - 2 * screenMargin - (cols - 1) * minPadding) / cols;
    float maxCardHeight = (visibleSize.height - 2 * screenMargin - (rows - 1) * minPadding) / rows;

    float cardWidth = std::min(maxCardWidth, maxCardHeight);
    float cardHeight = cardWidth;
    float padding = minPadding;

    float totalWidth = cols * cardWidth + (cols - 1) * padding;
    float totalHeight = rows * cardHeight + (rows - 1) * padding;

    // نقطه شروع برای مرکزیت دقیق شبکه
    Vec2 startPos = Vec2(visibleSize.width / 2 - totalWidth / 2,
                         visibleSize.height / 2 - totalHeight / 2);
    startPos = startPos + origin;

    // جابجایی به سمت بالا (20 پیکسل)
    float offsetY = 20;
    startPos.y += offsetY;

    int cardIndex = 0;
    for (int i = 0; i < rows; ++i)
    {
        for (int j = 0; j < cols; ++j)
        {
            int cardID = cardIDs[cardIndex];

            std::string frontImg = StringUtils::format("card_%d.png", cardID);
            std::string backImg = "card_back.png";

            auto card = MemoryCard::createCard(cardID, frontImg, backImg);

            // تنظیم موقعیت
            float x = startPos.x + j * (cardWidth + padding) + cardWidth / 2;
            float y = startPos.y + (rows - 1 - i) * (cardHeight + padding) + cardHeight / 2;
            card->setPosition(x, y);

            if (card->getContentSize().width > 0)
            {
                card->setScale(cardWidth / card->getContentSize().width);
            }

            this->addChild(card, 1);
            _allCards.push_back(card);
            cardIndex++;
        }
    }
}

void GameScene::cardTapped(MemoryCard* card)
{
    if (card->getIsFlipped()) return;

    card->flipToFront();

    if (_firstCard == nullptr)
    {
        _firstCard = card;
    }
    else if (_secondCard == nullptr)
    {
        _secondCard = card;
        _isProcessing = true;

        this->runAction(Sequence::create(DelayTime::create(0.7f),
                                         CallFunc::create(CC_CALLBACK_0(GameScene::checkMatch, this)),
                                         nullptr));
    }
}

void GameScene::checkMatch()
{
    if (_firstCard == nullptr || _secondCard == nullptr)
    {
        _isProcessing = false;
        return;
    }

    if (_firstCard->getCardID() == _secondCard->getCardID())
    {
        // جفت پیدا شد!
        _matchesFound++;

        // ✅ افزایش امتیاز (+10)
        _score += 10;

        // رفع کرش: حذف اشاره‌گرها از بردار _allCards
        _allCards.erase(std::remove(_allCards.begin(), _allCards.end(), _firstCard), _allCards.end());
        _allCards.erase(std::remove(_allCards.begin(), _allCards.end(), _secondCard), _allCards.end());

        // حذف کارت‌ها از صحنه
        _firstCard->removeCard();
        _secondCard->removeCard();
    }
    else
    {
        // جفت پیدا نشد!

        // ✅ کاهش امتیاز (-2)
        _score -= 2;

        // برگرداندن کارت‌ها
        _firstCard->flipToBack();
        _secondCard->flipToBack();
    }

    // ✅ به‌روزرسانی لیبل امتیاز
    if (_scoreLabel) {
        _scoreLabel->setString(StringUtils::format("Score: %d", _score));
    }

    // بازنشانی اشاره‌گرها و وضعیت پردازش
    _firstCard = nullptr;
    _secondCard = nullptr;
    _isProcessing = false;

    // شرط پایان بازی
    if (_allCards.empty())
    {
        gameFinished();
    }
}

void GameScene::gameFinished()
{
    // حذف شنونده‌ی لمس اصلی بازی
    _eventDispatcher->removeEventListenersForTarget(this);

    // نمایش پیام پایان بازی و امتیاز نهایی
    auto gameOverLabel = Label::createWithTTF(
            StringUtils::format("Game Over! Final Score: %d\nTap to Play Again.", _score),
            "fonts/Marker Felt.ttf",
            36);

    gameOverLabel->setPosition(Director::getInstance()->getVisibleSize() / 2);
    this->addChild(gameOverLabel, 5);

    // فعال کردن شنونده رویداد جدید برای شروع مجدد بازی (روی خود لیبل)
    auto touchListener = EventListenerTouchOneByOne::create();
    touchListener->setSwallowTouches(true);
    touchListener->onTouchBegan = [this, gameOverLabel](Touch* touch, Event* event){
        gameOverLabel->removeFromParent();
        _eventDispatcher->removeEventListenersForTarget(gameOverLabel);

        this->resetGame();
        return true;
    };
    _eventDispatcher->addEventListenerWithSceneGraphPriority(touchListener, gameOverLabel);
}

// **********************************************
// ✅ جدید: تابع بازگشت به منو
// **********************************************
void GameScene::onGoToMenu(Ref* sender)
{
    // بازگشت به صحنه‌ی منو
    auto scene = MenuScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
}

void GameScene::resetGame()
{
    // ۱. حذف تمام گره‌های بازی قبلی
    for (auto card : _allCards)
    {
        card->removeFromParent();
    }
    _allCards.clear();

    // حذف لیبل امتیاز قبلی و دکمه منو
    if (_scoreLabel) {
        _scoreLabel->removeFromParent();
        _scoreLabel = nullptr;
    }

    if (auto background = this->getChildByTag(99))
    {
        background->removeFromParent();
    }

    // ۲. بازنشانی متغیرهای وضعیت
    _firstCard = nullptr;
    _secondCard = nullptr;
    _isProcessing = false;
    _matchesFound = 0;
    _score = 0; // بازنشانی امتیاز

    // ۳. فراخوانی setupGame()
    setupGame();
}