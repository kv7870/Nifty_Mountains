#pragma once
#include "apmatrix.h"
#include <vector>
using namespace std; 

int initAllegro(ALLEGRO_DISPLAY ** display, ALLEGRO_TIMER ** timer, ALLEGRO_EVENT_QUEUE ** event_queue, ALLEGRO_FONT ** font);
int readElevation(apmatrix<int> &elevation); 
int findMin(const apmatrix<int> &elevation);
int findMax(const apmatrix<int> &elevation);
void drawMap(const int &min, const int &max, const apmatrix<int> &elevation, ALLEGRO_DISPLAY * display);
int drawLowestElevPath(const apmatrix<int> &elevation, vector<int> &shortestPath, int &minTotalChange,
	int currRow, int * jIndex, int jDim, vector<int> &currentPath); 
int indexOfLowestElevPath(const vector<int> &shortestPath, const int &minTotalChange, int rMin);
void drawBestPath(const int &startRow, const vector<int> &shortestPath);
void levelThree(const int &min, const int &max, const apmatrix<int> &elevation, ALLEGRO_DISPLAY * display); 
void levelFour_1(const int &min, const int &max, const apmatrix<int> &elevation, ALLEGRO_DISPLAY * display); 
