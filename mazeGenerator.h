#include <iostream>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <fstream>
using namespace std;

#ifndef __MAZEGENERATOR
#define __MAZEGENERATOR

//------------�̷� ������ �ʿ��� ������------------//
typedef struct _cell {
	bool left;
	bool up;
	bool right;
	bool down;		// ���� ���� ��� false
	bool visited;	// �湮�� ��� true
} cell;
cell **arr;

int dx[4] = { -1, 0, 1, 0 }, dy[4] = { 0, -1, 0, 1 }; // left up right down
int n, m; // n : ���� m : ����
queue<pair<int, int> > q;

#endif


//------------�̷� ������ �ʿ��� �Լ���------------//
void printMazeToFile();
void printMazeToScreen();

bool check4Dir(int x, int y, bool v);
bool check1Dir(int x, int y, int direction, bool v);

void breakWall(int x, int y, int dir);
void walk();
bool hunt();

void generateMaze();


//-----------�Լ� ����------------//
void printMazeToFile()
{
	ofstream writeFile;
	writeFile.open("output.maz");
	if (!writeFile.is_open()) {
		printf("error\n");
		exit(1);
	}
	else {
		writeFile << "+";
		for (int i = 0; i < n; i++) {
			writeFile << "-+";
		}
		writeFile << "\n";
		for (int i = 0; i < m; i++) {
			writeFile << "|";
			for (int j = 0; j < n; j++) {
				if (arr[i][j].right)
					writeFile << " |";
				else
					writeFile << "  ";
			}

			writeFile << "\n";

			writeFile << "+";
			for (int j = 0; j < n; j++) {
				if (arr[i][j].down)
					writeFile << "-+";
				else
					writeFile << " +";
			}

			writeFile << "\n";
		}
		writeFile << "\n";
	}
	writeFile.close();
}

void printMazeToScreen()
{
	printf("+");
	for (int i = 0; i < n; i++) {
		printf("-+");
	}
	printf("\n");
	for (int i = 0; i < m; i++) {
		printf("|");
		for (int j = 0; j < n; j++) {
			if (arr[i][j].right)
				printf(" |");
			else
				printf("  ");
		}

		printf("\n");

		printf("+");
		for (int j = 0; j < n; j++) {
			if (arr[i][j].down)
				printf("-+");
			else
				printf(" +");
		}

		printf("\n");
	}
	printf("\n");
}



bool check4Dir(int x, int y, bool v)
{
	int chk = 0;
	for (int i = 0; i < 4; i++) {
		if (x + dx[i] < 0 || x + dx[i] >= n
			|| y + dy[i] < 0 || y + dy[i] >= m
			|| arr[y + dy[i]][x + dx[i]].visited == v)
			continue;
		chk++;
	}
	if (chk)
		return true;
	return false;
}

bool check1Dir(int x, int y, int direction, bool v)
{
	if (x < 0 || x >= n || y < 0 || y >= m || arr[y][x].visited == v)
		return false;
	return true;
}

void breakWall(int x, int y, int dir)
{
	arr[y][x].visited = true;
	if (dir == 0) {
		arr[y][x].left = false;
		x += dx[dir];
		y += dy[dir];
		arr[y][x].right = false;
	}
	else if (dir == 1) {
		arr[y][x].up = false;
		x += dx[dir];
		y += dy[dir];
		arr[y][x].down = false;
	}
	else if (dir == 2) {
		arr[y][x].right = false;
		x += dx[dir];
		y += dy[dir];
		arr[y][x].left = false;
	}
	else if (dir == 3) {
		arr[y][x].down = false;
		x += dx[dir];
		y += dy[dir];
		arr[y][x].up = false;
	}
	arr[y][x].visited = true;

	///////////////////////////////////// ****queue�� push�� �Ͼ�°�****
	q.push(make_pair(x, y));
}



void walk()
{
	int x = q.front().first;
	int y = q.front().second;
	/////////////////////////////////////****queue�� pop�� �Ͼ�°�****
	q.pop();
	arr[y][x].visited = true;

	if (!check4Dir(x, y, true))	// ���̻� ������ �� ���ٸ� �Լ��� ��ģ��
		return;

	int dir = rand() % 4;
	while (!check1Dir(x + dx[dir], y + dy[dir], dir, true)) // ������ �� ���°�� �����
		dir = rand() % 4;

	breakWall(x, y, dir);
}

bool hunt()
{
	/*
	���� �湮���� �ʰ� && ��ó�� �湮�� ���� �ִ� ĭ�� ã�´�.
	*/
	for (int y = 0; y < m; y++) {
		for (int x = 0; x < n; x++) {
			if (!arr[y][x].visited && check4Dir(x, y, false)) {
				int dir = rand() % 4;
				while (!check1Dir(x + dx[dir], y + dy[dir], dir, false)) // ������ �� ���°�� �����
					dir = rand() % 4;

				breakWall(x, y, dir);

				return true;
			}
		}
	}
	return false;
}

void generateMaze() {
	srand(time(NULL));

	cout << "HEIGHT : ";
	cin >> n;
	cout << "WIDTH : ";
	cin >> m;

	arr = (cell**)malloc(sizeof(cell*) * m);
	for (int i = 0; i < m; i++) {
		arr[i] = (cell*)malloc(sizeof(cell) * n);
		for (int j = 0; j < n; j++) {
			arr[i][j].left = true;
			arr[i][j].up = true;
			arr[i][j].right = true;
			arr[i][j].down = true;
			arr[i][j].visited = false;
		}
	}


	int startX = rand() % n;
	int startY = rand() % m;
	// �̷� ������ ������ ������ �������� �����Ѵ�
	q.push(make_pair(startX, startY));

	do {
		while (!q.empty()) // ���̻� ������ �� ���� ������
			walk();
	} while (hunt());		// ���� �湮���� ���� ���� �ִ��� ã�´�


	printMazeToFile();
	printMazeToScreen();
}