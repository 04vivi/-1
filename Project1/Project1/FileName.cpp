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
					cout << "@ "; // �ѹ���m
				else if (i == exit.x && j == exit.y)
					cout << "+ "; // �X�f��m
				else if (grid[i][j] == 1)
					cout << "# "; // ���
				else
					cout << ". "; // �i������
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
	const vector<string> DirNames = { "�e", "��", "��", "�k" };

public:
	Game(Maze maze, Point start, Point end) : maze(maze), mouse(start), end(end) {}

	int play() {
		// �C���}�l�e����
		cout << " �w��Ө�ѹ��g�c (OOP)���I" << endl;
		cout << "�ϥ���L����ѹ����X�g�c�I\n" << endl;
		cout << "�Ÿ������G" << endl;
		cout << "@ �G�ѹ��ثe��m" << endl;
		cout << "+ �G�g�c�X�f" << endl;
		cout << "# �G����A���ਫ" << endl;
		cout << ". �G�i�H������" << endl;
		cout << "\n�ާ@�覡 :" << endl;
		cout << "w(�e)�Bs(��)�Ba(��)�Bd(��)\n" << endl;

		while (true) {
			Point curr = mouse.getPosition();
			maze.display(curr, end);

			// ��ܥi����V
			cout << "\n�ثe��m: (" << curr.x << ", " << curr.y << ")�A�i����V: ";
			bool hasDir = false;
			for (int i = 0; i < 4; ++i) {
				Point next = { curr.x + Directions[i].x, curr.y + Directions[i].y };
				if (maze.isValid(next)) {
					cout << DirNames[i] << "(" << DirKeys[i] << ") ";
					hasDir = true;
				}
			}
			if (!hasDir) cout << "�L�]�����^";
			cout << endl;

			// ���a��J��V
			cout << "�п�J��V�G";
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
				cout << "�L�Ŀ�J�A�п�J w/s/a/d�C\n" << endl;
				continue;
			}

			Point next = { curr.x + Directions[DirIndex].x, curr.y + Directions[DirIndex].y };
			if (!maze.isValid(next)) {
				cout << "? ����O����A���ਫ�I\n" << endl;
				continue;
			}

			mouse.move(next);

			if (next == end) {
				cout << "\n?? ���\���X�g�c�I" << endl;
				cout << "�`���ʦ���: " << mouse.getMoveCount() << endl;
				cout << "���L�����u: ";
				for (auto p : mouse.getPath()) {
					cout << "(" << p.x << "," << p.y << ") ";
				}
				cout << endl;
				return mouse.getMoveCount(); // ��^�����d�����ʦ���
			}

			cout << endl;
		}
	}
};

// ����1�G5x5 �g�c
vector<vector<int>> mazeMap1 = {
{1, 1, 1, 1, 1},
{1, 0, 0, 0, 1},
{1, 1, 1, 0, 1},
{1, 0, 0, 0, 1},
{1, 1, 1, 1, 1}
};
Point start1(1, 1);
Point end1(3, 3);

// ��s�G����2�G7x7 �g�c
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

// ��s�G����3�G10x10 �g�c
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

	// ����1�G5x5 �g�c
	Maze maze1(mazeMap1);
	Game game1(maze1, start1, end1);
	cout << "?? ����1:5x5 �g�c ??" << endl;
	totalMoves += game1.play();

	// ����2�G7x7 �g�c
	Maze maze2(mazeMap2);
	Game game2(maze2, start2, end2);
	cout << "\n?? ����2:7x7 �g�c ??" << endl;
	totalMoves += game2.play();

	// ����3�G10x10 �g�c
	Maze maze3(mazeMap3);
	Game game3(maze3, start3, end3);
	cout << "\n?? ����3:10x10 �g�c ??" << endl;
	totalMoves += game3.play();

	// ����`���ʦ���
	cout << "\n?? �����Ҧ������I" << endl;
	cout << "�`�@���ʤF " << totalMoves << " ���C" << endl;

	return 0;
}