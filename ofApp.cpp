#include "ofApp.h"

//------------�̷� ������ �ʿ��� ������------------//
typedef struct _cell {
	bool left;
	bool up;
	bool right;
	bool down;		// ���� ���� ��� false
	bool visited;	// �湮�� ��� true
} cell;
cell mazeInfo[100][100];
char mazeTxt[100][100];

int dx[4] = { -1, 0, 1, 0 }, dy[4] = { 0, -1, 0, 1 }; // left up right down
int WIDTH, HEIGHT;
queue<pair<int, int> > q;

Player player;
Ant ant;





//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(255);
}

//--------------------------------------------------------------
void ofApp::update() {
	if (drawFlag && player.curX == ant.curX && player.curY == ant.curY) {
		gameEndFlag = true;
		drawFlag = false;
	}

	// 30�����ӿ� �ѹ��� ��ֹ��� �̵��Ѵ�.
	if (drawFlag && (ofGetFrameNum() % 30) == 0)
		ant.moveAnt();
} 

//--------------------------------------------------------------
void ofApp::draw() {
	ofTrueTypeFont myfont;
	myfont.load("arial.ttf", 15);
	ofSetColor(100);
	myfont.drawString("Enter n to generate new maze", 20, 20);

	if (drawFlag) {
		drawMaze();
	}

	/*if (gameEndFlag) {
		
	}
	*/
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'N' || key == 'n') {
		cout << "GENERATE NEW MAZE!\n";
		generateMaze();
		drawFlag = true;

		// �÷��̾��� �������� ���� ����̴�
		player.curX = 1;
		player.curY = 1;

		// ��ֹ��� �������� �������� ��������///////////////////////////////////
		ant.curX = WIDTH * 2 - 1;
		ant.curY = HEIGHT * 2 - 1;
	}

	if (key == 'Q' || key == 'q') {
		ofExit();
	}

	if (key == OF_KEY_DOWN && drawFlag) {
		visited[player.curY][player.curX] = true;
		if (mazeTxt[player.curY + 1][player.curX] == ' ')
			player.curY++;
	}
	if (key == OF_KEY_LEFT && drawFlag) {
		visited[player.curY][player.curX] = true;
		if (mazeTxt[player.curY][player.curX - 1] == ' ')
			player.curX--;
	}
	if (key == OF_KEY_RIGHT && drawFlag) {
		visited[player.curY][player.curX] = true;
		if (mazeTxt[player.curY][player.curX + 1] == ' ')
			player.curX++;
	}
	if (key == OF_KEY_UP && drawFlag) {
		visited[player.curY][player.curX] = true;
		if (mazeTxt[player.curY - 1][player.curX] == ' ')
			player.curY--;
	}


	/*if (key == 'j') {
		ofTrueTypeFont myfont;
		myfont.load("arial.ttf", 15);
		ofSetColor(100);
		myfont.drawString("Enter name : ", 200, 200);

		string arr;
		ofUTF8Insert(arr, 80, 1);

		myfont.drawString(arr, 400, 400);
	}*/
}

//--------------------------------------------------------------
void ofApp::keyReleased(int key) {

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button) {

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y) {

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h) {

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg) {

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo) {

}



//-----------�Լ� ����------------//
void ofApp::saveMazeToChar()
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


bool ofApp::check4Dir(int x, int y, bool v)
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

bool ofApp::check1Dir(int x, int y, int direction, bool v)
{
	if (x < 0 || x >= WIDTH || y < 0 || y >= HEIGHT || mazeInfo[y][x].visited == v)
		return false;
	return true;
}

void ofApp::breakWall(int x, int y, int dir)
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



void ofApp::walk()
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

bool ofApp::hunt()
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

void ofApp::generateMaze() {
	srand(time(NULL));

	do {
		cout << "Maze size must be smaller than 30 x 30\n";
		cout << "WIDTH : ";
		cin >> WIDTH;
		cout << "HEIGHT : ";
		cin >> HEIGHT;
	} while (WIDTH < 0 || WIDTH > 30 || HEIGHT < 0 || HEIGHT > 30);
	
	for (int i = 0; i < HEIGHT; i++) {
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
		while (!q.empty())	// ���̻� ������ �� ���� ������
			walk();
	} while (hunt());		// ���� �湮���� ���� ���� �ִ��� ã�´�

	saveMazeToChar();
}


//--------------�̷θ� �׸��� �Լ��̴�-------------//
void ofApp::drawMaze()
{
	for (int i = 0; i < HEIGHT * 2 + 1; i++) {
		for (int j = 0; j < WIDTH * 2 + 1; j++) {

			ofRectangle rect;
			rect.x = j * cellSize + margin;
			rect.y = i * cellSize + margin;
			rect.width = cellSize;
			rect.height = cellSize;

			// �׵θ�
			if (mazeTxt[i][j] == '+' ||
				mazeTxt[i][j] == '-' ||
				mazeTxt[i][j] == '|') {
				ofSetColor(0, 40, 80);
			}
			// ��
			else {
				ofSetColor(0, 100, 150);
			}
			ofDrawRectangle(rect);
		}
	}

	// �������� ��� ���� �׸���
	ofSetColor(255);
	ofDrawCircle((WIDTH + 1) * 2 * cellSize, (HEIGHT + 1) * 2 * cellSize, cellSize / 2);
	
	// ���� �������� ������ ���, �̷� �׸��� ����� gmaeEndFlag�� set�Ѵ�
	if (player.curX == WIDTH * 2 - 1 && player.curY == HEIGHT * 2 - 1) {
		gameEndFlag = true;
		ofSleepMillis(100);
		drawFlag = false;		// �̷ΰ� ���̻� ǥ�õ��� �ʵ��� �Ѵ�
	}

	
	player.drawPlayer(cellSize, margin);
	ant.drawAnt(cellSize, margin);
}


//--------------�÷��̾ �׸��� �Լ��̴�-------------//
void Player::drawPlayer(int size, int margin)
{
	ofRectangle rect;
	rect.x = player.curX * size + margin;
	rect.y = player.curY * size + margin;
	rect.width = size;
	rect.height = size;
	int fr = ofGetFrameNum();
	if (fr % 20 < 10)
		ofSetColor(230, 240, 80);
	else
		ofSetColor(140, 180, 110);
	ofDrawRectangle(rect);
}

//--------------��ֹ��� �׸��� �Լ��̴�-------------//
void Ant::drawAnt(int size, int margin)
{
	ofRectangle rect;
	rect.x = ant.curX * size + margin;
	rect.y = ant.curY * size + margin;
	rect.width = size;
	rect.height = size;
	int fr = ofGetFrameNum();
	if (fr % 20 < 10)
		ofSetColor(0, 230, 190);
	else
		ofSetColor(0, 150, 160);
	ofDrawRectangle(rect);
}

//--------------��ֹ��� �̵���Ű�� �Լ��̴�-------------//
void Ant::moveAnt()
{
	/*for (int i = 0; i < 4; i++) {
		if (mazeTxt[curY + dy[i]][curX + dx[i]] == ' ') {
			curX += dx[i];
			curY += dy[i];
			break;
		}
	}*/
	ant.dfs();
	curX = stack[1][0];
	curY = stack[1][1];
}

//--------------��ֹ��� �̵��� ��θ� ã�� �Լ��̴�-------------//
void Ant::dfs()
{
	//-------�ʱ�ȭ
	for (int i = 0; i < HEIGHT * 2 + 1; i++) {
		for (int j = 0; j < WIDTH * 2 + 1; j++) {
			mazeInfo[i][j].visited = false;
		}
	}
	int top = -1;
	//-------�ʱ�ȭ

	int maze_x = ant.curX, maze_y = ant.curY;

	stack[++top][0] = maze_x;
	stack[top][1] = maze_y;

	while (1) {
		maze_x = stack[top][0];
		maze_y = stack[top][1];
		mazeInfo[maze_y][maze_x].visited = true;

		if (maze_x == player.curX && maze_y == player.curY)
			break;

		bool endOfPath = true;

		for (int i = 0; i < 4; i++) {
			if (mazeInfo[maze_y + dy[i]][maze_x + dx[i]].visited)
				continue;
			if (mazeTxt[maze_y + dy[i]][maze_x + dx[i]] != ' ')
				continue;

			stack[++top][0] = maze_x + dx[i];
			stack[top][1] = maze_y + dy[i];
			endOfPath = false;
			break;
		}

		if (endOfPath)
			top--;
	};
}