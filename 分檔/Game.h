#ifndef GAME_H
#define GAME_H

#include <vector>
#include <string>
#include <iostream> // For std::cout, std::cin
#include <cctype>   // For tolower
#include <utility>  // For std::pair (although Point is used)
#include "Maze.h"
#include "Mouse.h"
#include "Point.h" // �]�t Point ���O�A�T�O�Ҧ��C�����O����ϥ�

// --- Game ��H���� ---
class Game {
protected:
    Maze maze;
    Mouse mouse;
    Point end; // �X�f��m

    const std::vector<Point> Directions = { {0, -1}, {0, 1}, {-1, 0}, {1, 0} }; // �W, �U, ��, �k
    const std::vector<char> DirKeys = { 'w', 's', 'a', 'd' };
    const std::vector<std::string> DirNames = { "�W", "�U", "��", "�k" };

public:
    // �غc�l 1�G�Ω� int ���檺�g�c (���d 1-4)
    Game(const std::vector<std::vector<int>>& mazeData, Point start, Point end);
    // �غc�l 2�G�Ω� string ���檺�g�c (���d 5)
    Game(const std::vector<std::string>& rawMazeData, Point start, Point end);
    // �غc�l 3�G�Ω�ʺA�g�c (���d 6)
    Game(int width, int height);

    virtual ~Game() = default;

    virtual int play() = 0;
};

// --- NormalGame ���O (���d 1-3) ---
class NormalGame : public Game {
public:
    NormalGame(const std::vector<std::vector<int>>& mazeData, Point start, Point end);
    int play() override;
};

// --- BoxGame ���O (���d 4) ---
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

// --- FogGame ���O (���d 5) ---
class FogGame : public Game {
public:
    FogGame(const std::vector<std::string>& rawMazeData, Point start, Point end);
    int play() override;
};

// --- DynamicGame ���O (���d 6 - ���c�t��) ---
class DynamicGame : public Game {
private:
    std::mt19937 dynamicRng; // �M���Ω�ʺA�g�c���H���ƥͦ���
    Point currentExit; // �x�s��e�X�f��m
    Point startPosition; // �x�s���a�_�l��m�A�Ω�b���c��T�O���|

public:
    DynamicGame(int width, int height);
    int play() override;
};

#endif // GAME_H