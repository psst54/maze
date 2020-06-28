#include "ofApp.h"

//------------�̷� ������ �ʿ��� ������------------//
typedef struct _cell {
	bool left;
	bool up;
	bool right;
	bool down;		
	// ���� ���� ��� false
	bool visited;	
	// �湮�� ��� true
} cell;

// �̷� ������ �ʿ��� ����ü�̴�. �� ĭ�� �����¿쿡 ���� �ִ���, �湮�� ĭ������ �����Ѵ�
cell mazeInfo[30][30];	
// mazeInfo�� ���� ������ �̷θ� char������ �׸�ó�� �����ϴ� �迭�̴�
char mazeTxt[70][70];

// ���� left up right down �������� �̵��ϴ� ��츦 dx, dy�� �����Ͽ���
int dx[4] = { -1, 0, 1, 0 }, dy[4] = { 0, -1, 0, 1 }; 
//�̷��� ����, ���� ĭ ���̴�
int WIDTH, HEIGHT;
// �̷� ����, bfs���� ���Ǵ� ť�̴�
queue<pair<int, int> > q;

// �̷θ� ������ �ں��� ������ ����Ǳ� ������ drawFlag = true, �̿ܿ��� flase�̴�
bool drawFlag = false, gameEndFlag = false;


Player player;			// player�� �����Ѵ�.
Ant ant[10];			// ��ֹ��� �����Ѵ�.
Coin coin[20];			// ������ �����Ѵ�


// ���� �̸�, ���� / ����, �̸� ���� ����Ž��Ʈ���� �����
map<string, int> nameidx;
map<int, string> idxname;
 
// N�� Ʈ���� ���� ����� ������ �����Ѵ�. 
// rank �Լ����� ��ŷ�� �Է¹����� �Բ� N�� ������ �����Ѵ�.
int N;
struct seg {
	// lim�� ���� ����� �������� ũ�ų� ���� 2�� �������̴�.
	int lim;
	vector<ii> tree;

	// N���� lim�� ������, lim *= 2�� ���ش�.
	void init(int N) {
		for (lim = 1; lim <= N; lim <<= 1);
		tree.resize(lim * 2 + 1);
	}

	void update(int i, int v) {
		tree[i += lim] = { v,i };
		i /= 2;
		while (i) {
			// i�� ����� ���� �ڽİ� ������ �ڽ� �� ū ���� i�� ��忡 �����Ѵ�.
			// �� ������ ��Ʈ ������ ��� �ݺ��Ͽ� ��Ʈ�� �ִ밪�� ������ ������Ʈ�Ѵ�. 
			tree[i] = max(tree[i * 2], tree[i * 2 + 1]);
			i /= 2;
		}
	}
	ii mx() {
		// �ִ밪�� ��ȯ�Ѵ�. �� ������Ʈ�� �������� ��Ʈ�� �ִ밪�� ����Ǿ� �ִ�.
		return tree[1];
	}

	// [] �����ڸ� �����ε��Ѵ�. 
	int operator[](int i) 
	{ 
		return tree[i + lim].first; 
	}
} seg;


//--------------------------------------------------------------
void ofApp::setup() {
	ofBackground(255);	//����� ������� �����Ѵ�
}

//--------------------------------------------------------------
void ofApp::update() {
	// ��ֹ��� ����ģ ���, �̷� �׸��� ����� gmaeEndFlag�� set�Ѵ�
	for (int i = 0; i < NumOfAnt; i++)
		if (drawFlag && player.curX == ant[i].curX && player.curY == ant[i].curY) {
			ofSleepMillis(500);		// �̷θ� ����� ���� ��� �����
			gameEndFlag = true;
			drawFlag = false;
		}

	// ������ ���� ���, ������ �ø��� �ٸ� ���� �ٽ� �׷��ش�
	for (int i = 0; i < NumOfCoin; i++)
		if (drawFlag && player.curX == coin[i].curX && player.curY == coin[i].curY) {
			score += 100;

			// �ش� ������ ��ġ�� �ٽ� �����ϰ� ��ġ���ش�
			coin[i].curX = (rand() % WIDTH) * 2 - 1;
			coin[i].curY = (rand() % HEIGHT) * 2 - 1;
			while ((coin[i].curX == player.curX && coin[i].curY == player.curY)
				|| coin[i].curX < 0 || coin[i].curY < 0) {
				coin[i].curX = (rand() % WIDTH) * 2 - 1;
				coin[i].curY = (rand() % HEIGHT) * 2 - 1;
			}

			cout << "+100\n";
		}

	// 10�����Ӹ��� ��ֹ��� �̵��Ѵ�.
	for (int i = 0; i < NumOfAnt; i++)
		if (drawFlag && (ofGetFrameNum() % 10) == 0)
			ant[i].moveAnt(i);

	// ������ ����Ǿ� gameEndFlag�� true�� ��쿡, 
	// rank �Լ��� ȣ���� ��ũ ������ �аų� �����Ѵ�.
	if (gameEndFlag) {
		rank();
	}
} 

//--------------------------------------------------------------
void ofApp::draw() {
	// �̷� ���� �׻� 
	ofTrueTypeFont myfont;
	myfont.load("arial.ttf", 15);
	ofSetColor(100);
	myfont.drawString("Press N to generate new maze", 20, 20);
	myfont.drawString("Press H to generate new 10x10 maze", 20, 40);

	// �̷ΰ� ������ ��� drawFlag�� true�̰�, �� ��쿡 �̷θ� �׸���
	if (drawFlag) {
		drawMaze();
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key) {
	// n�� ������ �� �̷θ� ���� �� �ִ�
	if (key == 'N' || key == 'n') {
		cout << "\nGENERATE NEW MAZE!\n";
		
		//�̷� ���� �Լ��� ȣ���Ѵ�
		generateMaze(0, 0);
		InitGame();
	}

	// h�� ������ �̷� ũ�� �Է��� ���� �ʰ� 10 * 10 �������� �̷θ� �����Ѵ�
	if (key == 'H' || key == 'h') {
		cout << "\nGENERATE 15x15 MAZE!\n";

		//�̷� ���� �Լ��� ȣ���Ѵ�
		generateMaze(10, 10);
		InitGame();
	}

	//Q�� ������ ���α׷��� �����Ѵ�
	if (key == 'Q' || key == 'q') {
		ofExit();
	}

	// ����Ű�� ���� �÷��̾ ������ �� �ִ�
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

void ofApp::generateMaze(int initX, int initY) {
	srand(time(NULL));

	// �̷��� ũ��� 5 �̻� 30 ���Ϸ� �����Ѵ�
	// �� ū �̷θ� ����� �ʹٸ� mazeInfo, mazeTxt, visitNum�� �迭 ũ�⸦ �ø��� cellSize�� �����Ѵ�
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
	// �̷� ������ ������ ������ �������� �����Ѵ�
	q.push(make_pair(startX, startY));

	do {
		while (!q.empty())	// ���̻� ������ �� ���� ������
			walk();
	} while (hunt());		// ���� �湮���� ���� ���� �ִ��� ã�´�

	// ������ �̷��� ��ü �׸��� char������ �����Ѵ�
	saveMazeToChar();
	// �����̷θ� �ҿ����̷η� �ٲ۴�
	makeImperfect();
}

//--------------�����̷θ� �ҿ����̷η� �ٲ۴�-------------//
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

	// �÷��̾ �׸���
	player.drawPlayer(cellSize, margin);
	// ��ֹ��� �׸���
	for (int i = 0; i < NumOfAnt; i++)
		ant[i].drawAnt(cellSize, margin, i);
	// ������ �׸���
	for (int i = 0; i < NumOfCoin; i++)
		coin[i].drawCoin(cellSize, margin, i);
}

//-------------������ �ʱⰪ�� �����ϴ� �Լ��̴�-------------//
void ofApp::InitGame()
{
	// �÷��̾��� �������� ���� ����̴�
	player.curX = 1;
	player.curY = 1;

	// ��ֹ� ������ ����/���� ���� �� ª�� ���� 3���� ���� ���̴�.
	// ���� �׻� �ڿ����� �ǵ��� ���ش�.
	NumOfAnt = (HEIGHT < WIDTH ? HEIGHT : WIDTH);
	NumOfAnt /= 3;
	if (NumOfAnt < 1)
		NumOfAnt = 1;

	// ��ֹ��� �������� �������� ��������
	for (int i = 0; i < NumOfAnt; i++) {
		do {
			ant[i].curX = (rand() % WIDTH) * 2 - 1;
		} while (ant[i].curX <= 3); // �������� �ʹ� ������ �ʵ��� �����Ѵ�

		do {
			ant[i].curY = (rand() % HEIGHT) * 2 - 1;
		} while (ant[i].curY <= 3); // �������� �ʹ� ������ �ʵ��� �����Ѵ�
	}

	// ���� ������ ����/���� ���� �� �� ���� 3���� ���� ���̴�.
	// ���� �׻� �ڿ����� �ǵ��� ���ش�.
	NumOfCoin = (HEIGHT > WIDTH ? HEIGHT : WIDTH);
	NumOfCoin /= 3;
	if (NumOfCoin < 1)
		NumOfCoin = 1;

	// ������ ��ġ�� �������� ��������
	for (int i = 0; i < NumOfCoin; i++) {
		coin[i].curX = (rand() % WIDTH) * 2 - 1;
		coin[i].curY = (rand() % HEIGHT) * 2 - 1;

		// �÷��̾��� ����ġ���� ������ ������ �ʵ��� �Ѵ�
		while ((coin[i].curX == player.curX && coin[i].curY == player.curY) 
			|| coin[i].curX < 0 || coin[i].curY < 0) {
			coin[i].curX = (rand() % WIDTH) * 2 - 1;
			coin[i].curY = (rand() % HEIGHT) * 2 - 1;
		}
	}

	drawFlag = true;
	score = 0;
}





//--------------�÷��̾ �׸��� �Լ��̴�-------------//
void Player::drawPlayer(int size, int margin)
{
	ofRectangle rect;
	// �÷��̾��� x��ǥ�� curX * size(�̷� �� ĭ�� ������) + margin(���� ����)�� �ȴ�.
	rect.x = player.curX * size + margin;	
	// �÷��̾��� y��ǥ�� curY * size(�̷� �� ĭ�� ������) + margin(���� ����)�� �ȴ�.
	rect.y = player.curY * size + margin;
	// �� ĭ�� ũ��� size * size�̴�
	rect.width = size;
	rect.height = size;

	// �����̴� ȿ���� ���� frame ��ȣ�� 5�� �ٲ𶧸��� ���� �ٲ��ش�
	int fr = ofGetFrameNum();
	if (fr % 20 < 10)
		ofSetColor(230, 240, 80);
	else
		ofSetColor(140, 180, 110);

	// ������ ������ ��ǥ, ������, ������ ���� �÷��̾ �׸���
	ofDrawRectangle(rect);
}

//--------------��ֹ��� �׸��� �Լ��̴�-------------//
void Ant::drawAnt(int size, int margin, int antNum)
{
	// drawPlayer���� ���� �ٲ� �Լ��̴�
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

//--------------��ֹ��� �̵���Ű�� �Լ��̴�-------------//
void Ant::moveAnt(int antNum)
{
	// ��ֹ��� bfs�� ������� �̿��� �÷��̾� �������� �̵��Ѵ�
	ant[antNum].bfs(antNum);

	// bfs �Լ����� ����س��� �湮 ������ �Ųٷ� ���󰡸鼭 ���� ĭ�� �����Ѵ�
	int nextX = player.curX, nextY = player.curY;

	// �湮 ������ ù������ ������ ��ֹ��� ���� ��ġ�̴�
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

//--------------��ֹ��� �̵��� ��θ� ã�� �Լ��̴�-------------//
void Ant::bfs(int antNum)
{
	//-------ť�� �̷��� �湮 ������ �ʱ�ȭ�Ѵ�
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

		// �÷��̾��� ��ġ���� �����ߴٸ� bfs�� ��ģ��
		if (maze_x == player.curX && maze_y == player.curY)
			break;

		for (int i = 0; i < 4; i++) {
			// ���� ĭ�� �̹� �湮�� ĭ�̶�� �� �� ���� �ʵ��� �Ѵ�
			if (visitNum[maze_y + dy[i]][maze_x + dx[i]])
				continue;
			// ���� ĭ�� ���̶�� �ٸ� ������ ã�´�
			if (mazeTxt[maze_y + dy[i]][maze_x + dx[i]] != ' ')
				continue;

			// ���� ĭ�� visitNum���� ���� ĭ�� visitNum���� 1 ū ���̴�
			visitNum[maze_y + dy[i]][maze_x + dx[i]] = visitNum[maze_y][maze_x] + 1;
			q.push(make_pair(maze_x + dx[i], maze_y + dy[i]));
		}
	}

	while (!q.empty()) // ť�� �ʱ�ȭ�Ѵ�
		q.pop();
}

//--------------������ �׸��� �Լ��̴�-------------//
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

	// s���� �Է¹��� �̸���, x���� ������ �����Ѵ�
	cout << "\nEnter name : ";
	cin >> s;
	x = score;

	// nameidx���� ���� �̸��� �߰��� ���, ���� ������ ������Ʈ�Ѵ�
	if (nameidx.find(s) != nameidx.end()) {
		int idx = nameidx[s];
		if (seg[idx] < x) {
			seg.update(idx, x);
			cout << "User's information has been updated\n";
		}
	}
	// nameidx�� ���� �̸��� ���� ���, ���ο� �����̹Ƿ� N - 1��°�� �����Ѵ�
	else {
		seg.update(nameidx[s] = N, x);
		idxname[N++] = s;
		cout << "User's information has been inserted\n";
	}

	// 1�� �Է¹��� ��쿡�� ��ũ ��ü�� ����Ѵ�
	// ������ ����Ű�� �Է¹��� ��쿡�� �ٽ� �� ȭ������ ���ư���
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

	// update���� �ٽ� rank �Լ��� ȣ������ �ʵ��� gameEndFlag = false�� �����Ѵ�
	gameEndFlag = false;
}