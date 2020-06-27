#include "ofApp.h"

//--------------------------------------------------------------
void ofApp::setup(){
	ofBackground(255);
}

//--------------------------------------------------------------
void ofApp::update(){

}

//--------------------------------------------------------------
void ofApp::draw() {
	if (drawFlag) {
		ofTrueTypeFont myfont;
		myfont.load("arial.ttf", 10);

		for (int i = 0; i < HEIGHT * 2 + 1; i++) {
			for (int j = 0; j < WIDTH * 2 + 1; j++) {
				ofRectangle rect;
				rect.x = j * cellSize;
				rect.y = i * cellSize;
				rect.width = cellSize;
				rect.height = cellSize;



				if (j == curX && i == curY) {
					ofSetColor(0, 0, 150);
					ofDrawRectangle(rect);

					ofSetColor(240);
					string tmp = std::string(1, mazeTxt[i][j]);
					myfont.drawString(tmp, j * cellSize, i * cellSize + 10);
				}
					
				else if (visited[i][j]) {
					ofSetColor(0, 150, 230);
					ofDrawRectangle(rect);

					ofSetColor(10);
					string tmp = std::string(1, mazeTxt[i][j]);
					myfont.drawString(tmp, j * cellSize, i * cellSize + 10);
				}

				else {
					ofSetColor(0, 100, 190);
					ofDrawRectangle(rect);
				}
			}
		}
	}
}

//--------------------------------------------------------------
void ofApp::keyPressed(int key){
	if (key == 'N' || key == 'n') {
		cout << "\nGENERATE NEW MAZE!\n";
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
		if (mazeTxt[curY + 1][curX] >= 48 && mazeTxt[curY + 1][curX] <= 57)
			curY++;
		//printf("pressed Down\n");
	}
	if (key == OF_KEY_LEFT && drawFlag) {
		visited[curY][curX] = true;
		if (mazeTxt[curY][curX - 1] >= 48 && mazeTxt[curY][curX - 1] <= 57) 
			curX--;
		//printf("pressed Left\n");
	}
	if (key == OF_KEY_RIGHT && drawFlag) {
		visited[curY][curX] = true;
		if (mazeTxt[curY][curX + 1] >= 48 && mazeTxt[curY][curX + 1] <= 57) 
			curX++;
		//printf("pressed Right\n");
	}
	if (key == OF_KEY_UP && drawFlag) {
		visited[curY][curX] = true;
		if (mazeTxt[curY - 1][curX] >= 48 && mazeTxt[curY - 1][curX] <= 57) 
			curY--;
		//printf("pressed Up\n");
	}

}

//--------------------------------------------------------------
void ofApp::keyReleased(int key){

}

//--------------------------------------------------------------
void ofApp::mouseMoved(int x, int y ){

}

//--------------------------------------------------------------
void ofApp::mouseDragged(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mousePressed(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseReleased(int x, int y, int button){

}

//--------------------------------------------------------------
void ofApp::mouseEntered(int x, int y){

}

//--------------------------------------------------------------
void ofApp::mouseExited(int x, int y){

}

//--------------------------------------------------------------
void ofApp::windowResized(int w, int h){

}

//--------------------------------------------------------------
void ofApp::gotMessage(ofMessage msg){

}

//--------------------------------------------------------------
void ofApp::dragEvent(ofDragInfo dragInfo){ 

}



//-----------함수 선언------------//
void ofApp::printMazeToFile()
{
	ofstream writeFile;
	writeFile.open("output.maz");
	if (!writeFile.is_open()) {
		printf("error\n");
		std::exit(1);
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

void ofApp::printMazeToScreen()
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


	printf("\n\n");
	for (int i = 0; i < HEIGHT * 2 + 1; i++) {
		for (int j = 0; j < WIDTH * 2 + 1; j++) {
			printf("%c", mazeTxt[i][j]);
		}
		printf("\n");
	}
}

char ofApp::getDirNum(int tX, int tY)
{
	//int dx[8] = { -1, -1, -1, 0, 0, 1, 1, 1 },
	//	dy[8] = { -1, 0, 1, -1, 1, -1, 0, 1 };
	int dx[4] = { -1, 0, 1, 0 },
		dy[4] = { 0, -1, 0, 1 };
	char cnt = 0;

	for (int i = 0; i < 4; i++) {
		if (mazeTxt[tY + dy[i]][tX + dx[i]] == '|' ||
			mazeTxt[tY + dy[i]][tX + dx[i]] == '-' ||
			mazeTxt[tY + dy[i]][tX + dx[i]] == '+')
			cnt++;
	}

	return (4 - cnt);
}

void ofApp::saveMazeToCharWithNum()
{
	mazeTxt[0][0] = '+';
	for (int i = 1; i <= WIDTH; i++) {
		mazeTxt[0][i * 2 - 1] = '-';
		mazeTxt[0][i * 2] = '+';
	}

	for (int i = 1; i <= HEIGHT; i++) {

		mazeTxt[i * 2 - 1][0] = '|';
		for (int j = 1; j <= WIDTH; j++) {
			

			if (mazeInfo[i - 1][j - 1].right)
				mazeTxt[i * 2 - 1][j * 2] = '|';
			else
				mazeTxt[i * 2 - 1][j * 2] = getDirNum(j * 2, i * 2 - 1) + 48;

			mazeTxt[i * 2 - 1][j * 2 - 1] = getDirNum(j * 2 - 1, i * 2 - 1) + 48;
		}

		mazeTxt[i * 2][0] = '+';
		for (int j = 1; j <= WIDTH; j++) {
			

			if (mazeInfo[i - 1][j - 1].down)
				mazeTxt[i * 2][j * 2 - 1] = '-';
			else
				mazeTxt[i * 2][j * 2 - 1] = getDirNum(j * 2 - 1, i * 2) + 48;

			mazeTxt[i * 2][j * 2] = '+';
		}
	}


	printf("\n\n");
	for (int i = 0; i < HEIGHT * 2 + 1; i++) {
		for (int j = 0; j < WIDTH * 2 + 1; j++) {
			printf("%c", mazeTxt[i][j]);
		}
		printf("\n");
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


	//printMazeToFile();
	//printMazeToScreen();
	saveMazeToChar();
	saveMazeToCharWithNum();
}