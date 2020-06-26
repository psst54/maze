#pragma once

#include "ofMain.h"

#include <iostream>
#include <cstdlib>
#include <ctime>
#include <queue>
#include <fstream>


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
		void printMazeToFile();				// ��� Ȯ�ο� �Լ��̴�
		void printMazeToScreen();			// ��� Ȯ�ο� �Լ��̴�
		void saveMazeToChar();				// saveMazeToCharWithNum�� ���� �����̴�
		char getDirNum(int tX, int tY);					// saveMazeToCharWithNum���� ������ ���ڸ� �ҷ����� �Լ��̴�
		void saveMazeToCharWithNum();


		bool check4Dir(int x, int y, bool v);
		bool check1Dir(int x, int y, int direction, bool v);
		void breakWall(int x, int y, int dir);

		void walk();
		bool hunt();
		void generateMaze();

};
