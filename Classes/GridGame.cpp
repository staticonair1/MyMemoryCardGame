#include "GridGame.h"
#include "GameSelectScene.h"
#include "ui/CocosGUI.h"
#include <random>
#include <algorithm>
#include <numeric>

USING_NS_CC;
using namespace ui;

// ثابت‌ها
const int ROWS = 5;
const int COLS = 8;
const int TOTAL_BUTTONS = ROWS * COLS;
static const float BUTTON_TEXTURE_SIZE = 1024.0f; // اندازه فرضی تکسچر

Scene* GridGame::createScene()
{
    return GridGame::create();
}

bool GridGame::init()
{
    if (!Scene::init()) return false;

    // Visible size و origin برای تطابق با رزولوشن‌های مختلف
    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto origin = Director::getInstance()->getVisibleOrigin();
    float screenW = visibleSize.width;
    float screenH = visibleSize.height;

    // مقداردهی اولیهٔ آرایه دکمه‌ها به nullptr (درصورتی که هدر تغییر نکرده باشد)
    for (int i = 0; i < TOTAL_BUTTONS; ++i) _buttons[i] = nullptr;

    // ---------------------------------------------------------
    // 1) محاسبه اندازهٔ دکمه‌ها
    // ---------------------------------------------------------
    float buttonSizeWidth = screenW / static_cast<float>(COLS);
    float buttonSizeHeight = screenH / static_cast<float>(ROWS);
    float buttonSize = std::min(buttonSizeWidth, buttonSizeHeight);

    // اگر خواستی spacing بین دکمه‌ها بدهی، اینجا می‌توانی تغییر دهی
    float spacing = 0.0f; // مقدار 0 یعنی چسبیده به هم
    float totalWidth = COLS * buttonSize + (COLS - 1) * spacing;
    float totalHeight = ROWS * buttonSize + (ROWS - 1) * spacing;

    // ---------------------------------------------------------
    // 2) دکمه منو (بالا-راست)
    // ---------------------------------------------------------
    auto menuButton = Button::create("res/blue_square.png", "res/blue_square.png");
    menuButton->setTitleText("Menu");
    menuButton->setTitleFontSize(34);
    menuButton->setAnchorPoint(Vec2(1, 1));

    Size desiredMenuSize(150.0f, 60.0f);
    float menuScale = desiredMenuSize.height / BUTTON_TEXTURE_SIZE;
    menuButton->setScale(menuScale);

    menuButton->setPosition(Vec2(origin.x + screenW - 20.0f, origin.y + screenH - 20.0f));
    menuButton->addClickEventListener(CC_CALLBACK_1(GridGame::onMenuButtonPress, this));
    this->addChild(menuButton, 20);

    // ---------------------------------------------------------
    // 3) امتیاز (بالا-چپ)
    // ---------------------------------------------------------
    _scoreLabel = Label::createWithSystemFont("Score: 0", "Arial", 40);
    _scoreLabel->setAnchorPoint(Vec2(0, 1));
    _scoreLabel->setPosition(Vec2(origin.x + 20.0f, origin.y + screenH - 20.0f));
    this->addChild(_scoreLabel, 20);

    // ---------------------------------------------------------
    // 4) متن راهنما
    // ---------------------------------------------------------
    _promptLabel = Label::createWithSystemFont("Ready to play? Touch to start.", "Arial", 48);
    _promptLabel->setPosition(Vec2(origin.x + screenW * 0.5f, origin.y + screenH * 0.5f));
    _promptLabel->setHorizontalAlignment(TextHAlignment::CENTER);
    this->addChild(_promptLabel, 20);

    // ---------------------------------------------------------
    // 5) ایجاد شبکهٔ دکمه‌ها (کاملاً وسط‌چین)
    // ---------------------------------------------------------
    float marginX = origin.x + (screenW - totalWidth) * 0.5f;
    float marginY = origin.y + (screenH - totalHeight) * 0.5f;

    float firstX = marginX + buttonSize * 0.5f;
    float firstY = marginY + buttonSize * 0.5f;

    for (int i = 0; i < TOTAL_BUTTONS; ++i)
    {
        int row = i / COLS;
        int col = i % COLS;

        auto btn = Button::create("res/blue_square.png", "res/blue_square.png");
        btn->setAnchorPoint(Vec2(0.5f, 0.5f));

        float scaleForThis = buttonSize / BUTTON_TEXTURE_SIZE;
        btn->setScale(scaleForThis);

        float x = firstX + col * (buttonSize + spacing);
        float y = firstY + row * (buttonSize + spacing);

        btn->setPosition(Vec2(x, y));
        btn->setTag(i);
        btn->addTouchEventListener(CC_CALLBACK_2(GridGame::onGridButtonPress, this));
        this->addChild(btn, 5);

        _buttons[i] = btn;
    }

    // ثبت لیسنر لمس صحنه (برای شروع بازی) — این لیسنر در h باید به عنوان عضو کلاس اضافه شده باشد
    // مثال: EventListenerTouchOneByOne* _touchListener = nullptr; // در GridGame.h
    _touchListener = EventListenerTouchOneByOne::create();
    _touchListener->onTouchBegan = CC_CALLBACK_2(GridGame::onTouchBegan, this);
    _touchListener->setSwallowTouches(false);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(_touchListener, this);

    // مقادیر اولیه
    _state = GameState::READY_TO_START;
    _score = 0;
    _correctGuesses = 0;
    _totalYellows = 0;

    return true;
}

// ---------------------------------------------------------
// onExit: پاک‌سازی لیسنرها و schedule ها
// ---------------------------------------------------------
void GridGame::onExit()
{
    Scene::onExit();

    if (_touchListener)
    {
        _eventDispatcher->removeEventListener(_touchListener);
        _touchListener = nullptr;
    }

    _eventDispatcher->removeEventListenersForTarget(this);
    this->unscheduleAllCallbacks();
}

// ---------------------------------------------------------
// لمس صفحه: شروع بازی یا ریست
// ---------------------------------------------------------
bool GridGame::onTouchBegan(Touch* touch, Event* event)
{
    if (_state == GameState::READY_TO_START)
    {
        _promptLabel->setVisible(false);
        _state = GameState::SHOWING_PATTERN;

        setupRound();
        showYellowButtons();

        // زمان‌دار برای مخفی کردن
        this->scheduleOnce(CC_SCHEDULE_SELECTOR(GridGame::hideYellowButtons), 2.0f);
        return true;
    }
    else if (_state == GameState::GAME_OVER)
    {
        // لغو schedule های مرتبط
        this->unschedule(CC_SCHEDULE_SELECTOR(GridGame::hideYellowButtons));

        resetButtons();
        _score = 0;
        _correctGuesses = 0;

        _promptLabel->setString("Ready to play? Touch to start.");
        _promptLabel->setVisible(true);
        _scoreLabel->setString(StringUtils::format("Score: %d", _score));

        _state = GameState::READY_TO_START;
        return true;
    }

    return false;
}

// ---------------------------------------------------------
// تشخیص همسایگی (برای جلوگیری از چسبیدن زردها)
// ---------------------------------------------------------
bool GridGame::isNeighboring(int currentIdx)
{
    if (_correctlyColoredIndices.empty()) return false;

    int r = currentIdx / COLS;
    int c = currentIdx % COLS;

    for (int idx : _correctlyColoredIndices)
    {
        int rr = idx / COLS;
        int cc = idx % COLS;

        if (std::abs(r - rr) <= 1 && std::abs(c - cc) <= 1)
        {
            if (r != rr || c != cc) return true;
        }
    }
    return false;
}

// ---------------------------------------------------------
// آماده سازی مرحله — نسخه امن بدون infinite-loop
// ---------------------------------------------------------
void GridGame::setupRound()
{
    _correctlyColoredIndices.clear();
    _correctGuesses = 0;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_int_distribution<> dist(5, 10);
    _totalYellows = dist(gen);

    std::vector<int> indices(TOTAL_BUTTONS);
    std::iota(indices.begin(), indices.end(), 0);
    std::shuffle(indices.begin(), indices.end(), gen);

    for (int idx : indices)
    {
        if ((int)_correctlyColoredIndices.size() >= _totalYellows) break;
        if (!isNeighboring(idx))
            _correctlyColoredIndices.push_back(idx);
    }

    if ((int)_correctlyColoredIndices.size() < _totalYellows)
    {
        CCLOG("Warning: couldn't place all yellows without adjacency; reducing totalYellows from %d to %zu",
              _totalYellows, _correctlyColoredIndices.size());
        _totalYellows = static_cast<int>(_correctlyColoredIndices.size());
    }

    _scoreLabel->setString(StringUtils::format("Score: %d", _score));
}

// ---------------------------------------------------------
// نمایش زرد
// ---------------------------------------------------------
void GridGame::showYellowButtons()
{
    for (int idx : _correctlyColoredIndices)
    {
        if (_buttons[idx])
        {
            _buttons[idx]->loadTextureNormal("res/yellow_square.png");
            _buttons[idx]->loadTexturePressed("res/yellow_square.png");
            // غیرفعال موقت تا لمس نشود در نمایش الگو
            _buttons[idx]->setTouchEnabled(false);
        }
    }
}

// ---------------------------------------------------------
// پنهان کردن الگو و فعال کردن دکمه‌ها برای حدس
// ---------------------------------------------------------
void GridGame::hideYellowButtons(float dt)
{
    // ست همه دکمه‌ها به حالت آبی و فعال
    resetButtons();
    _state = GameState::WAITING_FOR_GUESS;

    _promptLabel->setVisible(false);
}

// ---------------------------------------------------------
// ریست دکمه‌ها
// ---------------------------------------------------------
void GridGame::resetButtons()
{
    for (int i = 0; i < TOTAL_BUTTONS; ++i)
    {
        if (_buttons[i])
        {
            _buttons[i]->loadTextureNormal("res/blue_square.png");
            _buttons[i]->loadTexturePressed("res/blue_square.png");
            _buttons[i]->setTouchEnabled(true);
        }
    }
}

// ---------------------------------------------------------
// کلیک روی دکمه‌های شبکه
// ---------------------------------------------------------
void GridGame::onGridButtonPress(Ref* sender, Widget::TouchEventType type)
{
    if (_state != GameState::WAITING_FOR_GUESS) return;
    if (type != Widget::TouchEventType::ENDED) return;

    auto button = dynamic_cast<Button*>(sender);
    if (!button) return;
    if (!button->isTouchEnabled()) return;

    int idx = button->getTag();

    bool correct = (std::find(_correctlyColoredIndices.begin(), _correctlyColoredIndices.end(), idx) != _correctlyColoredIndices.end());

    if (correct)
    {
        button->loadTextureNormal("res/correct_guess.png");
        button->loadTexturePressed("res/correct_guess.png");
        _correctGuesses++;
        _score++;
    }
    else
    {
        button->loadTextureNormal("res/wrong_guess.png");
        button->loadTexturePressed("res/wrong_guess.png");

        _promptLabel->setVisible(false);
        _state = GameState::GAME_OVER;
    }

    button->setTouchEnabled(false);
    _scoreLabel->setString(StringUtils::format("Score: %d", _score));

    if (_state == GameState::GAME_OVER)
    {
        for (int i = 0; i < TOTAL_BUTTONS; ++i)
            if (_buttons[i]) _buttons[i]->setTouchEnabled(false);
        return;
    }

    if (_correctGuesses == _totalYellows)
    {
        _promptLabel->setVisible(false);
        _state = GameState::GAME_OVER;

        for (int i = 0; i < TOTAL_BUTTONS; ++i)
            if (_buttons[i]) _buttons[i]->setTouchEnabled(false);
    }
}

// ---------------------------------------------------------
// دکمهٔ منو -> بازگشت به انتخاب بازی
// ---------------------------------------------------------
void GridGame::onMenuButtonPress(Ref* pSender)
{
    // پاک‌سازی دقیق
    if (_touchListener)
    {
        _eventDispatcher->removeEventListener(_touchListener);
        _touchListener = nullptr;
    }
    _eventDispatcher->removeEventListenersForTarget(this);
    this->unscheduleAllCallbacks();

    auto scene = GameSelectScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5f, scene));
}
