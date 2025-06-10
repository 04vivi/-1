#include "Game.h"
#include "Utility.h" // 包含清屏宏
#include <chrono>
#include <thread>



// Game 抽象基類的建構子 1
Game::Game(const std::vector<std::vector<int>>& mazeData, Point start, Point end)
    : maze(mazeData), mouse(start), end(end) {
}

// Game 抽象基類的建構子 2
Game::Game(const std::vector<std::string>& rawMazeData, Point start, Point end)
    : maze(rawMazeData), mouse(start), end(end) {
}

// Game 抽象基類的建構子 3
Game::Game(int width, int height)
    : maze(width, height), mouse({ 1,1 }), end({ width - 2, height - 2 }) {
    // 實際的起始點和終點會在 DynamicGame 內部設置
}

// --- NormalGame 實作 ---
NormalGame::NormalGame(const std::vector<std::vector<int>>& mazeData, Point start, Point end)
    : Game(mazeData, start, end) {
}

int NormalGame::play() {
    std::cout << "\n--- 普通迷宮關卡 ---\n";
    std::cout << "使用 w/a/s/d 控制老鼠(@)走出迷宮(+)。\n";
    Enter();

    while (true) {
        system(CLEAR_SCREEN);
        Point curr = mouse.getPosition();
        maze.display(curr, end); // 無迷霧顯示

        if (curr == end) {
            std::cout << "\n成功走出迷宮！\n";
            std::cout << "總移動次數: " << mouse.getMoveCount() << "\n";
            return mouse.getMoveCount();
        }

        std::cout << "\n目前位置: (" << curr.x << ", " << curr.y << ")，可走方向: ";
        bool hasValidMove = false;
        for (int i = 0; i < 4; ++i) {
            Point next = { curr.x + Directions[i].x, curr.y + Directions[i].y };
            if (maze.isWalkable(next.x, next.y)) {
                std::cout << DirNames[i] << "(" << DirKeys[i] << ") ";
                hasValidMove = true;
            }
        }
        if (!hasValidMove) std::cout << "無（死路）";
        std::cout << "\n請輸入方向：";
        char input;
        std::cin >> input;
        input = tolower(input);

        int DirIndex = -1;
        for (int i = 0; i < 4; ++i) {
            if (input == DirKeys[i]) {
                DirIndex = i;
                break;
            }
        }

        if (DirIndex == -1) {
            std::cout << "無效輸入，請輸入 w/s/a/d。\n";
            std::this_thread::sleep_for(std::chrono::seconds(3));  // 停 3 秒
            continue;
        }

        Point next = { curr.x + Directions[DirIndex].x, curr.y + Directions[DirIndex].y };
        if (!maze.isWalkable(next.x, next.y)) {
            std::cout << "那邊是牆壁，不能走！\n";
            waitForEnter();
            continue;
        }

        mouse.move(next);
    }
}

// --- BoxGame 實作 ---
BoxGame::BoxGame(const std::vector<std::vector<int>>& mazeData, Point start, Point end, Point boxStart, Point target)
    : Game(mazeData, start, end), box(boxStart), boxTarget(target), startMouse(start), startBox(boxStart) {
}

int BoxGame::play() {
    std::cout << "\n--- 推箱子迷宮關卡 ---\n";
    std::cout << "你必須把箱子(2)推到 X 才能走出迷宮(+)！\n";
    std::cout << "輸入 r 可重置位置。\n";
    Enter();


    while (true) {
        system(CLEAR_SCREEN);
        Point curr = mouse.getPosition();
        maze.display(curr, end, &box, &boxTarget); // 無迷霧顯示

        if (curr == end && box == boxTarget) {
            std::cout << "\n恭喜！你成功將箱子推到目標並走出迷宮！\n";
            std::cout << "總移動次數: " << mouse.getMoveCount() << "\n";
            return mouse.getMoveCount();
        }

        std::cout << "\n目前位置: (" << curr.x << ", " << curr.y << ")，可走方向: ";
        bool hasValidMove = false;
        for (int i = 0; i < 4; ++i) {
            Point nextPlayerPos = { curr.x + Directions[i].x, curr.y + Directions[i].y };
            if (maze.isWalkable(nextPlayerPos.x, nextPlayerPos.y)) {
                if (nextPlayerPos == box) {
                    Point nextBoxPos = { box.x + Directions[i].x, box.y + Directions[i].y };
                    // 檢查箱子推動後的位置是否可通行，且不與玩家當前位置重疊（防止箱子推到玩家身上）
                    if (maze.isWalkable(nextBoxPos.x, nextBoxPos.y) && nextBoxPos != curr) {
                        std::cout << DirNames[i] << "(" << DirKeys[i] << ", 推箱) ";
                        hasValidMove = true;
                    }
                }
                else {
                    std::cout << DirNames[i] << "(" << DirKeys[i] << ") ";
                    hasValidMove = true;
                }
            }
        }
        std::cout << "重置(r) ";
        if (!hasValidMove) std::cout << "無有效移動";
        std::cout << "\n請輸入方向：";
        char input;
        std::cin >> input;
        input = tolower(input);

        if (input == 'r') {
            box = startBox;
            mouse = Mouse(startMouse);
            std::cout << "已重置位置！\n";
            continue;
        }

        int DirIndex = -1;
        for (int i = 0; i < 4; ++i) {
            if (input == DirKeys[i]) {
                DirIndex = i;
                break;
            }
        }

        if (DirIndex == -1) {
            std::cout << "無效輸入，請輸入 w/s/a/d 或 r。\n";
            continue;
        }

        Point dir = Directions[DirIndex];
        Point nextPlayerPos = { curr.x + dir.x, curr.y + dir.y };

        if (nextPlayerPos == box) {
            Point nextBoxPos = { box.x + dir.x, box.y + dir.y };
            if (maze.isWalkable(nextBoxPos.x, nextBoxPos.y) && nextBoxPos != curr) {
                box = nextBoxPos;
                mouse.move(nextPlayerPos);
            }
            else {
                std::cout << "箱子無法推動那個方向！\n";
            }
        }
        else if (maze.isWalkable(nextPlayerPos.x, nextPlayerPos.y)) {
            mouse.move(nextPlayerPos);
        }
        else {
            std::cout << "那邊是牆壁，不能走！\n";
            waitForEnter();
        }
    }
}

// --- FogGame 實作 ---
FogGame::FogGame(const std::vector<std::string>& rawMazeData, Point start, Point end)
    : Game(rawMazeData, start, end) {
    // 在建構子中，Game 的建構子會自動呼叫 maze.updateVisibility 進行初始化
}

int FogGame::play() {
    std::cout << "\n--- 迷霧迷宮關卡 ---\n";
    std::cout << "部分區域被迷霧籠罩，只有探索才能揭開全貌！\n";
    std::cout << "使用 w/a/s/d 控制老鼠(@)走出迷宮(+)。\n";
    Enter();

    while (true) {
        system(CLEAR_SCREEN);
        Point curr = mouse.getPosition();
        maze.updateVisibility(curr.x, curr.y); // 更新迷霧視野
        maze.display(curr, end); // 帶迷霧顯示

        if (curr == end) {
            std::cout << "\n成功走出迷宮！\n";
            std::cout << "總移動次數: " << mouse.getMoveCount() << "\n";
            return mouse.getMoveCount();
        }

        std::cout << "\n目前位置: (" << curr.x << ", " << curr.y << ")，可走方向: ";
        bool hasValidMove = false;
        for (int i = 0; i < 4; ++i) {
            Point next = { curr.x + Directions[i].x, curr.y + Directions[i].y };
            if (maze.isWalkable(next.x, next.y)) {
                std::cout << DirNames[i] << "(" << DirKeys[i] << ") ";
                hasValidMove = true;
            }
        }
        if (!hasValidMove) std::cout << "無（死路）";
        std::cout << "\n請輸入方向：";
        char input;
        std::cin >> input;
        input = tolower(input);

        int DirIndex = -1;
        for (int i = 0; i < 4; ++i) {
            if (input == DirKeys[i]) {
                DirIndex = i;
                break;
            }
        }

        if (DirIndex == -1) {
            std::cout << "無效輸入，請輸入 w/s/a/d。\n";
            continue;
        }

        Point next = { curr.x + Directions[DirIndex].x, curr.y + Directions[DirIndex].y };
        if (!maze.isWalkable(next.x, next.y)) {
            std::cout << "那邊是牆壁，不能走！\n";
            waitForEnter();
            continue;
        }

        mouse.move(next);
    }
}

// --- DynamicGame 實作 ---
DynamicGame::DynamicGame(int width, int height)
    : Game(width, height), dynamicRng(std::chrono::steady_clock::now().time_since_epoch().count()) {

    std::uniform_int_distribution<int> dist_x(1, maze.getWidth() - 2);
    std::uniform_int_distribution<int> dist_y(1, maze.getHeight() - 2);

    // 循環生成直到迷宮中存在可達路徑
    bool pathFound = false;
    while (!pathFound) {
        maze.generateInitialRandomMaze(); // 重新生成隨機牆壁分佈

        // 隨機選擇起始點和終點
        do {
            startPosition = { dist_x(dynamicRng), dist_y(dynamicRng) };
        } while (maze.getCell(startPosition.x, startPosition.y) == '#'); // 確保起點不是牆壁

        do {
            currentExit = { dist_x(dynamicRng), dist_y(dynamicRng) };
        } while (maze.getCell(currentExit.x, currentExit.y) == '#' || (currentExit.x == startPosition.x && currentExit.y == startPosition.y)); // 確保終點不是牆壁且不與起點重疊

        // 檢查路徑是否存在
        pathFound = maze.hasPath(startPosition.x, startPosition.y, currentExit.x, currentExit.y);
    }
    mouse = Mouse(startPosition); // 初始化玩家位置
    end = currentExit; // 設置遊戲出口位置 (雖然 DynamicGame 直接用 currentExit，但為繼承關係還是設定)
}

int DynamicGame::play() {
    std::cout << "\n--- 動態迷宮關卡 (牆壁和出口會移動！) ---\n";
    std::cout << "每移動 10 步，迷宮中的牆壁會隨機重構！\n";
    std::cout << "每移動 15 步，出口(+)會隨機移動！\n";
    std::cout << "使用 w/a/s/d 控制老鼠(@)走出迷宮(+)。\n";
    Enter();

    while (true) {
        system(CLEAR_SCREEN);
        Point curr = mouse.getPosition();
        maze.display(curr, currentExit); // 顯示當前出口位置

        if (curr == currentExit) {
            std::cout << "\n成功走出動態迷宮！\n";
            std::cout << "總移動次數: " << mouse.getMoveCount() << "\n";
            return mouse.getMoveCount();
        }

        std::cout << "\n目前位置: (" << curr.x << ", " << curr.y << ")，總移動次數: " << mouse.getMoveCount();
        std::cout << "，可走方向: ";
        bool hasValidMove = false;
        for (int i = 0; i < 4; ++i) {
            Point next = { curr.x + Directions[i].x, curr.y + Directions[i].y };
            if (maze.isWalkable(next.x, next.y)) {
                std::cout << DirNames[i] << "(" << DirKeys[i] << ") ";
                hasValidMove = true;
            }
        }
        if (!hasValidMove) std::cout << "無（死路）";
        std::cout << "\n請輸入方向：";
        char input;
        std::cin >> input;
        input = tolower(input);

        int DirIndex = -1;
        for (int i = 0; i < 4; ++i) {
            if (input == DirKeys[i]) {
                DirIndex = i;
                break;
            }
        }

        if (DirIndex == -1) {
            std::cout << "無效輸入，請輸入 w/s/a/d。\n";
            continue;
        }

        Point next = { curr.x + Directions[DirIndex].x, curr.y + Directions[DirIndex].y };
        if (!maze.isWalkable(next.x, next.y)) {
            std::cout << "那邊是牆壁，不能走！\n";
            waitForEnter();
            continue;
        }

        mouse.move(next);
        // 每次移動後進行檢查和重構
        // 牆壁重構頻率：每 10 步
        if (mouse.getMoveCount() % 10 == 0) {
            maze.reconstructWall(mouse.getPosition().x, mouse.getPosition().y, currentExit.x, currentExit.y);
            std::cout << "\n迷宮牆壁重構了！\n";
        }
        // 出口移動頻率：每 15 步
        if (mouse.getMoveCount() % 15 == 0) {
            currentExit = maze.relocateExit(mouse.getPosition().x, mouse.getPosition().y, currentExit);
            std::cout << "\n出口位置移動了！\n";
        }
    }
}
