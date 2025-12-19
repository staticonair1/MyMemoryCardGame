
#include "DifficultySelectScene.h"
#include "SudokuGame.h"
#include "MenuScene.h"

USING_NS_CC;

Scene* DifficultySelectScene::createScene()
{
    return DifficultySelectScene::create();
}

bool DifficultySelectScene::init()
{
    if (!Scene::init()) {
        return false;
    }

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    // Background
    auto background = LayerColor::create(Color4B(30, 40, 50, 255));
    this->addChild(background);

    // Title
    auto title = Label::createWithTTF("Select Difficulty", "fonts/Marker Felt.ttf", 48);
    title->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height - 100);
    this->addChild(title, 1);

    // Difficulty buttons
    auto easyButton = MenuItemFont::create("Beginner", CC_CALLBACK_1(DifficultySelectScene::onDifficultySelected, this, 30));
    easyButton->setFontSize(36);

    auto mediumButton = MenuItemFont::create("Intermediate", CC_CALLBACK_1(DifficultySelectScene::onDifficultySelected, this, 45));
    mediumButton->setFontSize(36);

    auto hardButton = MenuItemFont::create("Advanced", CC_CALLBACK_1(DifficultySelectScene::onDifficultySelected, this, 60));
    hardButton->setFontSize(36);

    auto menu = Menu::create(easyButton, mediumButton, hardButton, nullptr);
    menu->alignItemsVerticallyWithPadding(30);
    menu->setPosition(origin.x + visibleSize.width / 2, origin.y + visibleSize.height / 2);
    this->addChild(menu, 1);

    // Back button
    auto backButton = MenuItemFont::create("Back to Menu", CC_CALLBACK_1(DifficultySelectScene::onGoToMenu, this));
    backButton->setFontSize(30);
    backButton->setPosition(origin.x + visibleSize.width / 2, origin.y + 80);
    
    auto backMenu = Menu::create(backButton, nullptr);
    backMenu->setPosition(Vec2::ZERO);
    this->addChild(backMenu, 1);

    return true;
}

void DifficultySelectScene::onDifficultySelected(Ref* sender, int difficulty)
{
    // Create the Sudoku scene and pass the difficulty (number of empty cells)
    auto scene = SudokuGame::createSceneWithDifficulty(difficulty);
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
}

void DifficultySelectScene::onGoToMenu(Ref* sender)
{
    auto scene = MenuScene::createScene();
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, scene));
}
