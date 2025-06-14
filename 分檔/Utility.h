#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <string>

// 根據作業系統定義清屏命令
#ifdef _WIN32
#define CLEAR_SCREEN "cls" // Windows 系統使用 cls
#else
#define CLEAR_SCREEN "clear" // Linux/macOS 系統使用 clear
#endif


void printVictoryScreen();
void waitForEnter();//撞到牆壁，按下enter才可以繼續
void Enter();//按下enter繼續，用於解釋畫面

#endif // UTILITY_H
