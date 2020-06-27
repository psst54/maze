#include "ofApp.h"

//------------미로 생성에 필요한 변수들------------//
typedef struct _cell {
	bool left;
	bool up;
	bool right;
	bool down;		// 벽이 없는 경우 false
	bool visited;	// 방문한 경우 true
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

	// 30프레임에 한번씩 장애물을 이동한다.
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

		// 플레이어의 시작점은 좌측 상단이다
		player.curX = 1;
		player.curY = 1;

		// 장애물의 시작점은 랜덤으로 정해진다///////////////////////////////////
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



//-----------함수 선언------------//
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

	///////////////////////////////////// ****queue의 push가 일어나는곳****
	q.push(make_pair(x, y));
}



void ofApp::walk()
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

bool ofApp::hunt()
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
	// 미로 생성을 시작할 지점을 랜덤으로 지정한다
	q.push(make_pair(startX, startY));

	do {
		while (!q.empty())	// 더이상 움질일 수 없을 때까지
			walk();
	} while (hunt());		// 아직 방문하지 않은 곳이 있는지 찾는다

	saveMazeToChar();
}


//--------------미로를 그리는 함수이다-------------//
void ofApp::drawMaze()
{
	for (int i = 0; i < HEIGHT * 2 + 1; i++) {
		for (int j = 0; j < WIDTH * 2 + 1; j++) {

			ofRectangle rect;
			rect.x = j * cellSize + margin;
			rect.y = i * cellSize + margin;
			rect.width = cellSize;
			rect.height = cellSize;

			// 테두리
			if (mazeTxt[i][j] == '+' ||
				mazeTxt[i][j] == '-' ||
				mazeTxt[i][j] == '|') {
				ofSetColor(0, 40, 80);
			}
			// 길
			else {
				ofSetColor(0, 100, 150);
			}
			ofDrawRectangle(rect);
		}
	}

	// 도착점에 흰색 원을 그린다
	ofSetColor(255);
	ofDrawCircle((WIDTH + 1) * 2 * cellSize, (HEIGHT + 1) * 2 * cellSize, cellSize / 2);
	
	// 만일 도착점에 도착한 경우, 미로 그림을 지우고 gmaeEndFlag를 set한다
	if (player.curX == WIDTH * 2 - 1 && player.curY == HEIGHT * 2 - 1) {
		gameEndFlag = true;
		ofSleepMillis(100);
		drawFlag = false;		// 미로가 더이상 표시되지 않도록 한다
	}

	
	player.drawPlayer(cellSize, margin);
	ant.drawAnt(cellSize, margin);
}


//--------------플레이어를 그리는 함수이다-------------//
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

//--------------장애물을 그리는 함수이다-------------//
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

//--------------장애물을 이동시키는 함수이다-------------//
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

//--------------장애물이 이동할 경로를 찾는 함수이다-------------//
void Ant::dfs()
{
	//-------초기화
	for (int i = 0; i < HEIGHT * 2 + 1; i++) {
		for (int j = 0; j < WIDTH * 2 + 1; j++) {
			mazeInfo[i][j].visited = false;
		}
	}
	int top = -1;
	//-------초기화

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