#ifndef POINT_H
#define POINT_H

// --- Point Ãþ§O ---
class Point {
public:
    int x, y;
    Point(int x = 0, int y = 0);

    bool operator==(const Point& other) const;
    bool operator!=(const Point& other) const;
};

#endif // POINT_H
