#pragma once

#include "ofMain.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <fstream>
#include <string>

class ofApp : public ofBaseApp{

	public:
		void setup();
		void update();
		void draw();

		void keyPressed(int key);
		void keyReleased(int key);
		void mouseMoved(int x, int y );
		void mouseDragged(int x, int y, int button);
		void mousePressed(int x, int y, int button);
		void mouseReleased(int x, int y, int button);
		void mouseEntered(int x, int y);
		void mouseExited(int x, int y);
		void windowResized(int w, int h);
		void dragEvent(ofDragInfo dragInfo);
		void gotMessage(ofMessage msg);

		

		//------------미로 생성에 필요한 함수들------------//
		bool check4Dir(int x, int y, bool v);						// 움직일 수 있는 방향이 있는지 확인한다
		bool check1Dir(int x, int y, int direction, bool v);		// 주어진 방향으로 움직일 수 있는지 확인한다
		void breakWall(int x, int y, int dir);						// 주어진 방향으로 벽을 부수고 나아간다

		void walk();				// 한 칸씩 이동하며 미로를 만든다
		bool hunt();				// 아직 이어지지 않은 길이 있는 경우를 찾는다

		void saveMazeToChar();
		void makeImperfect();

		void generateMaze(int initX, int initY);		// 미로 생성을 할 때 제일 먼저 호출되는 함수이다
														// 이 함수에서 위 함수들의 호출이 일어난다


		//------------미로 그리기에 필요한 변수들------------//
		bool drawFlag = false, gameEndFlag = false;
		int cellSize = 20, margin = 50;

		//------------미로 그리기에 필요한 함수------------//
		void drawMaze();

		//------------게임 플레이에 필요한 변수------------//
		int NumOfAnt = 1, NumOfCoin = 1;
		int score;

		//------------게임 플레이에 필요한 함수------------//
		void InitGame();
};


//------------플레이어의 정보를 저장하는 class------------//
class Player {
public:
	// 플레이어의 x, y좌표를 저장한다
	int curX, curY;

	// 플레이어의 위치를 그린다
	void drawPlayer(int size, int margin);
};

//------------장애물의 정보를 저장하는 class------------//
class Ant {
public:
	// 장애물의 x, y좌표를 저장한다
	int curX, curY;
	// bfs에서 방문 정보를 저장하기 위해 사용하는 배열이다
	int visitNum[70][70];

	// 장애물의 위치를 그린다
	void drawAnt(int size, int margin, int antNum);
	// bfs를 통해 장애물이 움직일 곳을 정하고, 그 방향으로 움직인다
	void moveAnt(int antNum);
	void bfs(int antNum);
};

//------------코인의 정보를 저장하는 class------------//
class Coin {
public:
	// 코인의 x, y좌표를 저장한다
	int curX, curY;

	// 코인의 위치를 그린다
	void drawCoin(int size, int margin, int coinNum);
};