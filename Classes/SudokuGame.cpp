
#include "SudokuGame.h"
#include "DifficultySelectScene.h"
#include "ui/CocosGUI.h"
#include <random>
#include <algorithm>

USING_NS_CC;

Scene* SudokuGame::createSceneWithDifficulty(int emptyCells)
{
    auto scene = Scene::create();
    auto layer = SudokuGame::createWithDifficulty(emptyCells);
    scene->addChild(layer);
    return scene;
}

SudokuGame* SudokuGame::createWithDifficulty(int emptyCells)
{
    SudokuGame *pRet = new(std::nothrow) SudokuGame();
    if (pRet && pRet->initWithDifficulty(emptyCells))
    {
        pRet->autorelease();
        return pRet;
    }
    else
    {
        delete pRet;
        pRet = nullptr;
        return nullptr;
    }
}

bool SudokuGame::initWithDifficulty(int emptyCells)
{
    if (!Scene::init()) {
        return false;
    }

    _emptyCells = emptyCells;
    
    auto background = LayerColor::create(Color4B(50, 50, 50, 255));
    this->addChild(background);

    auto visibleSize = Director::getInstance()->getVisibleSize();
    auto title = Label::createWithTTF("Sudoku", "fonts/Marker Felt.ttf", 48);
    // Move title up to be clearly above the grid
   // title->setPosition(Vec2(visibleSize.width / 2, visibleSize.height - 60));
    this->addChild(title, 1);

    createGrid();
    createMenus();
    generatePuzzle();

    auto listener = EventListenerTouchOneByOne::create();
    listener->setSwallowTouches(false);
    listener->onTouchBegan = CC_CALLBACK_2(SudokuGame::onGridTouch, this);
    _eventDispatcher->addEventListenerWithSceneGraphPriority(listener, this);

    return true;
}

void SudokuGame::createGrid()
{auto title = Label::createWithTTF("Sudoku", "fonts/Marker Felt.ttf", 52);
    title->setPosition(
            _gridOrigin.x + _gridSize / 2,
            _gridOrigin.y + _gridSize + 40   // فاصله از بالای جدول
    );
    this->addChild(title, 2);

    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();

    float sideMargin = 150;
    _gridSize = std::min(visibleSize.width - 2 * sideMargin, visibleSize.height - 150);
    _cellSize = _gridSize / 9.0f;
    _gridOrigin = Vec2(origin.x + (visibleSize.width - _gridSize) / 2, origin.y + (visibleSize.height - _gridSize) / 2);

    _gridNode = DrawNode::create();
    this->addChild(_gridNode, 1);

    for (int i = 0; i <= 9; ++i) {
        float thickness = (i % 3 == 0) ? 3.0f : 1.0f;
        _gridNode->drawSegment(Vec2(_gridOrigin.x, _gridOrigin.y + i * _cellSize), Vec2(_gridOrigin.x + _gridSize, _gridOrigin.y + i * _cellSize), thickness, Color4F::WHITE);
        _gridNode->drawSegment(Vec2(_gridOrigin.x + i * _cellSize, _gridOrigin.y), Vec2(_gridOrigin.x + i * _cellSize, _gridOrigin.y + _gridSize), thickness, Color4F::WHITE);
    }

    _cellLabels.resize(9, std::vector<Label*>(9));
    _isGivenNumber.resize(9, std::vector<bool>(9, false));
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            _cellLabels[i][j] = Label::createWithTTF("", "fonts/arial.ttf", _cellSize * 0.7f);
            _cellLabels[i][j]->setPosition(_gridOrigin.x + j * _cellSize + _cellSize / 2, _gridOrigin.y + (8 - i) * _cellSize + _cellSize / 2);
            this->addChild(_cellLabels[i][j], 2);
        }
    }
    
    _selectionNode = DrawNode::create();
    this->addChild(_selectionNode, 3);
}

void SudokuGame::createMenus()
{
    auto visibleSize = Director::getInstance()->getVisibleSize();
    Vec2 origin = Director::getInstance()->getVisibleOrigin();
    
    auto newGameBtn = MenuItemFont::create("New Game", CC_CALLBACK_1(SudokuGame::showNewGameConfirmation, this));
    auto checkBtn = MenuItemFont::create("Check", CC_CALLBACK_1(SudokuGame::onCheckSolution, this));
    auto backBtn = MenuItemFont::create("Back", CC_CALLBACK_1(SudokuGame::onGoToMenu, this));

    auto rightMenu = Menu::create(newGameBtn, checkBtn, backBtn, nullptr);
    rightMenu->alignItemsVerticallyWithPadding(40);
    rightMenu->setPosition(origin.x + visibleSize.width - 80, origin.y + visibleSize.height / 2);
    this->addChild(rightMenu, 2);

    _numberPad = NumberPad::create();
    _numberPad->setScale(1.15f);
    _numberPad->setAnchorPoint(Vec2(0.5, 0.5));
    _numberPad->setAnchorPoint(Vec2(0.5f, 0.5f));
    _numberPad->setPosition(
            _gridOrigin.x - 320   ,                 // ⬅️ چپ‌تر
            _gridOrigin.y + _gridSize * 0.75f    // ⬆️ بالاتر
    );
    this->addChild(_numberPad, 2);
    _numberPad->onButtonPressed = CC_CALLBACK_1(SudokuGame::onNumberPadPressed, this);
    _numberPad->setVisible(false);
}

void SudokuGame::generatePuzzle()
{
    resetBoard();
    solveSudoku(_solutionBoard, 0, 0);

    std::vector<int> indices(81);
    std::iota(indices.begin(), indices.end(), 0);
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(indices.begin(), indices.end(), g);

    auto playableBoard = _solutionBoard;

    for (int i = 0; i < _emptyCells; ++i) {
        int row = indices[i] / 9;
        int col = indices[i] % 9;
        playableBoard[row][col] = 0;
    }

    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
             _isGivenNumber[i][j] = (playableBoard[i][j] != 0);
            _cellLabels[i][j]->setString(playableBoard[i][j] == 0 ? "" : std::to_string(playableBoard[i][j]));
            _cellLabels[i][j]->setTextColor(_isGivenNumber[i][j] ? Color4B::YELLOW : Color4B::WHITE);
        }
    }
}

void SudokuGame::resetBoard() {
    _solutionBoard.assign(9, std::vector<int>(9, 0));
    _isGivenNumber.assign(9, std::vector<bool>(9, false));
    if (_selectedCell) {
        deselectCell();
    }
}

bool SudokuGame::solveSudoku(std::vector<std::vector<int>>& board, int row, int col)
{
    if (row == 8 && col == 9) return true;
    if (col == 9) {
        row++;
        col = 0;
    }
    if (board[row][col] != 0) return solveSudoku(board, row, col + 1);

    std::vector<int> nums = {1, 2, 3, 4, 5, 6, 7, 8, 9};
    std::random_device rd;
    std::mt19937 g(rd());
    std::shuffle(nums.begin(), nums.end(), g);

    for (int num : nums) {
        if (isSafe(board, row, col, num)) {
            board[row][col] = num;
            if (solveSudoku(board, row, col + 1)) return true;
            board[row][col] = 0;
        }
    }
    return false;
}

bool SudokuGame::isSafe(const std::vector<std::vector<int>>& board, int row, int col, int num)
{
    for (int x = 0; x < 9; ++x) if (board[row][x] == num || board[x][col] == num) return false;
    int startRow = row - row % 3, startCol = col - col % 3;
    for (int i = 0; i < 3; i++) for (int j = 0; j < 3; j++) if (board[i + startRow][j + startCol] == num) return false;
    return true;
}

void SudokuGame::showNewGameConfirmation(Ref* pSender) 
{
    if (this->getChildByName("confirmPopup")) {
        return;
    }

    deselectCell();
    auto overlay = LayerColor::create(Color4B(0, 0, 0, 150));
    this->addChild(overlay, 4, "confirmPopup");

    auto box = ui::Scale9Sprite::create();
    box->setContentSize(Size(400, 200));
    box->setPosition(Director::getInstance()->getVisibleSize() / 2);
    overlay->addChild(box);

    auto label = Label::createWithTTF("Are you sure?", "fonts/Marker Felt.ttf", 32);
    label->setPosition(box->getContentSize() / 2 + Size(0, 50));
    box->addChild(label);

    auto yesButton = MenuItemFont::create("Yes", [this, overlay](Ref* sender){
        this->removeChild(overlay);
        generatePuzzle();
    });
    auto noButton = MenuItemFont::create("No", [this, overlay](Ref* sender){
        this->removeChild(overlay);
    });

    auto menu = Menu::create(yesButton, noButton, nullptr);
    menu->alignItemsHorizontallyWithPadding(50);
    menu->setPosition(box->getContentSize() / 2 - Size(0, 50));
    box->addChild(menu);
}

bool SudokuGame::onGridTouch(Touch* touch, Event*)
{
    Vec2 pos = touch->getLocation();

    Rect gridRect(_gridOrigin.x, _gridOrigin.y, _gridSize, _gridSize);

    if (!gridRect.containsPoint(pos))
    {
        return false; // ⬅️ اجازه بده Menu تاچ را بگیرد
    }

    float localX = pos.x - _gridOrigin.x;
    float localY = pos.y - _gridOrigin.y;

    int col = static_cast<int>(localX / _cellSize);
    int row = 8 - static_cast<int>(localY / _cellSize);

    if (_isGivenNumber[row][col])
        return true;

    selectCell(_cellLabels[row][col], row, col);
    return true; // ⬅️ فقط Grid تاچ را می‌خورد
}



void SudokuGame::selectCell(Label* cellLabel, int row, int col)
{
    if (_isGivenNumber[row][col]) return;

    _selectedCell = cellLabel;
    _selectedRow = row;
    _selectedCol = col;

    _selectionNode->clear();
    Vec2 cellOrigin(
            _gridOrigin.x + col * _cellSize,
            _gridOrigin.y + (8 - row) * _cellSize
    );

    _selectionNode->drawRect(
            cellOrigin,
            cellOrigin + Vec2(_cellSize, _cellSize),
            Color4F(0.2f, 0.5f, 1.0f, 0.6f)
    );

    _numberPad->setVisible(true);
}

void SudokuGame::deselectCell()
{
    _selectedCell = nullptr;
    _selectedRow = -1;
    _selectedCol = -1;
    _selectionNode->clear();
    _numberPad->setVisible(false);
}


void SudokuGame::onNumberPadPressed(int number)
{
    if (_selectedRow == -1 || _selectedCol == -1) return;
    if (_isGivenNumber[_selectedRow][_selectedCol]) return;

    if (number == 0)
    {
        _cellLabels[_selectedRow][_selectedCol]->setString("");
    }
    else
    {
        _cellLabels[_selectedRow][_selectedCol]->setString(std::to_string(number));
    }
}



void SudokuGame::onCheckSolution(Ref* pSender)
{
    deselectCell();
    for (int i = 0; i < 9; ++i) {
        for (int j = 0; j < 9; ++j) {
            if (!_isGivenNumber[i][j]) {
                std::string userValStr = _cellLabels[i][j]->getString();
                int userVal = userValStr.empty() ? 0 : std::stoi(userValStr);
                
                if (userVal != 0) { 
                    Color4F color = (userVal == _solutionBoard[i][j]) ? Color4F::GREEN : Color4F::RED;
                    auto flash = LayerColor::create(Color4B(color), _cellSize, _cellSize);
                    flash->setPosition(_gridOrigin.x + j * _cellSize, _gridOrigin.y + (8 - i) * _cellSize);
                    this->addChild(flash, 4);
                    flash->runAction(Sequence::create(FadeOut::create(0.7f), RemoveSelf::create(), nullptr));
                }
            }
        }
    }
}

void SudokuGame::onGoToMenu(Ref* pSender)
{
    Director::getInstance()->replaceScene(TransitionFade::create(0.5, DifficultySelectScene::createScene()));
}
