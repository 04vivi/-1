#include "Utility.h"
#include <string>

// --- ³Ì²× ASCII ³Ó§Qµe­± ---
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

    std::string border = "ùÝ" + std::string(width + 78, 'ùù') + "ùß";

    auto centerAlign = [](const std::string& text, size_t w) {
        size_t left_padding = (w - text.length()) / 2;
        size_t right_padding = w - text.length() - left_padding;
        return std::string(left_padding, ' ') + text + std::string(right_padding, ' ');
        };

    std::cout << "\n" << border << "\n";
    for (const auto& line : message) {
        std::cout << "ùø " << centerAlign(line, width) << " ùø\n";
    }
    std::cout << "ùã" << std::string(width + 78, 'ùù') + "ùå\n\n";
}