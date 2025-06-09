#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include <iostream> // For std::cout, std::cin
#include <cctype>   // For tolower
#include <utility>  // For std::pair (although Point is used)
#include "Maze.h"
#include "Mouse.h"
#include "Point.h" // 包含 Point 類別，確保所有遊戲類別都能使用

// --- Game 抽象基類 ---
class Game {
protected:
    Maze maze;
    Mouse mouse;
    Point end; // 出口位置

    const std::vector<Point> Directions = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} }; // 上, 下, 左, 右
    const std::vector<char> DirKeys = { 'w', 's', 'a', 'd' };
    const std::vector<std::string> DirNames = { "上", "下", "左", "右" };

public:
    // 建構子 1：用於 int 網格的迷宮 (關卡 1-4)
    Game(const std::vector<std::vector<int>>& mazeData, Point start, Point end);
    // 建構子 2：用於 string 網格的迷宮 (關卡 5)
    Game(const std::vector<std::string>& rawMazeData, Point start, Point end);
    // 建構子 3：用於動態迷宮 (關卡 6)
    Game(int width, int height);

    virtual ~Game() = default;

    virtual int play() = 0;
};

// --- NormalGame 類別 (關卡 1-3) ---
class NormalGame : public Game {
public:
    NormalGame(const std::vector<std::vector<int>>& mazeData, Point start, Point end);
    int play() override;
};

// --- BoxGame 類別 (關卡 4) ---
class BoxGame : public Game {
private:
    Point box;
    Point boxTarget;
    Point startMouse;
    Point startBox;

public:
    BoxGame(const std::vector<std::vector<int>>& mazeData, Point start, Point end, Point boxStart, Point target);
    int play() override;
};

// --- FogGame 類別 (關卡 5) ---
class FogGame : public Game {
public:
    FogGame(const std::vector<std::string>& rawMazeData, Point start, Point end);
    int play() override;
};

// --- DynamicGame 類別 (關卡 6 - 重構系統) ---
class DynamicGame : public Game {
private:
    std::mt19937 dynamicRng; // 專門用於動態迷宮的隨機數生成器
    Point currentExit; // 儲存當前出口位置
    Point startPosition; // 儲存玩家起始位置，用於在重構後確保路徑

public:
    DynamicGame(int width, int height);
    int play() override;
};

#endif // GAME_H