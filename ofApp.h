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
		void saveMazeToChar();				// saveMazeToCharWithNum의 이전 버전이다

		bool check4Dir(int x, int y, bool v);
		bool check1Dir(int x, int y, int direction, bool v);
		void breakWall(int x, int y, int dir);

		void walk();
		bool hunt();
		void generateMaze();


		//------------미로 그리기에 필요한 변수들------------////
		bool drawFlag = false, gameEndFlag = false;
		int cellSize = 20, margin = 50;
		bool visited[500][500];

		void drawMaze();
};


class Player {
public:
	int curX, curY;

	void drawPlayer(int size, int margin);
};


class Ant {
public:
	int curX, curY;
	int stack[1000][2];

	void drawAnt(int size, int margin);
	void moveAnt();
	void dfs();
};