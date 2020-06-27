#include "ofApp.h"

//------------미로 생성에 필요한 변수들------------//
typedef struct _cell {
	bool left;
	bool up;
	bool right;
	bool down;		
	// 벽이 없는 경우 false
	bool visited;	
	// 방문한 경우 true
	int num;
} cell;

// 미로 생성에 필요한 구조체이다. 각 칸의 상하좌우에 벽이 있는지, 방문한 칸인지를 저장한다
cell mazeInfo[30][30];	
// mazeInfo를 통해 생성된 미로를 char형으로 그림처럼 저장하는 배열이다
char mazeTxt[70][70];

// 각각 left up right down 방향으로 이동하는 경우를 dx, dy에 저장하였다
int dx[4] = { -1, 0, 1, 0 }, dy[4] = { 0, -1, 0, 1 }; 
//미로의 가로, 세로 칸 수이다
int WIDTH, HEIGHT;
// 미로 생성, bfs에서 사용되는 큐이다
queue<pair<int, int> > q;


Player player;		// player를 생성한다.
Ant ant;			// 장애물을 생성한다.



//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(255);	//배경은 흰색으로 지정한다
}

//--------------------------------------------------------------
void ofApp::update() {
	// 도착점에 도착한 경우, 미로 그림을 지우고 gmaeEndFlag를 set한다
	if (drawFlag && player.curX == WIDTH * 2 - 1 && player.curY == HEIGHT * 2 - 1) {
		ofSleepMillis(500);		// 미로를 지우기 전에 잠시 멈춘다
		gameEndFlag = true;
		drawFlag = false;
	}

	// 장애물과 마주친 경우, 미로 그림을 지우고 gmaeEndFlag를 set한다
	if (drawFlag && player.curX == ant.curX && player.curY == ant.curY) {
		ofSleepMillis(500);		// 미로를 지우기 전에 잠시 멈춘다
		gameEndFlag = true;
		drawFlag = false;
	}

	// 30프레임에 한번씩 장애물을 이동한다.
	if (drawFlag && (ofGetFrameNum() % 10) == 0)
		ant.moveAnt();
} 

//--------------------------------------------------------------
void ofApp::draw() {
	ofTrueTypeFont myfont;
	myfont.load("arial.ttf", 15);
	ofSetColor(100);
	myfont.drawString("Enter n to generate new maze", 20, 20);

	// 미로가 생성된 경우 drawFlag가 true이고, 그 경우에 미로를 그린다
	if (drawFlag) {
		drawMaze();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	// n을 누르면 새 미로를 만들 수 있다
	if (key == 'N' || key == 'n') {
		cout << "GENERATE NEW MAZE!\n";
		
		//미로 생성 함수를 호출한다
		generateMaze();
		
		// 플레이어의 시작점은 좌측 상단이다
		player.curX = 1;
		player.curY = 1;

		// 장애물의 시작점은 랜덤으로 정해진다
		do {
			ant.curX = (rand() % WIDTH) * 2 - 1;
		} while (ant.curX <= 1); // 시작점과 너무 가깝지 않도록 조정한다

		do {
			ant.curY = (rand() % HEIGHT) * 2 - 1;
		} while (ant.curY <= 1); // 시작점과 너무 가깝지 않도록 조정한다

		drawFlag = true;
	}

	//Q를 누르면 프로그램을 종료한다
	if (key == 'Q' || key == 'q') {
		ofExit();
	}

	// 방향키를 통해 플레이어를 움직일 수 있다
	if (key == OF_KEY_DOWN && drawFlag) {
		if (mazeTxt[player.curY + 1][player.curX] == ' ')
			player.curY++;
	}
	if (key == OF_KEY_LEFT && drawFlag) {
		if (mazeTxt[player.curY][player.curX - 1] == ' ')
			player.curX--;
	}
	if (key == OF_KEY_RIGHT && drawFlag) {
		if (mazeTxt[player.curY][player.curX + 1] == ' ')
			player.curX++;
	}
	if (key == OF_KEY_UP && drawFlag) {
		if (mazeTxt[player.curY - 1][player.curX] == ' ')
			player.curY--;
	}

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

	while (!check4Dir(x, y, true)) {// 더이상 움직일 수 없다면 함수를 마친다
		if (x + dx[0] >= 0 && mazeInfo[y][x].left) {// 왼쪽 방향으로 루프를 만들 수 있다면 그렇게 한다
			breakWall(x, y, 0);
			return;
		}
		if (y + dy[1] >= 0 && mazeInfo[y][x].up) {// 위쪽 방향으로 루프를 만들 수 있다면 그렇게 한다
			breakWall(x, y, 1);
			return;
		}
		if (x + dx[2] < WIDTH && mazeInfo[y][x].right) {// 오른쪽 방향으로 루프를 만들 수 있다면 그렇게 한다
			breakWall(x, y, 2);
			return;
		}
		if (y + dy[3] < HEIGHT && mazeInfo[y][x].down) {// 아래쪽 방향으로 루프를 만들 수 있다면 그렇게 한다
			breakWall(x, y, 3);
			return;
		}
		return;
	}

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
		cout << "3 <= WIDTH <= 30, 3 <= HEIGHT <= 30\n";
		cout << "WIDTH : ";
		cin >> WIDTH;
		cout << "HEIGHT : ";
		cin >> HEIGHT;
	} while (WIDTH < 3 || WIDTH > 30 || HEIGHT < 3 || HEIGHT > 30);
	
	for (int i = 0; i < HEIGHT; i++) {
		for (int j = 0; j < WIDTH; j++) {
			mazeInfo[i][j].left = true;
			mazeInfo[i][j].up = true;
			mazeInfo[i][j].right = true;
			mazeInfo[i][j].down = true;
			mazeInfo[i][j].visited = false;
		}
	}

	// 시작 지점은 임의로 0, 0으로 둔다
	int startX = 0;
	int startY = 0;
	q.push(make_pair(startX, startY));

	do {
		while (!q.empty())	// 더이상 움질일 수 없을 때까지
			walk();
	} while (hunt());		// 아직 방문하지 않은 곳이 있는지 찾는다

	// 생성된 미로의 전체 그림을 char형으로 저장한다
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
	
	player.drawPlayer(cellSize, margin);
	ant.drawAnt(cellSize, margin);
}


//--------------플레이어를 그리는 함수이다-------------//
void Player::drawPlayer(int size, int margin)
{
	ofRectangle rect;
	// 플레이어의 x좌표는 curX * size(미로 한 칸의 사이즈) + margin(왼쪽 여백)이 된다.
	rect.x = player.curX * size + margin;	
	// 플레이어의 y좌표는 curY * size(미로 한 칸의 사이즈) + margin(위쪽 여백)이 된다.
	rect.y = player.curY * size + margin;
	// 한 칸의 크기는 size * size이다
	rect.width = size;
	rect.height = size;

	// 깜박이는 효과를 위해 frame 번호가 5번 바뀔때마다 색을 바꿔준다
	int fr = ofGetFrameNum();
	if (fr % 20 < 10)
		ofSetColor(230, 240, 80);
	else
		ofSetColor(140, 180, 110);

	// 위에서 지정된 좌표, 사이즈, 색상을 토대로 플레이어를 그린다
	ofDrawRectangle(rect);
}

//--------------장애물을 그리는 함수이다-------------//
void Ant::drawAnt(int size, int margin)
{
	// drawPlayer에서 색만 바꾼 함수이다
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
	// 장애물은 bfs의 결과값을 이용해 플레이어 방향으로 이동한다
	ant.bfs();

	// bfs 함수에서 기록해놓은 방문 순서를 거꾸로 따라가면서 다음 칸을 결정한다
	int nextX = player.curX, nextY = player.curY;
	while (1) {
		for (int i = 0; i < 4; i++) {
			if (mazeTxt[nextY + dy[i]][nextX + dx[i]] != ' ')
				continue;
			if (visitNum[nextY + dy[i]][nextX + dx[i]] != visitNum[nextY][nextX] - 1)
				continue;

			nextX += dx[i];
			nextY += dy[i];
			break;
		}

		if (visitNum[nextY][nextX] == 1)
			break;
	}
	curX = nextX;
	curY = nextY;
	
}

//--------------장애물이 이동할 경로를 찾는 함수이다-------------//
void Ant::bfs()
{
	//-------큐와 미로의 방문 정보를 초기화한다
	for (int i = 0; i < HEIGHT * 2 + 1; i++) {
		for (int j = 0; j < WIDTH * 2 + 1; j++) {
			visitNum[i][j] = 0;
		}
	}
	while (!q.empty())
		q.pop();
	//-------------------------------------

	int maze_x = ant.curX, maze_y = ant.curY;
	q.push(make_pair(maze_x, maze_y));

	while (1) {
		maze_x = q.front().first;
		maze_y = q.front().second;
		q.pop();

		// 플레이어의 위치까지 도달했다면 bfs를 마친다
		if (maze_x == player.curX && maze_y == player.curY)
			break;

		for (int i = 0; i < 4; i++) {
			// 다음 칸이 이미 방문한 칸이라면 두 번 밟지 않도록 한다
			if (visitNum[maze_y + dy[i]][maze_x + dx[i]])
				continue;
			// 다음 칸이 벽이라면 다른 방향을 찾는다
			if (mazeTxt[maze_y + dy[i]][maze_x + dx[i]] != ' ')
				continue;

			// 다음 칸의 visitNum값은 현재 칸의 visitNum보다 1 큰 수이다
			visitNum[maze_y + dy[i]][maze_x + dx[i]] = visitNum[maze_y][maze_x] + 1;
			q.push(make_pair(maze_x + dx[i], maze_y + dy[i]));
		}
	}

	while (!q.empty()) // 큐를 초기화한다
		q.pop();
}