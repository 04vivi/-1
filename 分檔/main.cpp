#include <iostream>
#include <vector>
#include <string>
#include "Point.h"
#include "Maze.h"
#include "Mouse.h"
#include "Game.h" // �]�t�Ҧ��C�����O����H����
#include "Utility.h" // �]�t�M�̧��M�ӧQ�e��

// �U���d�g�c���
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

// ���d 4 �ϥΪ��a�� (�P mazeMap3 �ۦP)
std::vector<std::vector<int>> mazeMap4_box = mazeMap3;

// ���d 5 (�g���t��) �ϥΪ���l�r��g�c
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

    // ���d 1
    system(CLEAR_SCREEN);
    std::cout << "\n--- ���� 1 (²��g�c) ---\n";
    NormalGame game1(mazeMap1, Point(1, 1), Point(3, 3));
    totalMoves += game1.play();

    // ���d 2
    system(CLEAR_SCREEN);
    std::cout << "\n--- ���� 2 (�����g�c) ---\n";
    NormalGame game2(mazeMap2, Point(1, 1), Point(5, 5));
    totalMoves += game2.play();

    // ���d 3
    system(CLEAR_SCREEN);
    std::cout << "\n--- ���� 3 (�����g�c) ---\n";
    NormalGame game3(mazeMap3, Point(1, 1), Point(8, 8));
    totalMoves += game3.play();

    // ���d 4 (���c�l�g�c)
    system(CLEAR_SCREEN);
    std::cout << "\n--- ���� 4 (���c�l�g�c) ---\n";
    BoxGame game4(mazeMap4_box, Point(1, 1), Point(8, 8), Point(3, 2), Point(6, 8));
    totalMoves += game4.play();

    // ���d 5 (�g���g�c)
    system(CLEAR_SCREEN);
    std::cout << "\n--- ���� 5 (�g���g�c) ---\n";
    FogGame game5(rawMaze_fog, Point(1, 1), Point(18, 3));
    totalMoves += game5.play();

    // ���d 6 (�ʺA���c�g�c)
    system(CLEAR_SCREEN);
    std::cout << "\n--- ���� 6 (�ʺA���c�g�c) ---\n";
    DynamicGame game6(30, 15); // ���A�ݭn�ǤJ start �M end�A���̷|�b DynamicGame �����B�z
    totalMoves += game6.play();


    // �����Ҧ����d�᪺�`��
    system(CLEAR_SCREEN);
    printVictoryScreen();
    std::cout << "���߱z�I�����Ҧ����d�I\n";
    std::cout << "�`���ʦ���: " << totalMoves << " ���C\n";

    std::cout << "���U Enter �䵲���C��...";
    std::cin.ignore();
    std::cin.get();
    return 0;
}