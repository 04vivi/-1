#include "Maze.h"
#include <iostream> // For std::cout in display, debug
#include <cmath>    // For abs
#include <queue>    // For std::queue in hasPath

// 建構子 1：用於 int 網格 (關卡 1-4)
Maze::Maze(const std::vector<std::vector<int>>& initialGrid)
    : MAZE_HEIGHT(initialGrid.size()), MAZE_WIDTH(initialGrid[0].size()),
    isFoggy(false), isDynamic(false),
    rng(std::chrono::steady_clock::now().time_since_epoch().count()) // 以時間為種子
{
    mazeGrid.resize(MAZE_HEIGHT, std::vector<char>(MAZE_WIDTH));
    visibility.resize(MAZE_HEIGHT, std::vector<Visibility>(MAZE_WIDTH, UNSEEN)); // 仍然初始化大小

    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            if (initialGrid[y][x] == 1) {
                mazeGrid[y][x] = '#'; // 牆壁
            }
            else {
                mazeGrid[y][x] = ' '; // 可通行路徑
            }
        }
    }
}

// 建構子 2：用於 string 向量 (關卡 5 - 迷霧系統)
Maze::Maze(const std::vector<std::string>& initialRawMaze)
    : MAZE_HEIGHT(initialRawMaze.size()), MAZE_WIDTH(initialRawMaze[0].length()),
    isFoggy(true), isDynamic(false),
    rng(std::chrono::steady_clock::now().time_since_epoch().count()) // 以時間為種子
{
    mazeGrid.resize(MAZE_HEIGHT, std::vector<char>(MAZE_WIDTH));
    visibility.resize(MAZE_HEIGHT, std::vector<Visibility>(MAZE_WIDTH, UNSEEN));

    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            mazeGrid[y][x] = initialRawMaze[y][x];
        }
    }
}

// 建構子 3：用於動態迷宮 (關卡 6 - 重構系統)
Maze::Maze(int width, int height, bool dynamic)
    : MAZE_WIDTH(width), MAZE_HEIGHT(height),
    isFoggy(false), isDynamic(dynamic),
    rng(std::chrono::steady_clock::now().time_since_epoch().count()) // 以時間為種子
{
    mazeGrid.resize(MAZE_HEIGHT, std::vector<char>(MAZE_WIDTH));
    visibility.resize(MAZE_HEIGHT, std::vector<Visibility>(MAZE_WIDTH, UNSEEN)); // 仍然初始化大小
    generateInitialRandomMaze(); // 初始生成隨機迷宮
}

// 初始生成隨機迷宮 (用於關卡 6 的 Maze 建構子)
void Maze::generateInitialRandomMaze() {
    for (int y = 0; y < MAZE_HEIGHT; ++y) {
        for (int x = 0; x < MAZE_WIDTH; ++x) {
            if (y == 0 || y == MAZE_HEIGHT - 1 || x == 0 || x == MAZE_WIDTH - 1) {
                mazeGrid[y][x] = '#'; // 邊界是牆
            }
            else {
                mazeGrid[y][x] = (rng() % 3 == 0) ? '#' : ' '; // 約 33% 是牆
            }
        }
    }
}

// 重構牆壁 (用於關卡 6)，並確保可達性
void Maze::reconstructWall(int playerX, int playerY, int exitX, int exitY) {
    if (!isDynamic) return;

    std::vector<std::vector<char>> originalGrid = mazeGrid; // 保存當前迷宮狀態以便回溯

    std::uniform_int_distribution<int> dist_x(1, MAZE_WIDTH - 2);
    std::uniform_int_distribution<int> dist_y(1, MAZE_HEIGHT - 2);

    bool pathExists = false;
    for (int attempt = 0; attempt < 50; ++attempt) { // 最多嘗試 50 次
        mazeGrid = originalGrid; // 每次嘗試都從原始狀態開始

        // 隨機改變一定數量的格子
        int changes = 5 + (rng() % 5); // 每次嘗試改變 5-9 個格子
        for (int i = 0; i < changes; ++i) {
            int rx = dist_x(rng);
            int ry = dist_y(rng);

            // 避免改變玩家、出口或非常靠近玩家的位置
            // 距離玩家和出口至少 2 格
            if ((std::abs(rx - playerX) > 1 || std::abs(ry - playerY) > 1) &&
                (std::abs(rx - exitX) > 1 || std::abs(ry - exitY) > 1) &&
                (rx != playerX || ry != playerY) && (rx != exitX || ry != exitY))
            {
                mazeGrid[ry][rx] = (mazeGrid[ry][rx] == '#') ? ' ' : '#'; // 牆變路或路變牆
            }
        }

        // 檢查是否仍然有路徑
        if (hasPath(playerX, playerY, exitX, exitY)) { // 使用 Maze 自己的 hasPath
            pathExists = true;
            break; // 找到可行路徑，跳出循環
        }
    }

    if (!pathExists) {
        mazeGrid = originalGrid; // 如果所有嘗試都失敗，恢復到重構前的狀態
        // std::cout << "DEBUG: 重構失敗，恢復到原迷宮。\n"; // 可用於調試
    }
}

// 重新定位出口 (用於關卡 6)，並確保可達性
Point Maze::relocateExit(int playerX, int playerY, Point currentExit) {
    if (!isDynamic) return currentExit;

    std::vector<std::vector<char>> originalGrid = mazeGrid;
    Point newExit = currentExit;

    std::uniform_int_distribution<int> dist_x(1, MAZE_WIDTH - 2);
    std::uniform_int_distribution<int> dist_y(1, MAZE_HEIGHT - 2);

    for (int attempt = 0; attempt < 50; ++attempt) { // 最多嘗試 50 次
        int potentialExitX = dist_x(rng);
        int potentialExitY = dist_y(rng);

        // 確保新出口不在玩家當前位置，且是可通行空間
        if ((potentialExitX != playerX || potentialExitY != playerY) &&
            mazeGrid[potentialExitY][potentialExitX] == ' ') {

            // 臨時地將 potentialExit 設置為出口，檢查路徑
            char originalContent = mazeGrid[potentialExitY][potentialExitX];
            mazeGrid[potentialExitY][potentialExitX] = 'E'; // 暫時標記為出口

            if (hasPath(playerX, playerY, potentialExitX, potentialExitY)) { // 使用 Maze 自己的 hasPath
                newExit = { potentialExitX, potentialExitY };
                // 不需要在這裡清除舊出口或設置新出口，因為 DynamicGame 會在 draw 時處理這些
                mazeGrid[potentialExitY][potentialExitX] = originalContent; // 恢復原始內容
                return newExit; // 找到新出口並返回
            }
            else {
                mazeGrid[potentialExitY][potentialExitX] = originalContent; // 恢復
            }
        }
    }
    mazeGrid = originalGrid; // 如果所有嘗試都失敗，恢復到重構前的狀態
    // std::cout << "DEBUG: 出口重定位失敗，維持原出口。\n"; // 可用於調試
    return currentExit; // 如果找不到新出口，返回原出口
}

// 更新迷霧視野 (僅用於迷霧關卡)
void Maze::updateVisibility(int playerX, int playerY, int radius) {
    if (!isFoggy) return;

    for (int y = 0; y < MAZE_HEIGHT; ++y) {
        for (int x = 0; x < MAZE_WIDTH; ++x) {
            if (visibility[y][x] == VISIBLE) {
                visibility[y][x] = SEEN_BEFORE;
            }
        }
    }

    for (int dy = -radius; dy <= radius; ++dy) {
        for (int dx = -radius; dx <= radius; ++dx) {
            int nx = playerX + dx;
            int ny = playerY + dy;
            if (nx >= 0 && nx < MAZE_WIDTH && ny >= 0 && ny < MAZE_HEIGHT) {
                visibility[ny][nx] = VISIBLE;
            }
        }
    }
}

// 繪製迷宮到控制台，考慮玩家、箱子、出口和可見性
void Maze::display(const Point& mouse, const Point& exit, const Point* box, const Point* target) const {
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            // 首先判斷是否為迷霧關卡，並根據可見性顯示內容
            if (isFoggy) {
                if (visibility[y][x] == UNSEEN) {
                    std::cout << ". ";
                    continue; // 未看過的區域，直接跳過後續判斷
                }
                else if (visibility[y][x] == SEEN_BEFORE) {
                    // 曾經看過但目前不在視野內的區域，顯示 '-'
                    // 但是如果該位置是玩家、箱子或出口，仍然應該顯示其圖標
                    if (x == mouse.x && y == mouse.y) {
                        std::cout << "@ ";
                    }
                    else if (box && x == box->x && y == box->y) {
                        std::cout << "2 ";
                    }
                    else if (x == exit.x && y == exit.y) {
                        std::cout << "+ ";
                    }
                    else if (target && x == target->x && y == target->y) {
                        std::cout << "X ";
                    }
                    else {
                        std::cout << "- "; // 否則顯示為曾經看過的迷霧
                    }
                    continue;
                }
                // VISIBLE 情況下繼續往下執行，顯示實際內容
            }

            // 非迷霧關卡或迷霧關卡的 VISIBLE 區域
            if (x == mouse.x && y == mouse.y) {
                std::cout << "@ "; // 老鼠位置
            }
            else if (box && x == box->x && y == box->y) {
                std::cout << "2 "; // 箱子
            }
            else if (x == exit.x && y == exit.y) {
                std::cout << "+ "; // 出口
            }
            else if (target && x == target->x && y == target->y) {
                std::cout << "X "; // 箱子目標
            }
            else {
                std::cout << mazeGrid[y][x] << " "; // 迷宮內容
            }
        }
        std::cout << std::endl;
    }
}

// 檢查指定座標是否可通行 (不會撞牆)
bool Maze::isWalkable(int x, int y) const {
    return (x >= 0 && x < MAZE_WIDTH && y >= 0 && y < MAZE_HEIGHT && mazeGrid[y][x] != '#');
}

// 檢查從起點到終點是否有路徑 (使用 BFS)
bool Maze::hasPath(int startX, int startY, int endX, int endY) const {
    // 如果起點或終點是牆壁，則無路徑
    if (mazeGrid[startY][startX] == '#' || mazeGrid[endY][endX] == '#') {
        return false;
    }

    std::vector<std::vector<bool>> visited(MAZE_HEIGHT, std::vector<bool>(MAZE_WIDTH, false));
    std::queue<Point> q;

    q.push({ startX, startY });
    visited[startY][startX] = true;

    int dx[] = { 0, 0, 1, -1 }; // 上下左右
    int dy[] = { -1, 1, 0, 0 };

    while (!q.empty()) {
        Point curr = q.front();
        q.pop();

        if (curr.x == endX && curr.y == endY) {
            return true;
        }

        for (int i = 0; i < 4; ++i) {
            int nx = curr.x + dx[i];
            int ny = curr.y + dy[i];

            if (nx >= 0 && nx < MAZE_WIDTH && ny >= 0 && ny < MAZE_HEIGHT &&
                mazeGrid[ny][nx] != '#' && !visited[ny][nx]) {
                visited[ny][nx] = true;
                q.push({ nx, ny });
            }
        }
    }
    return false; // 無法到達終點
}

// 獲取迷宮單元格的內容
char Maze::getCell(int x, int y) const {
    if (x >= 0 && x < MAZE_WIDTH && y >= 0 && y < MAZE_HEIGHT) {
        return mazeGrid[y][x];
    }
    return '#'; // 超出邊界視為牆壁
}

// 設置迷宮單元格的內容 (新增的公共方法)
void Maze::setCell(int x, int y, char value) {
    if (x >= 0 && x < MAZE_WIDTH && y >= 0 && y < MAZE_HEIGHT) {
        mazeGrid[y][x] = value;
    }
}

// 尋找特定字元在迷宮中的位置 (用於 string 網格的 P 和 E)
Point Maze::findChar(char c) const {
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            if (mazeGrid[y][x] == c) {
                return { x, y };
            }
        }
    }
    return { -1, -1 }; // 未找到則返回 (-1,-1)
}

int Maze::getWidth() const { return MAZE_WIDTH; }
int Maze::getHeight() const { return MAZE_HEIGHT; }