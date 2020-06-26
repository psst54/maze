#include <iostream>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <fstream>
using namespace std;

#ifndef __MAZEGENERATOR
#define __MAZEGENERATOR

//------------미로 생성에 필요한 변수들------------//
typedef struct _cell {
	bool left;
	bool up;
	bool right;
	bool down;		// 벽이 없는 경우 false
	bool visited;	// 방문한 경우 true
} cell;
cell **arr;

int dx[4] = { -1, 0, 1, 0 }, dy[4] = { 0, -1, 0, 1 }; // left up right down
int n, m; // n : 가로 m : 세로
queue<pair<int, int> > q;

#endif


//------------미로 생성에 필요한 함수들------------//
void printMazeToFile();
void printMazeToScreen();

bool check4Dir(int x, int y, bool v);
bool check1Dir(int x, int y, int direction, bool v);

void breakWall(int x, int y, int dir);
void walk();
bool hunt();

void generateMaze();


//-----------함수 선언------------//
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

	///////////////////////////////////// ****queue의 push가 일어나는곳****
	q.push(make_pair(x, y));
}



void walk()
{
	int x = q.front().first;
	int y = q.front().second;
	/////////////////////////////////////****queue의 pop이 일어나는곳****
	q.pop();
	arr[y][x].visited = true;

	if (!check4Dir(x, y, true))	// 더이상 움직일 수 없다면 함수를 마친다
		return;

	int dir = rand() % 4;
	while (!check1Dir(x + dx[dir], y + dy[dir], dir, true)) // 움질일 수 없는경우 재생성
		dir = rand() % 4;

	breakWall(x, y, dir);
}

bool hunt()
{
	/*
	아직 방문하지 않고 && 근처에 방문한 적이 있는 칸을 찾는다.
	*/
	for (int y = 0; y < m; y++) {
		for (int x = 0; x < n; x++) {
			if (!arr[y][x].visited && check4Dir(x, y, false)) {
				int dir = rand() % 4;
				while (!check1Dir(x + dx[dir], y + dy[dir], dir, false)) // 움질일 수 없는경우 재생성
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
	// 미로 생성을 시작할 지점을 랜덤으로 지정한다
	q.push(make_pair(startX, startY));

	do {
		while (!q.empty()) // 더이상 움질일 수 없을 때까지
			walk();
	} while (hunt());		// 아직 방문하지 않은 곳이 있는지 찾는다


	printMazeToFile();
	printMazeToScreen();
}