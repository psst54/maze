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

		//------------�̷� ������ �ʿ��� �Լ���------------//
		void saveMazeToChar();				// saveMazeToCharWithNum�� ���� �����̴�

		bool check4Dir(int x, int y, bool v);
		bool check1Dir(int x, int y, int direction, bool v);
		void breakWall(int x, int y, int dir);

		void walk();
		bool hunt();
		void generateMaze();


		//------------�̷� �׸��⿡ �ʿ��� ������------------////
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


class Ant : public Player {
public:
	int curX, curY;

	void drawAnt(int size, int margin);
	void moveAnt();
};