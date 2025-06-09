#ifndef MAZE_H
#define MAZE_H

#include <vector>
#include <string>
#include <random>
#include <chrono>
#include <queue> // For BFS
#include "Point.h" // �]�t Point ���O

// --- Visibility �C�| (�ȥΩ�g�����d) ---
enum Visibility {
    UNSEEN,      // ���a�q����X�Ψ��L���ϰ� (��ܬ� '.')
    SEEN_BEFORE, // ���a���g�ݨ�L�A���ثe���b��e�����d�򤺪��ϰ� (��ܬ� '-')
    VISIBLE      // ���a��e�����d�򤺪��ϰ� (��ܹ�ڤ��e)
};

// --- Maze ���O ---
class Maze {
private:
    std::vector<std::vector<char>> mazeGrid; // �p������
    std::vector<std::vector<Visibility>> visibility; // �ȥΩ�g���t��
    bool isFoggy; // �аO���g�c�O�_�ϥΰg��
    bool isDynamic; // �аO���g�c�O�_���ʺA�g�c

    int MAZE_WIDTH;
    int MAZE_HEIGHT;

    // �H���ƥͦ���
    std::mt19937 rng;

public:
    // �غc�l 1�G�Ω� int ���� (���d 1-4)
    Maze(const std::vector<std::vector<int>>& initialGrid);
    // �غc�l 2�G�Ω� string �V�q (���d 5 - �g���t��)
    Maze(const std::vector<std::string>& initialRawMaze);
    // �غc�l 3�G�Ω�ʺA�g�c (���d 6 - ���c�t��)
    Maze(int width, int height, bool dynamic = true);

    // ��l�ͦ��H���g�c (�Ω����d 6 �� Maze �غc�l)
    void generateInitialRandomMaze();

    // ���c��� (�Ω����d 6)�A�ýT�O�i�F��
    void reconstructWall(int playerX, int playerY, int exitX, int exitY);

    // ���s�w��X�f (�Ω����d 6)�A�ýT�O�i�F��
    Point relocateExit(int playerX, int playerY, Point currentExit);

    // ��s�g������ (�ȥΩ�g�����d)
    void updateVisibility(int playerX, int playerY, int radius = 3);

    // ø�s�g�c�챱��x�A�Ҽ{���a�B�c�l�B�X�f�M�i����
    void display(const Point& mouse, const Point& exit, const Point* box = nullptr, const Point* target = nullptr) const;

    // �ˬd���w�y�ЬO�_�i�q�� (���|����)
    bool isWalkable(int x, int y) const;

    // �ˬd�q�_�I����I�O�_�����| (�ϥ� BFS)
    bool hasPath(int startX, int startY, int endX, int endY) const;

    // ����g�c�椸�檺���e
    char getCell(int x, int y) const;

    // �]�m�g�c�椸�檺���e (�s�W�����@��k)
    void setCell(int x, int y, char value);

    // �M��S�w�r���b�g�c������m (�Ω� string ���檺 P �M E)
    Point findChar(char c) const;

    int getWidth() const;
    int getHeight() const;
};

#endif // MAZE_H