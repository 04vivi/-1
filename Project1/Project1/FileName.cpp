#include <iostream>
#include <vector>
#include <string>

using namespace std;

class Point {
public:
	int x, y;
	Point(int x = 0, int y = 0) : x(x), y(y) {}
	bool operator==(const Point& other) const {
		return x == other.x && y == other.y;
	}
};

class Maze {
private:
	vector<vector<int> > grid;
public:
	Maze(vector<vector<int> > grid) : grid(grid) {}

	bool isWall(int x, int y) const {
		return grid[x][y] == 1;
	}

	bool isValid(Point p) const {
		int rows = grid.size();
		int cols = grid[0].size();
		return p.x >= 0 && p.x < rows && p.y >= 0 && p.y < cols && grid[p.x][p.y] == 0;
	}

	void display(const Point& mouse, const Point& exit) const {
		for (int i = 0; i < grid.size(); i++) {
			for (int j = 0; j < grid[i].size(); j++) {
				if (i == mouse.x && j == mouse.y)
					cout << "@ "; // 老鼠位置
				else if (i == exit.x && j == exit.y)
					cout << "+ "; // 出口位置
				else if (grid[i][j] == 1)
					cout << "# "; // 牆壁
				else
					cout << ". "; // 可走的路
			}
			cout << endl;
		}
	}
};

class Mouse {
private:
	Point position;
	vector<Point> path;
	int moveCount;
public:
	Mouse(Point start) : position(start), moveCount(0) {
		path.push_back(start);
	}

	Point getPosition() const {
		return position;
	}

	int getMoveCount() const {
		return moveCount;
	}

	const vector<Point>& getPath() const {
		return path;
	}

	void move(Point newPos) {
		position = newPos;
		path.push_back(newPos);
		moveCount++;
	}
};

class Game {
private:
	Maze maze;
	Mouse mouse;
	Point end;
	const vector<Point> Directions = { {-1, 0}, {1, 0}, {0, -1}, {0, 1} };
	const vector<char> DirKeys = { 'w', 's', 'a', 'd' };
	const vector<string> DirNames = { "前", "後", "左", "右" };

public:
	Game(Maze maze, Point start, Point end) : maze(maze), mouse(start), end(end) {}

	int play() {
		// 遊戲開始前說明
		cout << " 歡迎來到老鼠迷宮 (OOP)版！" << endl;
		cout << "使用鍵盤控制老鼠走出迷宮！\n" << endl;
		cout << "符號說明：" << endl;
		cout << "@ ：老鼠目前位置" << endl;
		cout << "+ ：迷宮出口" << endl;
		cout << "# ：牆壁，不能走" << endl;
		cout << ". ：可以走的路" << endl;
		cout << "\n操作方式 :" << endl;
		cout << "w(前)、s(後)、a(左)、d(左)\n" << endl;

		while (true) {
			Point curr = mouse.getPosition();
			maze.display(curr, end);

			// 顯示可走方向
			cout << "\n目前位置: (" << curr.x << ", " << curr.y << ")，可走方向: ";
			bool hasDir = false;
			for (int i = 0; i < 4; ++i) {
				Point next = { curr.x + Directions[i].x, curr.y + Directions[i].y };
				if (maze.isValid(next)) {
					cout << DirNames[i] << "(" << DirKeys[i] << ") ";
					hasDir = true;
				}
			}
			if (!hasDir) cout << "無（死路）";
			cout << endl;

			// 玩家輸入方向
			cout << "請輸入方向：";
			char input;
			cin >> input;

			int DirIndex = -1;
			for (int i = 0; i < 4; ++i) {
				if (input == DirKeys[i]) {
					DirIndex = i;
					break;
				}
			}

			if (DirIndex == -1) {
				cout << "無效輸入，請輸入 w/s/a/d。\n" << endl;
				continue;
			}

			Point next = { curr.x + Directions[DirIndex].x, curr.y + Directions[DirIndex].y };
			if (!maze.isValid(next)) {
				cout << "? 那邊是牆壁，不能走！\n" << endl;
				continue;
			}

			mouse.move(next);

			if (next == end) {
				cout << "\n?? 成功走出迷宮！" << endl;
				cout << "總移動次數: " << mouse.getMoveCount() << endl;
				cout << "走過的路線: ";
				for (auto p : mouse.getPath()) {
					cout << "(" << p.x << "," << p.y << ") ";
				}
				cout << endl;
				return mouse.getMoveCount(); // 返回本關卡的移動次數
			}

			cout << endl;
		}
	}
};

// 闖關1：5x5 迷宮
vector<vector<int>> mazeMap1 = {
{1, 1, 1, 1, 1},
{1, 0, 0, 0, 1},
{1, 1, 1, 0, 1},
{1, 0, 0, 0, 1},
{1, 1, 1, 1, 1}
};
Point start1(1, 1);
Point end1(3, 3);

// 更新：闖關2：7x7 迷宮
vector<vector<int> > mazeMap2 = {
{1, 1, 1, 1, 1, 1, 1},
{1, 0, 1, 0, 1, 0, 1},
{1, 0, 1, 0, 1, 0, 1},
{1, 0, 1, 0, 0, 0, 1},
{1, 0, 0, 0, 1, 0, 1},
{1, 1, 1, 0, 1, 0, 1},
{1, 1, 1, 1, 1, 1, 1}
};
Point start2(1, 1);
Point end2(5, 5);

// 更新：闖關3：10x10 迷宮
vector<vector<int> > mazeMap3 = {
{1, 1, 1, 1, 1, 1, 1, 1, 1, 1},
{1, 0, 1, 0, 0, 0, 0, 1, 0, 1},
{1, 0, 1, 0, 1, 1, 0, 1, 0, 1},
{1, 0, 0, 0, 1, 1, 0, 0, 0, 1},
{1, 1, 1, 0, 1, 0, 1, 1, 0, 1},
{1, 0, 0, 0, 1, 0, 0, 1, 0, 1},
{1, 0, 1, 0, 1, 1, 0, 1, 0, 1},
{1, 1, 1, 0, 1, 0, 0, 0, 1, 1},
{1, 0, 0, 0, 0, 0, 1, 0, 0, 1},
{1, 1, 1, 1, 1, 1, 1, 1, 1, 1}
};
Point start3(1, 1);
Point end3(8, 8);

int main() {
	int totalMoves = 0;

	// 闖關1：5x5 迷宮
	Maze maze1(mazeMap1);
	Game game1(maze1, start1, end1);
	cout << "?? 闖關1:5x5 迷宮 ??" << endl;
	totalMoves += game1.play();

	// 闖關2：7x7 迷宮
	Maze maze2(mazeMap2);
	Game game2(maze2, start2, end2);
	cout << "\n?? 闖關2:7x7 迷宮 ??" << endl;
	totalMoves += game2.play();

	// 闖關3：10x10 迷宮
	Maze maze3(mazeMap3);
	Game game3(maze3, start3, end3);
	cout << "\n?? 闖關3:10x10 迷宮 ??" << endl;
	totalMoves += game3.play();

	// 顯示總移動次數
	cout << "\n?? 完成所有闖關！" << endl;
	cout << "總共移動了 " << totalMoves << " 次。" << endl;

	return 0;
}