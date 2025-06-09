#include "Maze.h"
#include <iostream> // For std::cout in display, debug
#include <cmath>    // For abs
#include <queue>    // For std::queue in hasPath

// �غc�l 1�G�Ω� int ���� (���d 1-4)
Maze::Maze(const std::vector<std::vector<int>>& initialGrid)
    : MAZE_HEIGHT(initialGrid.size()), MAZE_WIDTH(initialGrid[0].size()),
    isFoggy(false), isDynamic(false),
    rng(std::chrono::steady_clock::now().time_since_epoch().count()) // �H�ɶ����ؤl
{
    mazeGrid.resize(MAZE_HEIGHT, std::vector<char>(MAZE_WIDTH));
    visibility.resize(MAZE_HEIGHT, std::vector<Visibility>(MAZE_WIDTH, UNSEEN)); // ���M��l�Ƥj�p

    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            if (initialGrid[y][x] == 1) {
                mazeGrid[y][x] = '#'; // ���
            }
            else {
                mazeGrid[y][x] = ' '; // �i�q����|
            }
        }
    }
}

// �غc�l 2�G�Ω� string �V�q (���d 5 - �g���t��)
Maze::Maze(const std::vector<std::string>& initialRawMaze)
    : MAZE_HEIGHT(initialRawMaze.size()), MAZE_WIDTH(initialRawMaze[0].length()),
    isFoggy(true), isDynamic(false),
    rng(std::chrono::steady_clock::now().time_since_epoch().count()) // �H�ɶ����ؤl
{
    mazeGrid.resize(MAZE_HEIGHT, std::vector<char>(MAZE_WIDTH));
    visibility.resize(MAZE_HEIGHT, std::vector<Visibility>(MAZE_WIDTH, UNSEEN));

    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            mazeGrid[y][x] = initialRawMaze[y][x];
        }
    }
}

// �غc�l 3�G�Ω�ʺA�g�c (���d 6 - ���c�t��)
Maze::Maze(int width, int height, bool dynamic)
    : MAZE_WIDTH(width), MAZE_HEIGHT(height),
    isFoggy(false), isDynamic(dynamic),
    rng(std::chrono::steady_clock::now().time_since_epoch().count()) // �H�ɶ����ؤl
{
    mazeGrid.resize(MAZE_HEIGHT, std::vector<char>(MAZE_WIDTH));
    visibility.resize(MAZE_HEIGHT, std::vector<Visibility>(MAZE_WIDTH, UNSEEN)); // ���M��l�Ƥj�p
    generateInitialRandomMaze(); // ��l�ͦ��H���g�c
}

// ��l�ͦ��H���g�c (�Ω����d 6 �� Maze �غc�l)
void Maze::generateInitialRandomMaze() {
    for (int y = 0; y < MAZE_HEIGHT; ++y) {
        for (int x = 0; x < MAZE_WIDTH; ++x) {
            if (y == 0 || y == MAZE_HEIGHT - 1 || x == 0 || x == MAZE_WIDTH - 1) {
                mazeGrid[y][x] = '#'; // ��ɬO��
            }
            else {
                mazeGrid[y][x] = (rng() % 3 == 0) ? '#' : ' '; // �� 33% �O��
            }
        }
    }
}

// ���c��� (�Ω����d 6)�A�ýT�O�i�F��
void Maze::reconstructWall(int playerX, int playerY, int exitX, int exitY) {
    if (!isDynamic) return;

    std::vector<std::vector<char>> originalGrid = mazeGrid; // �O�s��e�g�c���A�H�K�^��

    std::uniform_int_distribution<int> dist_x(1, MAZE_WIDTH - 2);
    std::uniform_int_distribution<int> dist_y(1, MAZE_HEIGHT - 2);

    bool pathExists = false;
    for (int attempt = 0; attempt < 50; ++attempt) { // �̦h���� 50 ��
        mazeGrid = originalGrid; // �C�����ճ��q��l���A�}�l

        // �H�����ܤ@�w�ƶq����l
        int changes = 5 + (rng() % 5); // �C�����է��� 5-9 �Ӯ�l
        for (int i = 0; i < changes; ++i) {
            int rx = dist_x(rng);
            int ry = dist_y(rng);

            // �קK���ܪ��a�B�X�f�ΫD�`�a�񪱮a����m
            // �Z�����a�M�X�f�ܤ� 2 ��
            if ((std::abs(rx - playerX) > 1 || std::abs(ry - playerY) > 1) &&
                (std::abs(rx - exitX) > 1 || std::abs(ry - exitY) > 1) &&
                (rx != playerX || ry != playerY) && (rx != exitX || ry != exitY))
            {
                mazeGrid[ry][rx] = (mazeGrid[ry][rx] == '#') ? ' ' : '#'; // ���ܸ��θ�����
            }
        }

        // �ˬd�O�_���M�����|
        if (hasPath(playerX, playerY, exitX, exitY)) { // �ϥ� Maze �ۤv�� hasPath
            pathExists = true;
            break; // ���i����|�A���X�`��
        }
    }

    if (!pathExists) {
        mazeGrid = originalGrid; // �p�G�Ҧ����ճ����ѡA��_�쭫�c�e�����A
        // std::cout << "DEBUG: ���c���ѡA��_���g�c�C\n"; // �i�Ω�ո�
    }
}

// ���s�w��X�f (�Ω����d 6)�A�ýT�O�i�F��
Point Maze::relocateExit(int playerX, int playerY, Point currentExit) {
    if (!isDynamic) return currentExit;

    std::vector<std::vector<char>> originalGrid = mazeGrid;
    Point newExit = currentExit;

    std::uniform_int_distribution<int> dist_x(1, MAZE_WIDTH - 2);
    std::uniform_int_distribution<int> dist_y(1, MAZE_HEIGHT - 2);

    for (int attempt = 0; attempt < 50; ++attempt) { // �̦h���� 50 ��
        int potentialExitX = dist_x(rng);
        int potentialExitY = dist_y(rng);

        // �T�O�s�X�f���b���a��e��m�A�B�O�i�q��Ŷ�
        if ((potentialExitX != playerX || potentialExitY != playerY) &&
            mazeGrid[potentialExitY][potentialExitX] == ' ') {

            // �{�ɦa�N potentialExit �]�m���X�f�A�ˬd���|
            char originalContent = mazeGrid[potentialExitY][potentialExitX];
            mazeGrid[potentialExitY][potentialExitX] = 'E'; // �ȮɼаO���X�f

            if (hasPath(playerX, playerY, potentialExitX, potentialExitY)) { // �ϥ� Maze �ۤv�� hasPath
                newExit = { potentialExitX, potentialExitY };
                // ���ݭn�b�o�̲M���¥X�f�γ]�m�s�X�f�A�]�� DynamicGame �|�b draw �ɳB�z�o��
                mazeGrid[potentialExitY][potentialExitX] = originalContent; // ��_��l���e
                return newExit; // ���s�X�f�ê�^
            }
            else {
                mazeGrid[potentialExitY][potentialExitX] = originalContent; // ��_
            }
        }
    }
    mazeGrid = originalGrid; // �p�G�Ҧ����ճ����ѡA��_�쭫�c�e�����A
    // std::cout << "DEBUG: �X�f���w�쥢�ѡA������X�f�C\n"; // �i�Ω�ո�
    return currentExit; // �p�G�䤣��s�X�f�A��^��X�f
}

// ��s�g������ (�ȥΩ�g�����d)
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

// ø�s�g�c�챱��x�A�Ҽ{���a�B�c�l�B�X�f�M�i����
void Maze::display(const Point& mouse, const Point& exit, const Point* box, const Point* target) const {
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            // �����P�_�O�_���g�����d�A�îھڥi������ܤ��e
            if (isFoggy) {
                if (visibility[y][x] == UNSEEN) {
                    std::cout << ". ";
                    continue; // ���ݹL���ϰ�A�������L����P�_
                }
                else if (visibility[y][x] == SEEN_BEFORE) {
                    // ���g�ݹL���ثe���b���������ϰ�A��� '-'
                    // ���O�p�G�Ӧ�m�O���a�B�c�l�ΥX�f�A���M������ܨ�ϼ�
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
                        std::cout << "- "; // �_�h��ܬ����g�ݹL���g��
                    }
                    continue;
                }
                // VISIBLE ���p�U�~�򩹤U����A��ܹ�ڤ��e
            }

            // �D�g�����d�ΰg�����d�� VISIBLE �ϰ�
            if (x == mouse.x && y == mouse.y) {
                std::cout << "@ "; // �ѹ���m
            }
            else if (box && x == box->x && y == box->y) {
                std::cout << "2 "; // �c�l
            }
            else if (x == exit.x && y == exit.y) {
                std::cout << "+ "; // �X�f
            }
            else if (target && x == target->x && y == target->y) {
                std::cout << "X "; // �c�l�ؼ�
            }
            else {
                std::cout << mazeGrid[y][x] << " "; // �g�c���e
            }
        }
        std::cout << std::endl;
    }
}

// �ˬd���w�y�ЬO�_�i�q�� (���|����)
bool Maze::isWalkable(int x, int y) const {
    return (x >= 0 && x < MAZE_WIDTH && y >= 0 && y < MAZE_HEIGHT && mazeGrid[y][x] != '#');
}

// �ˬd�q�_�I����I�O�_�����| (�ϥ� BFS)
bool Maze::hasPath(int startX, int startY, int endX, int endY) const {
    // �p�G�_�I�β��I�O����A�h�L���|
    if (mazeGrid[startY][startX] == '#' || mazeGrid[endY][endX] == '#') {
        return false;
    }

    std::vector<std::vector<bool>> visited(MAZE_HEIGHT, std::vector<bool>(MAZE_WIDTH, false));
    std::queue<Point> q;

    q.push({ startX, startY });
    visited[startY][startX] = true;

    int dx[] = { 0, 0, 1, -1 }; // �W�U���k
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
    return false; // �L�k��F���I
}

// ����g�c�椸�檺���e
char Maze::getCell(int x, int y) const {
    if (x >= 0 && x < MAZE_WIDTH && y >= 0 && y < MAZE_HEIGHT) {
        return mazeGrid[y][x];
    }
    return '#'; // �W�X��ɵ������
}

// �]�m�g�c�椸�檺���e (�s�W�����@��k)
void Maze::setCell(int x, int y, char value) {
    if (x >= 0 && x < MAZE_WIDTH && y >= 0 && y < MAZE_HEIGHT) {
        mazeGrid[y][x] = value;
    }
}

// �M��S�w�r���b�g�c������m (�Ω� string ���檺 P �M E)
Point Maze::findChar(char c) const {
    for (int y = 0; y < MAZE_HEIGHT; y++) {
        for (int x = 0; x < MAZE_WIDTH; x++) {
            if (mazeGrid[y][x] == c) {
                return { x, y };
            }
        }
    }
    return { -1, -1 }; // �����h��^ (-1,-1)
}

int Maze::getWidth() const { return MAZE_WIDTH; }
int Maze::getHeight() const { return MAZE_HEIGHT; }