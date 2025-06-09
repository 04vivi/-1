#include "Utility.h"
#include <string>

// --- �̲� ASCII �ӧQ�e�� ---
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

    std::string border = "��" + std::string(width + 78, '��') + "��";

    auto centerAlign = [](const std::string& text, size_t w) {
        size_t left_padding = (w - text.length()) / 2;
        size_t right_padding = w - text.length() - left_padding;
        return std::string(left_padding, ' ') + text + std::string(right_padding, ' ');
        };

    std::cout << "\n" << border << "\n";
    for (const auto& line : message) {
        std::cout << "�� " << centerAlign(line, width) << " ��\n";
    }
    std::cout << "��" << std::string(width + 78, '��') + "��\n\n";
}