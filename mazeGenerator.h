#include "ofApp.h"

void printMazeToFile();
void printMazeToScreen();

bool check4Dir(int x, int y, bool v);
bool check1Dir(int x, int y, int direction, bool v);

void breakWall(int x, int y, int dir);
void walk();
bool hunt();

void generateMaze();
