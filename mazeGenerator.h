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
cell **mazeInfo;
char mazeTxt[500][500];

int dx[4] = { -1, 0, 1, 0 }, dy[4] = { 0, -1, 0, 1 }; // left up right down
int WIDTH, HEIGHT; // n : ���� m : ����
queue<pair<int, int> > q;

#endif


//------------�̷� ������ �ʿ��� �Լ���------------//
void printMazeToFile();				// ��� Ȯ�ο� �Լ��̴�
void printMazeToScreen();			// ��� Ȯ�ο� �Լ��̴�
void saveMazeToChar();				// saveMazeToCharWithNum�� ���� �����̴�
char getDirNum();					// saveMazeToCharWithNum���� ������ ���ڸ� �ҷ����� �Լ��̴�
void saveMazeToCharWithNum();


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

	///////////////////////////////////// ****queue�� push�� �Ͼ�°�****
	q.push(make_pair(x, y));
}



void walk()
{
	int x = q.front().first;
	int y = q.front().second;
	/////////////////////////////////////****queue�� pop�� �Ͼ�°�****
	q.pop();
	mazeInfo[y][x].visited = true;

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
	for (int y = 0; y < HEIGHT; y++) {
		for (int x = 0; x < WIDTH; x++) {
			if (!mazeInfo[y][x].visited && check4Dir(x, y, false)) {
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
	// �̷� ������ ������ ������ �������� �����Ѵ�
	q.push(make_pair(startX, startY));

	do {
		while (!q.empty()) // ���̻� ������ �� ���� ������
			walk();
	} while (hunt());		// ���� �湮���� ���� ���� �ִ��� ã�´�


	//printMazeToFile();
	//printMazeToScreen();
	//saveMazeToChar();
	saveMazeToCharWithNum();
}