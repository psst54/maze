#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(255);
}

//--------------------------------------------------------------
void ofApp::update() {

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

	/*if (gameEnd) {
		;
	}
	*/
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	if (key == 'N' || key == 'n') {
		cout << "GENERATE NEW MAZE!\n";
		generateMaze();
		drawFlag = true;
		curX = 1;
		curY = 1;

		for (int i = 0; i < HEIGHT * 2 + 1; i++) {
			for (int j = 0; j < WIDTH * 2 + 1; j++) {
				visited[i][j] = false;
			}
		}
	}

	if (key == 'Q' || key == 'q') {
		ofExit();
	}

	if (key == OF_KEY_DOWN && drawFlag) {
		visited[curY][curX] = true;
		if (mazeTxt[curY + 1][curX] == ' ')
			curY++;
	}
	if (key == OF_KEY_LEFT && drawFlag) {
		visited[curY][curX] = true;
		if (mazeTxt[curY][curX - 1] == ' ')
			curX--; 
	}
	if (key == OF_KEY_RIGHT && drawFlag) {
		visited[curY][curX] = true;
		if (mazeTxt[curY][curX + 1] == ' ')
			curX++;
	}
	if (key == OF_KEY_UP && drawFlag) {
		visited[curY][curX] = true;
		if (mazeTxt[curY - 1][curX] == ' ')
			curY--;
	}


	if (key == 'j') {
		ofTrueTypeFont myfont;
		myfont.load("arial.ttf", 15);
		ofSetColor(100);
		myfont.drawString("Enter name : ", 200, 200);

		string arr;
		ofUTF8Insert(arr, 80, 1);

		myfont.drawString(arr, 400, 400);
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

	saveMazeToChar();
}



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
	// 도착점
	ofSetColor(255);
	ofDrawCircle((WIDTH + 1) * 2 * cellSize, (HEIGHT + 1) * 2 * cellSize, cellSize / 2);

	// 현재 위치
	ofRectangle rect;
	rect.x = curX * cellSize + margin;
	rect.y = curY * cellSize + margin;
	rect.width = cellSize;
	rect.height = cellSize;
	int fr = ofGetFrameNum();
	if (fr % 20 < 10)
		ofSetColor(0, 230, 190);
	else
		ofSetColor(0, 150, 160);
	ofDrawRectangle(rect);

	if (curX == WIDTH * 2 - 1 && curY == HEIGHT * 2 - 1) {
		drawFlag = false;
		gameEnd = true;
	}

	Player player;
	player.drawPlayer(curX, curY);

}


/*
ofResetElapsedTimeCounter()

ofSleepMillis(...)

*/


void Player::drawPlayer(int x, int y)
{
	cout << "OK!!";
}