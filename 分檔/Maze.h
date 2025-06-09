#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <queue> // For BFS
#include "Point.h" // 包含 Point 類別

// --- Visibility 列舉 (僅用於迷霧關卡) ---
enum Visibility {
    UNSEEN,      // 玩家從未到訪或見過的區域 (顯示為 '.')
    SEEN_BEFORE, // 玩家曾經看到過，但目前不在當前視野範圍內的區域 (顯示為 '-')
    VISIBLE      // 玩家當前視野範圍內的區域 (顯示實際內容)
};

// --- Maze 類別 ---
class Maze {
private:
    std::vector<std::vector<char>> mazeGrid; // 私有成員
    std::vector<std::vector<Visibility>> visibility; // 僅用於迷霧系統
    bool isFoggy; // 標記此迷宮是否使用迷霧
    bool isDynamic; // 標記此迷宮是否為動態迷宮

    int MAZE_WIDTH;
    int MAZE_HEIGHT;

    // 隨機數生成器
    std::mt19937 rng;

public:
    // 建構子 1：用於 int 網格 (關卡 1-4)
    Maze(const std::vector<std::vector<int>>& initialGrid);
    // 建構子 2：用於 string 向量 (關卡 5 - 迷霧系統)
    Maze(const std::vector<std::string>& initialRawMaze);
    // 建構子 3：用於動態迷宮 (關卡 6 - 重構系統)
    Maze(int width, int height, bool dynamic = true);

    // 初始生成隨機迷宮 (用於關卡 6 的 Maze 建構子)
    void generateInitialRandomMaze();

    // 重構牆壁 (用於關卡 6)，並確保可達性
    void reconstructWall(int playerX, int playerY, int exitX, int exitY);

    // 重新定位出口 (用於關卡 6)，並確保可達性
    Point relocateExit(int playerX, int playerY, Point currentExit);

    // 更新迷霧視野 (僅用於迷霧關卡)
    void updateVisibility(int playerX, int playerY, int radius = 3);

    // 繪製迷宮到控制台，考慮玩家、箱子、出口和可見性
    void display(const Point& mouse, const Point& exit, const Point* box = nullptr, const Point* target = nullptr) const;

    // 檢查指定座標是否可通行 (不會撞牆)
    bool isWalkable(int x, int y) const;

    // 檢查從起點到終點是否有路徑 (使用 BFS)
    bool hasPath(int startX, int startY, int endX, int endY) const;

    // 獲取迷宮單元格的內容
    char getCell(int x, int y) const;

    // 設置迷宮單元格的內容 (新增的公共方法)
    void setCell(int x, int y, char value);

    // 尋找特定字元在迷宮中的位置 (用於 string 網格的 P 和 E)
    Point findChar(char c) const;

    int getWidth() const;
    int getHeight() const;
};

#endif // MAZE_H