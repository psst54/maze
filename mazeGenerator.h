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
cell **mazeInfo;
char mazeTxt[500][500];

int dx[4] = { -1, 0, 1, 0 }, dy[4] = { 0, -1, 0, 1 }; // left up right down
int WIDTH, HEIGHT; // n : 가로 m : 세로
queue<pair<int, int> > q;

#endif


//------------미로 생성에 필요한 함수들------------//
void printMazeToFile();				// 출력 확인용 함수이다
void printMazeToScreen();			// 출력 확인용 함수이다
void saveMazeToChar();				// saveMazeToCharWithNum의 이전 버전이다
char getDirNum();					// saveMazeToCharWithNum에서 저장할 숫자를 불러오는 함수이다
void saveMazeToCharWithNum();


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
		for (int i = 0; i < WIDTH; i++) {
			writeFile << "-+";
		}
		writeFile << "\n";
		for (int i = 0; i < HEIGHT; i++) {
			writeFile << "|";
			for (int j = 0; j < WIDTH; j++) {
				if (mazeInfo[i][j].right)
					writeFile << " |";
				else
					writeFile << "  ";
			}

			writeFile << "\n";

			writeFile << "+";
			for (int j = 0; j < WIDTH; j++) {
				if (mazeInfo[i][j].down)
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
	for (int i = 0; i < WIDTH; i++) {
		printf("-+");
	}
	printf("\n");
	for (int i = 0; i < HEIGHT; i++) {
		printf("|");
		for (int j = 0; j < WIDTH; j++) {
			if (mazeInfo[i][j].right)
				printf(" |");
			else
				printf("  ");
		}

		printf("\n");

		printf("+");
		for (int j = 0; j < WIDTH; j++) {
			if (mazeInfo[i][j].down)
				printf("-+");
			else
				printf(" +");
		}

		printf("\n");
	}
	printf("\n");
}

void saveMazeToChar()
{
	mazeTxt[0][0] = '+';
	for (int i = 1; i <= WIDTH; i++) {
		mazeTxt[0][i * 2 - 1] = '-';
		mazeTxt[0][i * 2] = '+';
	}
	
	for (int i = 1; i <= HEIGHT; i++) {

		mazeTxt[i * 2 - 1][0] = '|';
		for (int j = 1; j <= WIDTH; j++) {
			mazeTxt[i * 2 - 1][j * 2 - 1] = ' ';

			if (mazeInfo[i - 1][j - 1].right)
				mazeTxt[i * 2 - 1][j * 2] = '|';
			else
				mazeTxt[i * 2 - 1][j * 2] = ' ';
		}

		mazeTxt[i * 2][0] = '+';
		for (int j = 1; j <= WIDTH; j++) {
			mazeTxt[i * 2][j * 2] = '+';

			if (mazeInfo[i - 1][j - 1].down)
				mazeTxt[i * 2][j * 2 - 1] = '-';
			else
				mazeTxt[i * 2][j * 2 - 1] = ' ';
		}
	}


	/*printf("\n\n");
	for (int i = 0; i < HEIGHT * 2 + 1; i++) {
		for (int j = 0; j < WIDTH * 2 + 1; j++) {
			printf("%c", mazeTxt[i][j]);
		}
		printf("\n");
	}*/
}

char getDirNum(int tX, int tY)
{
	int dx[8] = { -1, -1, -1, 0, 0, 1, 1, 1 },
		dy[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	char cnt = 0;

	for (int i = 0; i < 8; i++) {
		if (mazeTxt[tY + dy[i]][tX + dx[i]] != '|' && 
			mazeTxt[tY + dy[i]][tX + dx[i]] != '-' && 
			mazeTxt[tY + dy[i]][tX + dx[i]] != '+')
			cnt++;
	}

	return cnt;
}

void saveMazeToCharWithNum()
{
	mazeTxt[0][0] = '+';
	for (int i = 1; i <= WIDTH; i++) {
		mazeTxt[0][i * 2 - 1] = '-';
		mazeTxt[0][i * 2] = '+';
	}

	for (int i = 1; i <= HEIGHT; i++) {

		mazeTxt[i * 2 - 1][0] = '|';
		for (int j = 1; j <= WIDTH; j++) {
			mazeTxt[i * 2 - 1][j * 2 - 1] = getDirNum(j * 2 - 1, i * 2 - 1) + 48;

			if (mazeInfo[i - 1][j - 1].right)
				mazeTxt[i * 2 - 1][j * 2] = '|';
			else
				mazeTxt[i * 2 - 1][j * 2] = getDirNum(j * 2, i * 2 - 1) + 48;
		}

		mazeTxt[i * 2][0] = '+';
		for (int j = 1; j <= WIDTH; j++) {
			mazeTxt[i * 2][j * 2] = '+';

			if (mazeInfo[i - 1][j - 1].down)
				mazeTxt[i * 2][j * 2 - 1] = '-';
			else
				mazeTxt[i * 2][j * 2 - 1] = getDirNum(j * 2 - 1, i * 2) + 48;
		}
	}


	/*printf("\n\n");
	for (int i = 0; i < HEIGHT * 2 + 1; i++) {
		for (int j = 0; j < WIDTH * 2 + 1; j++) {
			printf("%c", mazeTxt[i][j]);
		}
		printf("\n");
	}*/
}



bool check4Dir(int x, int y, bool v)
{
	int chk = 0;
	for (int i = 0; i < 4; i++) {
		if (x + dx[i] < 0 || x + dx[i] >= WIDTH
			|| y + dy[i] < 0 || y + dy[i] >= HEIGHT
			|| mazeInfo[y + dy[i]][x + dx[i]].visited == v)
			continue;
		chk++;
	}
	if (chk)
		return true;
	return false;
}

bool check1Dir(int x, int y, int direction, bool v)
{
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT || mazeInfo[y][x].visited == v)
		return false;
	return true;
}

void breakWall(int x, int y, int dir)
{
	mazeInfo[y][x].visited = true;
	if (dir == 0) {
		mazeInfo[y][x].left = false;
		x += dx[dir];
		y += dy[dir];
		mazeInfo[y][x].right = false;
	}
	else if (dir == 1) {
		mazeInfo[y][x].up = false;
		x += dx[dir];
		y += dy[dir];
		mazeInfo[y][x].down = false;
	}
	else if (dir == 2) {
		mazeInfo[y][x].right = false;
		x += dx[dir];
		y += dy[dir];
		mazeInfo[y][x].left = false;
	}
	else if (dir == 3) {
		mazeInfo[y][x].down = false;
		x += dx[dir];
		y += dy[dir];
		mazeInfo[y][x].up = false;
	}
	mazeInfo[y][x].visited = true;

	///////////////////////////////////// ****queue의 push가 일어나는곳****
	q.push(make_pair(x, y));
}



void walk()
{
	int x = q.front().first;
	int y = q.front().second;
	/////////////////////////////////////****queue의 pop이 일어나는곳****
	q.pop();
	mazeInfo[y][x].visited = true;

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
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (!mazeInfo[y][x].visited && check4Dir(x, y, false)) {
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

	cout << "WIDTH : ";
	cin >> WIDTH;
	cout << "HEIGHT : ";
	cin >> HEIGHT;

	mazeInfo = (cell**)malloc(sizeof(cell*) * HEIGHT);
	for (int i = 0; i < HEIGHT; i++) {
		mazeInfo[i] = (cell*)malloc(sizeof(cell) * WIDTH);
		for (int j = 0; j < WIDTH; j++) {
			mazeInfo[i][j].left = true;
			mazeInfo[i][j].up = true;
			mazeInfo[i][j].right = true;
			mazeInfo[i][j].down = true;
			mazeInfo[i][j].visited = false;
		}
	}


	int startX = rand() % WIDTH;
	int startY = rand() % HEIGHT;
	// 미로 생성을 시작할 지점을 랜덤으로 지정한다
	q.push(make_pair(startX, startY));

	do {
		while (!q.empty()) // 더이상 움질일 수 없을 때까지
			walk();
	} while (hunt());		// 아직 방문하지 않은 곳이 있는지 찾는다


	//printMazeToFile();
	//printMazeToScreen();
	//saveMazeToChar();
	saveMazeToCharWithNum();
}