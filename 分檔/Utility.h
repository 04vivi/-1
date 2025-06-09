#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <string>

// 沮穨╰参﹚竡睲㏑
#ifdef _WIN32
#define CLEAR_SCREEN "cls" // Windows ╰参ㄏノ cls
#else
#define CLEAR_SCREEN "clear" // Linux/macOS ╰参ㄏノ clear
#endif

// --- 程沧 ASCII 秤礶 ---
void printVictoryScreen();

#endif // UTILITY_H