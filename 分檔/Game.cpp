#include "Game.h"
#include "Utility.h" // �]�t�M�̧�
#include <chrono>
#include <thread>



// Game ��H�������غc�l 1
Game::Game(const std::vector<std::vector<int>>& mazeData, Point start, Point end)
    : maze(mazeData), mouse(start), end(end) {
}

// Game ��H�������غc�l 2
Game::Game(const std::vector<std::string>& rawMazeData, Point start, Point end)
    : maze(rawMazeData), mouse(start), end(end) {
}

// Game ��H�������غc�l 3
Game::Game(int width, int height)
    : maze(width, height), mouse({ 1,1 }), end({ width - 2, height - 2 }) {
    // ��ڪ��_�l�I�M���I�|�b DynamicGame �����]�m
}

// --- NormalGame ��@ ---
NormalGame::NormalGame(const std::vector<std::vector<int>>& mazeData, Point start, Point end)
    : Game(mazeData, start, end) {
}

int NormalGame::play() {
    std::cout << "\n--- ���q�g�c���d ---\n";
    std::cout << "�ϥ� w/a/s/d ����ѹ�(@)���X�g�c(+)�C\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));

    while (true) {
        system(CLEAR_SCREEN);
        Point curr = mouse.getPosition();
        maze.display(curr, end); // �L�g�����

        if (curr == end) {
            std::cout << "\n���\���X�g�c�I\n";
            std::cout << "�`���ʦ���: " << mouse.getMoveCount() << "\n";
            return mouse.getMoveCount();
        }

        std::cout << "\n�ثe��m: (" << curr.x << ", " << curr.y << ")�A�i����V: ";
        bool hasValidMove = false;
        for (int i = 0; i < 4; ++i) {
            Point next = { curr.x + Directions[i].x, curr.y + Directions[i].y };
            if (maze.isWalkable(next.x, next.y)) {
                std::cout << DirNames[i] << "(" << DirKeys[i] << ") ";
                hasValidMove = true;
            }
        }
        if (!hasValidMove) std::cout << "�L�]�����^";
        std::cout << "\n�п�J��V�G";
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
            std::cout << "�L�Ŀ�J�A�п�J w/s/a/d�C\n";
            std::this_thread::sleep_for(std::chrono::seconds(1));  // �� 1 ��
            continue;
        }

        Point next = { curr.x + Directions[DirIndex].x, curr.y + Directions[DirIndex].y };
        if (!maze.isWalkable(next.x, next.y)) {
            std::cout << "����O����A���ਫ�I\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            continue;
        }

        mouse.move(next);
    }
}

// --- BoxGame ��@ ---
BoxGame::BoxGame(const std::vector<std::vector<int>>& mazeData, Point start, Point end, Point boxStart, Point target)
    : Game(mazeData, start, end), box(boxStart), boxTarget(target), startMouse(start), startBox(boxStart) {
}

int BoxGame::play() {
    std::cout << "\n--- ���c�l�g�c���d ---\n";
    std::cout << "�A������c�l(2)���� X �~�ਫ�X�g�c(+)�I\n";
    std::cout << "��J r �i���m��m�C\n";
    std::this_thread::sleep_for(std::chrono::seconds(3));


    while (true) {
        system(CLEAR_SCREEN);
        Point curr = mouse.getPosition();
        maze.display(curr, end, &box, &boxTarget); // �L�g�����

        if (curr == end && box == boxTarget) {
            std::cout << "\n���ߡI�A���\�N�c�l����ؼШè��X�g�c�I\n";
            std::cout << "�`���ʦ���: " << mouse.getMoveCount() << "\n";
            return mouse.getMoveCount();
        }

        std::cout << "\n�ثe��m: (" << curr.x << ", " << curr.y << ")�A�i����V: ";
        bool hasValidMove = false;
        for (int i = 0; i < 4; ++i) {
            Point nextPlayerPos = { curr.x + Directions[i].x, curr.y + Directions[i].y };
            if (maze.isWalkable(nextPlayerPos.x, nextPlayerPos.y)) {
                if (nextPlayerPos == box) {
                    Point nextBoxPos = { box.x + Directions[i].x, box.y + Directions[i].y };
                    // �ˬd�c�l���ʫ᪺��m�O�_�i�q��A�B���P���a��e��m���|�]����c�l���쪱�a���W�^
                    if (maze.isWalkable(nextBoxPos.x, nextBoxPos.y) && nextBoxPos != curr) {
                        std::cout << DirNames[i] << "(" << DirKeys[i] << ", ���c) ";
                        hasValidMove = true;
                    }
                }
                else {
                    std::cout << DirNames[i] << "(" << DirKeys[i] << ") ";
                    hasValidMove = true;
                }
            }
        }
        std::cout << "���m(r) ";
        if (!hasValidMove) std::cout << "�L���Ĳ���";
        std::cout << "\n�п�J��V�G";
        char input;
        std::cin >> input;
        input = tolower(input);

        if (input == 'r') {
            box = startBox;
            mouse = Mouse(startMouse);
            std::cout << "�w���m��m�I\n";
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
            std::cout << "�L�Ŀ�J�A�п�J w/s/a/d �� r�C\n";
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
                std::cout << "�c�l�L�k���ʨ��Ӥ�V�I\n";
            }
        }
        else if (maze.isWalkable(nextPlayerPos.x, nextPlayerPos.y)) {
            mouse.move(nextPlayerPos);
        }
        else {
            std::cout << "����O����A���ਫ�I\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
        }
    }
}

// --- FogGame ��@ ---
FogGame::FogGame(const std::vector<std::string>& rawMazeData, Point start, Point end)
    : Game(rawMazeData, start, end) {
    // �b�غc�l���AGame ���غc�l�|�۰ʩI�s maze.updateVisibility �i���l��
}

int FogGame::play() {
    std::cout << "\n--- �g���g�c���d ---\n";
    std::cout << "�����ϰ�Q�g��Ţ�n�A�u�������~�ദ�}�����I\n";
    std::cout << "�ϥ� w/a/s/d ����ѹ�(@)���X�g�c(+)�C\n";
    std::this_thread::sleep_for(std::chrono::seconds(2));

    while (true) {
        system(CLEAR_SCREEN);
        Point curr = mouse.getPosition();
        maze.updateVisibility(curr.x, curr.y); // ��s�g������
        maze.display(curr, end); // �a�g�����

        if (curr == end) {
            std::cout << "\n���\���X�g�c�I\n";
            std::cout << "�`���ʦ���: " << mouse.getMoveCount() << "\n";
            return mouse.getMoveCount();
        }

        std::cout << "\n�ثe��m: (" << curr.x << ", " << curr.y << ")�A�i����V: ";
        bool hasValidMove = false;
        for (int i = 0; i < 4; ++i) {
            Point next = { curr.x + Directions[i].x, curr.y + Directions[i].y };
            if (maze.isWalkable(next.x, next.y)) {
                std::cout << DirNames[i] << "(" << DirKeys[i] << ") ";
                hasValidMove = true;
            }
        }
        if (!hasValidMove) std::cout << "�L�]�����^";
        std::cout << "\n�п�J��V�G";
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
            std::cout << "�L�Ŀ�J�A�п�J w/s/a/d�C\n";
            continue;
        }

        Point next = { curr.x + Directions[DirIndex].x, curr.y + Directions[DirIndex].y };
        if (!maze.isWalkable(next.x, next.y)) {
            std::cout << "����O����A���ਫ�I\n";
            std::this_thread::sleep_for(std::chrono::milliseconds(500));
            continue;
        }

        mouse.move(next);
    }
}

// --- DynamicGame ��@ ---
DynamicGame::DynamicGame(int width, int height)
    : Game(width, height), dynamicRng(std::chrono::steady_clock::now().time_since_epoch().count()) {

    std::uniform_int_distribution<int> dist_x(1, maze.getWidth() - 2);
    std::uniform_int_distribution<int> dist_y(1, maze.getHeight() - 2);

    // �`���ͦ�����g�c���s�b�i�F���|
    bool pathFound = false;
    while (!pathFound) {
        maze.generateInitialRandomMaze(); // ���s�ͦ��H��������G

        // �H����ܰ_�l�I�M���I
        do {
            startPosition = { dist_x(dynamicRng), dist_y(dynamicRng) };
        } while (maze.getCell(startPosition.x, startPosition.y) == '#'); // �T�O�_�I���O���

        do {
            currentExit = { dist_x(dynamicRng), dist_y(dynamicRng) };
        } while (maze.getCell(currentExit.x, currentExit.y) == '#' || (currentExit.x == startPosition.x && currentExit.y == startPosition.y)); // �T�O���I���O����B���P�_�I���|

        // �ˬd���|�O�_�s�b
        pathFound = maze.hasPath(startPosition.x, startPosition.y, currentExit.x, currentExit.y);
    }
    mouse = Mouse(startPosition); // ��l�ƪ��a��m
    end = currentExit; // �]�m�C���X�f��m (���M DynamicGame ������ currentExit�A�����~�����Y�٬O�]�w)
}

int DynamicGame::play() {
    std::cout << "\n--- �ʺA�g�c���d (����M�X�f�|���ʡI) ---\n";
    std::cout << "�C���� 10 �B�A�g�c��������|�H�����c�I\n";
    std::cout << "�C���� 15 �B�A�X�f(+)�|�H�����ʡI\n";
    std::cout << "�ϥ� w/a/s/d ����ѹ�(@)���X�g�c(+)�C\n";
    std::this_thread::sleep_for(std::chrono::seconds(3));

    while (true) {
        system(CLEAR_SCREEN);
        Point curr = mouse.getPosition();
        maze.display(curr, currentExit); // ��ܷ�e�X�f��m

        if (curr == currentExit) {
            std::cout << "\n���\���X�ʺA�g�c�I\n";
            std::cout << "�`���ʦ���: " << mouse.getMoveCount() << "\n";
            return mouse.getMoveCount();
        }

        std::cout << "\n�ثe��m: (" << curr.x << ", " << curr.y << ")�A�`���ʦ���: " << mouse.getMoveCount();
        std::cout << "�A�i����V: ";
        bool hasValidMove = false;
        for (int i = 0; i < 4; ++i) {
            Point next = { curr.x + Directions[i].x, curr.y + Directions[i].y };
            if (maze.isWalkable(next.x, next.y)) {
                std::cout << DirNames[i] << "(" << DirKeys[i] << ") ";
                hasValidMove = true;
            }
        }
        if (!hasValidMove) std::cout << "�L�]�����^";
        std::cout << "\n�п�J��V�G";
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
            std::cout << "�L�Ŀ�J�A�п�J w/s/a/d�C\n";
            continue;
        }

        Point next = { curr.x + Directions[DirIndex].x, curr.y + Directions[DirIndex].y };
        if (!maze.isWalkable(next.x, next.y)) {
            std::cout << "����O����A���ਫ�I\n";
             std::this_thread::sleep_for(std::chrono::milliseconds(500));
            continue;
        }

        mouse.move(next);
        // �C�����ʫ�i���ˬd�M���c
        // ������c�W�v�G�C 10 �B
        if (mouse.getMoveCount() % 10 == 0) {
            maze.reconstructWall(mouse.getPosition().x, mouse.getPosition().y, currentExit.x, currentExit.y);
            std::cout << "\n�g�c������c�F�I\n";
        }
        // �X�f�����W�v�G�C 15 �B
        if (mouse.getMoveCount() % 15 == 0) {
            currentExit = maze.relocateExit(mouse.getPosition().x, mouse.getPosition().y, currentExit);
            std::cout << "\n�X�f��m���ʤF�I\n";
        }
    }
}