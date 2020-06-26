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

		//------------미로 생성에 필요한 변수들------------//
		typedef struct _cell {
			bool left;
			bool up;
			bool right;
			bool down;		// 벽이 없는 경우 false
			bool visited;	// 방문한 경우 true
		} cell;
		cell **mazeInfo;
		char mazeTxt[500][500];

		int dx[4] = { -1, 0, 1, 0 }, dy[4] = { 0, -1, 0, 1 }; // left up right down
		int WIDTH, HEIGHT; // n : 가로 m : 세로
		queue<pair<int, int> > q;

		//------------미로 생성에 필요한 함수들------------//
		void printMazeToFile();				// 출력 확인용 함수이다
		void printMazeToScreen();			// 출력 확인용 함수이다
		void saveMazeToChar();				// saveMazeToCharWithNum의 이전 버전이다
		char getDirNum(int tX, int tY);					// saveMazeToCharWithNum에서 저장할 숫자를 불러오는 함수이다
		void saveMazeToCharWithNum();


		bool check4Dir(int x, int y, bool v);
		bool check1Dir(int x, int y, int direction, bool v);
		void breakWall(int x, int y, int dir);

		void walk();
		bool hunt();
		void generateMaze();

};
