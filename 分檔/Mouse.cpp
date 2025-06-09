#include "Mouse.h"

Mouse::Mouse(Point start) : position(start), moveCount(0) {}

Point Mouse::getPosition() const { return position; }
int Mouse::getMoveCount() const { return moveCount; }

void Mouse::move(Point newPos) {
    position = newPos;
    moveCount++;
}