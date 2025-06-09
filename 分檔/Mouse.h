#ifndef MOUSE_H
#define MOUSE_H

#include "Point.h" // 包含 Point 類別

// --- Mouse 類別 ---
class Mouse {
private:
    Point position;
    int moveCount;

public:
    Mouse(Point start);

    Point getPosition() const;
    int getMoveCount() const;

    void move(Point newPos);
};

#endif // MOUSE_H