#ifndef UTILITY_H
#define UTILITY_H

#include <iostream>
#include <string>

// �ھڧ@�~�t�Ωw�q�M�̩R�O
#ifdef _WIN32
#define CLEAR_SCREEN "cls" // Windows �t�Ψϥ� cls
#else
#define CLEAR_SCREEN "clear" // Linux/macOS �t�Ψϥ� clear
#endif

// --- �̲� ASCII �ӧQ�e�� ---
void printVictoryScreen();

#endif // UTILITY_H