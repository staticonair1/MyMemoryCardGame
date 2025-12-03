#ifndef __GRID_GAME_H__
#define __GRID_GAME_H__

#include "cocos2d.h"
#include "ui/CocosGUI.h"
#include <vector>

class GridGame : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createScene();
    virtual bool init();

    CREATE_FUNC(GridGame);

private:
    // --- Buttons Grid (5x8 = 40)
    cocos2d::ui::Button* _buttons[40] = { nullptr };

    // --- Game State Data
    std::vector<int> _correctlyColoredIndices;
    int _score = 0;
    int _correctGuesses = 0;
    int _totalYellows = 0;

    enum class GameState {
        READY_TO_START,
        SHOWING_PATTERN,
        WAITING_FOR_GUESS,
        GAME_OVER
    };
    GameState _state = GameState::READY_TO_START;

    // --- UI Elements
    cocos2d::Label* _scoreLabel = nullptr;
    cocos2d::Label* _promptLabel = nullptr;

    // --- Touch Listener (to avoid leaks)
    cocos2d::EventListenerTouchOneByOne* _touchListener = nullptr;

    // --- Logic Functions
    void setupRound();
    void onExit();
    void showYellowButtons();
    void hideYellowButtons(float dt);
    void resetButtons();
    bool isNeighboring(int currentIdx);

    // --- Callbacks
    bool onTouchBegan(cocos2d::Touch* touch, cocos2d::Event* event);
    void onGridButtonPress(cocos2d::Ref* sender, cocos2d::ui::Widget::TouchEventType type);
    void onMenuButtonPress(cocos2d::Ref* sender);
};

#endif // __GRID_GAME_H__
