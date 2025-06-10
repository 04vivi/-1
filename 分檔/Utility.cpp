#include "Utility.h"
#include <string>

void Enter() {
    std::cout << "請按 Enter 鍵繼續...";
    std::cin.ignore(); // 忽略之前輸入留下的換行符
    std::cin.get();    // 等待玩家按 Enter
}

void waitForEnter() {
    std::cout << "撞到牆了！請按 Enter 鍵繼續...";
    std::cin.ignore(); // 忽略之前輸入留下的換行符
    std::cin.get();    // 等待玩家按 Enter
}


// --- 最終 ASCII 勝利畫面 ---
void printVictoryScreen() {
    const std::string message[] = {
        "  ___    ___ ________  ___  ___          ___       __   ___  ________      ",
        " |\\  \\  /  /|\\   __  \\|\\  \\|\\  \\        |\\  \\     |\\  \\|\\  \\|\\   ___  \\    ",
        " \\ \\  \\/  / | \\  \\|\\  \\ \\  \\\\\\  \\       \\ \\  \\    \\ \\  \\ \\  \\ \\  \\\\ \\  \\   ",
        "  \\ \\    / / \\ \\  \\\\\\  \\ \\  \\\\\\  \\       \\ \\  \\  __\\ \\  \\ \\  \\ \\  \\\\ \\  \\  ",
        "   \\/  /  /   \\ \\  \\\\\\  \\ \\  \\\\\\  \\       \\ \\  \\|\\  \\_\\  \\ \\  \\ \\  \\\\ \\  \\ ",
        " __/  / /      \\ \\_______\\ \\_______\\       \\ \\____________\\ \\__\\ \\__\\\\ \\__\\",
        "|\\___/ /        \\|_______|\\|_______|        \\|____________|\\|__|\\|__| \\|__|",
        "\\|___|/                                                                   "
    };
    size_t width = 0;
    for (const auto& line : message) {
        if (line.length() > width)
            width = line.length();
    }

    std::string border = "╔" + std::string(width + 78, '=') + "╗";

    auto centerAlign = [](const std::string& text, size_t w) {
        size_t left_padding = (w - text.length()) / 2;
        size_t right_padding = w - text.length() - left_padding;
        return std::string(left_padding, ' ') + text + std::string(right_padding, ' ');
        };

    std::cout << "\n" << border << "\n";
    for (const auto& line : message) {
        std::cout << "|| " << centerAlign(line, width) << " ||\n";
    }
    std::cout << "╚" << std::string(width + 78, '═') + "╝\n\n";
}
