#include <iostream>
#include <vector>
#include <string>
#include "Point.h"
#include "Maze.h"
#include "Mouse.h"
#include "Game.h" // 包含所有遊戲類別的抽象基類
#include "Utility.h" // 包含清屏宏和勝利畫面

// 各關卡迷宮資料
std::vector<std::vector<int>> mazeMap1 =
{
    {1,1,1,1,1},
    {1,0,0,0,1},
    {1,1,1,0,1},
    {1,0,0,0,1},
    {1,1,1,1,1}
};
std::vector<std::vector<int>> mazeMap2 =
{
    {1,1,1,1,1,1,1},
    {1,0,1,0,1,0,1},
    {1,0,1,0,1,0,1},
    {1,0,1,0,0,0,1},
    {1,0,0,0,1,0,1},
    {1,1,1,0,1,0,1},
    {1,1,1,1,1,1,1}
};
std::vector<std::vector<int>> mazeMap3 =
{
    {1,1,1,1,1,1,1,1,1,1},
    {1,0,1,0,0,0,0,1,0,1},
    {1,0,1,0,1,1,0,1,0,1},
    {1,0,0,0,1,0,0,0,0,1},
    {1,1,1,0,1,0,1,1,0,1},
    {1,0,0,0,1,0,0,1,0,1},
    {1,0,1,0,1,1,0,1,0,1},
    {1,0,1,0,1,1,0,0,1,1},
    {1,0,0,0,0,0,0,0,0,1},
    {1,1,1,1,1,1,1,1,1,1}
};

// 關卡 4 使用的地圖 (與 mazeMap3 相同)
std::vector<std::vector<int>> mazeMap4_box = mazeMap3;

// 關卡 5 (迷霧系統) 使用的原始字串迷宮
std::vector<std::string> rawMaze_fog = {
        "####################",
        "#P     #       #   #",
        "# ### #### ##  # # #",
        "#     #     #   ##E#",
        "##### # ###### # # #",
        "#       #    # #   #",
        "# ####### ## # ### #",
        "#          #       #",
        "########## ####### #",
        "####################"

};


int main() {
    int totalMoves = 0;

    // 關卡 1
    system(CLEAR_SCREEN);
    std::cout << "\n--- 闖關 1 (簡單迷宮) ---\n";
    NormalGame game1(mazeMap1, Point(1, 1), Point(3, 3));
    totalMoves += game1.play();

    // 關卡 2
    system(CLEAR_SCREEN);
    std::cout << "\n--- 闖關 2 (中等迷宮) ---\n";
    NormalGame game2(mazeMap2, Point(1, 1), Point(5, 5));
    totalMoves += game2.play();

    // 關卡 3
    system(CLEAR_SCREEN);
    std::cout << "\n--- 闖關 3 (複雜迷宮) ---\n";
    NormalGame game3(mazeMap3, Point(1, 1), Point(8, 8));
    totalMoves += game3.play();

    // 關卡 4 (推箱子迷宮)
    system(CLEAR_SCREEN);
    std::cout << "\n--- 闖關 4 (推箱子迷宮) ---\n";
    BoxGame game4(mazeMap4_box, Point(1, 1), Point(8, 8), Point(3, 2), Point(6, 8));
    totalMoves += game4.play();

    // 關卡 5 (迷霧迷宮)
    system(CLEAR_SCREEN);
    std::cout << "\n--- 闖關 5 (迷霧迷宮) ---\n";
    FogGame game5(rawMaze_fog, Point(1, 1), Point(18, 3));
    totalMoves += game5.play();

    // 關卡 6 (動態重構迷宮)
    system(CLEAR_SCREEN);
    std::cout << "\n--- 闖關 6 (動態重構迷宮) ---\n";
    DynamicGame game6(30, 15); // 不再需要傳入 start 和 end，它們會在 DynamicGame 內部處理
    totalMoves += game6.play();


    // 完成所有關卡後的總結
    system(CLEAR_SCREEN);
    printVictoryScreen();
    std::cout << "恭喜您！完成所有關卡！\n";
    std::cout << "總移動次數: " << totalMoves << " 次。\n";

    std::cout << "按下 Enter 鍵結束遊戲...";
    std::cin.ignore();
    std::cin.get();
    return 0;
}