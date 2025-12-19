
#ifndef __SUDOKU_GAME_SCENE_H__
#define __SUDOKU_GAME_SCENE_H__

#include "cocos2d.h"
#include "NumberPad.h"
#include <vector>

class SudokuGame : public cocos2d::Scene
{
public:
    static cocos2d::Scene* createSceneWithDifficulty(int emptyCells);
    static SudokuGame* createWithDifficulty(int emptyCells);
    virtual bool initWithDifficulty(int emptyCells);
    virtual ~SudokuGame() = default;

private:
    int _selectedRow = -1;
    int _selectedCol = -1;
    // Menu Actions
    void showNewGameConfirmation(cocos2d::Ref* pSender);
    void onCheckSolution(cocos2d::Ref* pSender);
    void onGoToMenu(cocos2d::Ref* pSender);

    // Sudoku Logic
    void generatePuzzle();
    bool solveSudoku(std::vector<std::vector<int>>& board, int row, int col);
    bool isSafe(const std::vector<std::vector<int>>& board, int row, int col, int num);
    void resetBoard();
    int _emptyCells = 0;

    // Touch & Input
    bool onGridTouch(cocos2d::Touch* touch, cocos2d::Event* event);
    void onNumberPadPressed(int number);
    void selectCell(cocos2d::Label* cellLabel, int row=-1, int col=-1);
    void deselectCell();

    // UI & Grid
    void createGrid();
    void createMenus();

    cocos2d::DrawNode* _gridNode = nullptr;
    cocos2d::DrawNode* _selectionNode = nullptr;
    NumberPad* _numberPad = nullptr;
    cocos2d::Label* _selectedCell = nullptr;

    // Board Data
    std::vector<std::vector<int>> _solutionBoard;
    std::vector<std::vector<bool>> _isGivenNumber;
    std::vector<std::vector<cocos2d::Label*>> _cellLabels;

    // Layout
    cocos2d::Vec2 _gridOrigin;
    float _gridSize = 0;
    float _cellSize = 0;
};

#endif
