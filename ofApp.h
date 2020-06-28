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

		

		//------------�̷� ������ �ʿ��� �Լ���------------//
		bool check4Dir(int x, int y, bool v);						// ������ �� �ִ� ������ �ִ��� Ȯ���Ѵ�
		bool check1Dir(int x, int y, int direction, bool v);		// �־��� �������� ������ �� �ִ��� Ȯ���Ѵ�
		void breakWall(int x, int y, int dir);						// �־��� �������� ���� �μ��� ���ư���

		void walk();				// �� ĭ�� �̵��ϸ� �̷θ� �����
		bool hunt();				// ���� �̾����� ���� ���� �ִ� ��츦 ã�´�

		void saveMazeToChar();
		void makeImperfect();

		void generateMaze();		// �̷� ������ �� �� ���� ���� ȣ��Ǵ� �Լ��̴�
									// �� �Լ����� �� �Լ����� ȣ���� �Ͼ��


		//------------�̷� �׸��⿡ �ʿ��� ������------------//
		bool drawFlag = false, gameEndFlag = false;
		int cellSize = 20, margin = 50;

		//------------�̷� �׸��⿡ �ʿ��� �Լ�------------//
		void drawMaze();
};


//------------�÷��̾��� ������ �����ϴ� class------------//
class Player {
public:
	int curX, curY;

	void drawPlayer(int size, int margin);
};

//------------��ֹ��� ������ �����ϴ� class------------//
class Ant {
public:
	int curX, curY;
	int visitNum[70][70];

	void drawAnt(int size, int margin);
	void moveAnt();
	void bfs();
};