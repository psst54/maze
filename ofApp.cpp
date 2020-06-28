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

// 미로를 생성한 뒤부터 게임이 종료되기 전까지 drawFlag = true, 이외에는 flase이다
bool drawFlag = false, gameEndFlag = false;


Player player;			// player를 생성한다.
Ant ant[10];			// 장애물을 생성한다.
Coin coin[20];			// 코인을 생성한다


// 각각 이름, 점수 / 점수, 이름 쌍의 이진탐색트리를 만든다
map<string, int> nameidx;
map<int, string> idxname;
 
// N은 트리의 리프 노드의 개수를 저장한다. 
// rank 함수에서 랭킹을 입력받음과 함께 N의 개수가 증가한다.
int N;
struct seg {
	// lim은 리프 노드의 갯수보다 크거나 같은 2의 제곱수이다.
	int lim;
	vector<ii> tree;

	// N보다 lim이 작을때, lim *= 2를 해준다.
	void init(int N) {
		for (lim = 1; lim <= N; lim <<= 1);
		tree.resize(lim * 2 + 1);
	}

	void update(int i, int v) {
		tree[i += lim] = { v,i };
		i /= 2;
		while (i) {
			// i번 노드의 왼쪽 자식과 오른쪽 자식 중 큰 값을 i번 노드에 저장한다.
			// 이 과정을 루트 노드까지 계속 반복하여 루트에 최대값이 오도록 업데이트한다. 
			tree[i] = max(tree[i * 2], tree[i * 2 + 1]);
			i /= 2;
		}
	}
	ii mx() {
		// 최대값을 반환한다. 위 업데이트의 과정으로 루트에 최대값이 저장되어 있다.
		return tree[1];
	}

	// [] 연산자를 오버로딩한다. 
	int operator[](int i) 
	{ 
		return tree[i + lim].first; 
	}
} seg;


//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(255);	//배경은 흰색으로 지정한다
}

//--------------------------------------------------------------
void ofApp::update() {
	// 장애물과 마주친 경우, 미로 그림을 지우고 gmaeEndFlag를 set한다
	for (int i = 0; i < NumOfAnt; i++)
		if (drawFlag && player.curX == ant[i].curX && player.curY == ant[i].curY) {
			ofSleepMillis(500);		// 미로를 지우기 전에 잠시 멈춘다
			gameEndFlag = true;
			drawFlag = false;
		}

	// 코인을 만난 경우, 점수를 올리고 다른 곳에 다시 그려준다
	for (int i = 0; i < NumOfCoin; i++)
		if (drawFlag && player.curX == coin[i].curX && player.curY == coin[i].curY) {
			score += 100;

			// 해당 코인의 위치를 다시 랜덤하게 배치해준다
			coin[i].curX = (rand() % WIDTH) * 2 - 1;
			coin[i].curY = (rand() % HEIGHT) * 2 - 1;
			while ((coin[i].curX == player.curX && coin[i].curY == player.curY)
				|| coin[i].curX < 0 || coin[i].curY < 0) {
				coin[i].curX = (rand() % WIDTH) * 2 - 1;
				coin[i].curY = (rand() % HEIGHT) * 2 - 1;
			}

			cout << "+100\n";
		}

	// 10프레임마다 장애물을 이동한다.
	for (int i = 0; i < NumOfAnt; i++)
		if (drawFlag && (ofGetFrameNum() % 10) == 0)
			ant[i].moveAnt(i);

	// 게임이 종료되어 gameEndFlag가 true인 경우에, 
	// rank 함수를 호출해 랭크 정보를 읽거나 저장한다.
	if (gameEndFlag) {
		rank();
	}
} 

//--------------------------------------------------------------
void ofApp::draw() {
	// 미로 위에 항상 
	ofTrueTypeFont myfont;
	myfont.load("arial.ttf", 15);
	ofSetColor(100);
	myfont.drawString("Press N to generate new maze", 20, 20);
	myfont.drawString("Press H to generate new 10x10 maze", 20, 40);

	// 미로가 생성된 경우 drawFlag가 true이고, 그 경우에 미로를 그린다
	if (drawFlag) {
		drawMaze();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	// n을 누르면 새 미로를 만들 수 있다
	if (key == 'N' || key == 'n') {
		cout << "\nGENERATE NEW MAZE!\n";
		
		//미로 생성 함수를 호출한다
		generateMaze(0, 0);
		InitGame();
	}

	// h를 누르면 미로 크기 입력을 하지 않고 10 * 10 사이즈의 미로를 생성한다
	if (key == 'H' || key == 'h') {
		cout << "\nGENERATE 15x15 MAZE!\n";

		//미로 생성 함수를 호출한다
		generateMaze(10, 10);
		InitGame();
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

void ofApp::generateMaze(int initX, int initY) {
	srand(time(NULL));

	// 미로의 크기는 5 이상 30 이하로 제한한다
	// 더 큰 미로를 만들고 싶다면 mazeInfo, mazeTxt, visitNum의 배열 크기를 늘리고 cellSize를 조정한다
	if (initX == 0 && initY == 0) {
		do {
			cout << "5 <= WIDTH <= 30, 5 <= HEIGHT <= 30\n";
			cout << "WIDTH : ";
			cin >> WIDTH;
			cout << "HEIGHT : ";
			cin >> HEIGHT;
		} while (WIDTH < 5 || WIDTH > 30 || HEIGHT < 5 || HEIGHT > 30);
	}
	else {
		WIDTH = initX;
		HEIGHT = initY;
	}
	

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

	// 생성된 미로의 전체 그림을 char형으로 저장한다
	saveMazeToChar();
	// 완전미로를 불완전미로로 바꾼다
	makeImperfect();
}

//--------------완전미로를 불완전미로로 바꾼다-------------//
void ofApp::makeImperfect()
{
	int wallToBreak = (HEIGHT > WIDTH ? HEIGHT : WIDTH);
	while (wallToBreak) {
		int wX = rand() % (HEIGHT * 2);
		int wY = rand() % (WIDTH * 2);
		if (wX == 0 || wY == 0)
			continue;
		if (mazeTxt[wY][wX] == '+' || mazeTxt[wY][wX] == ' ')
			continue;
		wallToBreak--;
		mazeTxt[wY][wX] = ' ';
	}
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

	// 플레이어를 그린다
	player.drawPlayer(cellSize, margin);
	// 장애물을 그린다
	for (int i = 0; i < NumOfAnt; i++)
		ant[i].drawAnt(cellSize, margin, i);
	// 코인을 그린다
	for (int i = 0; i < NumOfCoin; i++)
		coin[i].drawCoin(cellSize, margin, i);
}

//-------------게임의 초기값을 결정하는 함수이다-------------//
void ofApp::InitGame()
{
	// 플레이어의 시작점은 좌측 상단이다
	player.curX = 1;
	player.curY = 1;

	// 장애물 개수는 가로/세로 길이 중 짧은 쪽을 3으로 나눈 몫이다.
	// 값은 항상 자연수가 되도록 해준다.
	NumOfAnt = (HEIGHT < WIDTH ? HEIGHT : WIDTH);
	NumOfAnt /= 3;
	if (NumOfAnt < 1)
		NumOfAnt = 1;

	// 장애물의 시작점은 랜덤으로 정해진다
	for (int i = 0; i < NumOfAnt; i++) {
		do {
			ant[i].curX = (rand() % WIDTH) * 2 - 1;
		} while (ant[i].curX <= 3); // 시작점과 너무 가깝지 않도록 조정한다

		do {
			ant[i].curY = (rand() % HEIGHT) * 2 - 1;
		} while (ant[i].curY <= 3); // 시작점과 너무 가깝지 않도록 조정한다
	}

	// 코인 개수는 가로/세로 길이 중 긴 쪽을 3으로 나눈 몫이다.
	// 값은 항상 자연수가 되도록 해준다.
	NumOfCoin = (HEIGHT > WIDTH ? HEIGHT : WIDTH);
	NumOfCoin /= 3;
	if (NumOfCoin < 1)
		NumOfCoin = 1;

	// 코인의 위치도 랜덤으로 정해진다
	for (int i = 0; i < NumOfCoin; i++) {
		coin[i].curX = (rand() % WIDTH) * 2 - 1;
		coin[i].curY = (rand() % HEIGHT) * 2 - 1;

		// 플레이어의 현위치에는 코인이 생기지 않도록 한다
		while ((coin[i].curX == player.curX && coin[i].curY == player.curY) 
			|| coin[i].curX < 0 || coin[i].curY < 0) {
			coin[i].curX = (rand() % WIDTH) * 2 - 1;
			coin[i].curY = (rand() % HEIGHT) * 2 - 1;
		}
	}

	drawFlag = true;
	score = 0;
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
void Ant::drawAnt(int size, int margin, int antNum)
{
	// drawPlayer에서 색만 바꾼 함수이다
	ofRectangle rect;
	rect.x = ant[antNum].curX * size + margin;
	rect.y = ant[antNum].curY * size + margin;
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
void Ant::moveAnt(int antNum)
{
	// 장애물은 bfs의 결과값을 이용해 플레이어 방향으로 이동한다
	ant[antNum].bfs(antNum);

	// bfs 함수에서 기록해놓은 방문 순서를 거꾸로 따라가면서 다음 칸을 결정한다
	int nextX = player.curX, nextY = player.curY;

	// 방문 순서가 첫번재인 지점이 장애물의 다음 위치이다
	while (visitNum[nextY][nextX] != 1) {
		for (int i = 0; i < 4; i++) {
			if (mazeTxt[nextY + dy[i]][nextX + dx[i]] != ' ')
				continue;
			if (visitNum[nextY + dy[i]][nextX + dx[i]] != visitNum[nextY][nextX] - 1)
				continue;

			nextX += dx[i];
			nextY += dy[i];
			break;
		}
	}
	curX = nextX;
	curY = nextY;
	
}

//--------------장애물이 이동할 경로를 찾는 함수이다-------------//
void Ant::bfs(int antNum)
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

	int maze_x = ant[antNum].curX, maze_y = ant[antNum].curY;
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

//--------------코인을 그리는 함수이다-------------//
void Coin::drawCoin(int size, int margin, int coinNum)
{
	int fr = ofGetFrameNum();
	if (fr % 20 < 10)
		ofSetColor(0, 230, 190);
	else
		ofSetColor(0, 150, 160);

	ofSetColor(255);
	ofDrawCircle(coin[coinNum].curX * size + margin + size / 2, coin[coinNum].curY * size + margin + size / 2, size / 2);
}



void ofApp::rank()
{
	string s;
	int x, op;
	seg.init(100000);

	// s에는 입력받은 이름을, x에는 점수를 저장한다
	cout << "\nEnter name : ";
	cin >> s;
	x = score;

	// nameidx에서 같은 이름을 발견한 경우, 기존 정보를 업데이트한다
	if (nameidx.find(s) != nameidx.end()) {
		int idx = nameidx[s];
		if (seg[idx] < x) {
			seg.update(idx, x);
			cout << "User's information has been updated\n";
		}
	}
	// nameidx에 같은 이름이 없는 경우, 새로운 정보이므로 N - 1번째로 삽입한다
	else {
		seg.update(nameidx[s] = N, x);
		idxname[N++] = s;
		cout << "User's information has been inserted\n";
	}

	// 1을 입력받은 경우에는 랭크 전체를 출력한다
	// 나머지 숫자키를 입력받은 경우에는 다시 빈 화면으로 돌아간다
	while (1) {
		cout << "Press 1 to show the list or Press any number to return to main\n";
		cin >> op;
		if (op == 1) {
			for (int i = 0; i < N; i++) {
				ii ret = seg.mx();
				cout << i + 1 << ". " << idxname[ret.second] << '\t' << ret.first << '\n';
				int v = seg[ret.second];
				seg.update(ret.second, -v);
			}
			for (int i = 0; i < N; i++) {
				int v = seg[i];
				seg.update(i, -v);
			}
		}
		else break;
	}

	// update에서 다시 rank 함수를 호출하지 않도록 gameEndFlag = false로 설정한다
	gameEndFlag = false;
}