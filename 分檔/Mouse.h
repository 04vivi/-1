#ifndef MOUSE_H
#define MOUSE_H

#include "Point.h" // �]�t Point ���O

// --- Mouse ���O ---
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