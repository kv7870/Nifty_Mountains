// Nifty_Mountains.cpp : Defines the entry point for the console application.
//

#include "stdafx.h"
#include <iostream>
#include <vector>
#include <allegro5/allegro.h> 
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include <fstream>
#include <algorithm>
#include <cmath>
#include <limits.h>
#include "header.h"			
#include "apmatrix.h"
using namespace std; 

const int ROW = 480;
const int COL = 844;

int main()
{	
	srand(time(0));
	ALLEGRO_DISPLAY * display = NULL;
	ALLEGRO_TIMER * timer = NULL;
	ALLEGRO_EVENT_QUEUE *  event_queue = NULL;
	ALLEGRO_FONT * font = NULL;

	apmatrix<int> elevation(ROW, COL);

	initAllegro(&display, &timer, &event_queue, &font); 

	readElevation(elevation); 
	int min = findMin(elevation);
	int max = findMax(elevation);

	int choice; 
	cout << "select 1 to 3"<<" ";
	cin >> choice;

	switch (choice) {
	case 1: 
		levelThree(min, max, elevation, display);
		break;
	case 2:
		levelFour_1(min, max, elevation, display);
		break;
	case 3:
		break; 

	default: {
		cout << "invalid choice";
		exit(-1);
		}
	}

 
    return 0;
}

void levelFour_1(const int &min, const int &max, const apmatrix<int> &elevation, ALLEGRO_DISPLAY * display) {
	apmatrix<int> gradient(ROW, COL);
	vector<int> shortestPath(COL);
	int minTotalChange = INT_MAX;
	int jIndex[COL - 1];
	int jDim = COL - 1;
	int iIndex[ROW];
	int iDim = ROW;
	int numSearchesRow = 2;
	int rMin = 0;

	vector<int> currentPath(COL, -1);

	drawMap(min, max, elevation, display);

	for (int i = 0; i < iDim; i++) {
		iIndex[i] = i;
	}

	for (int ii = 0; ii < iDim; ii++) {
		int currRow = iIndex[ii];
		int totalChange = 0;

		int r = rand() % (COL - 2) + 1;

		for (int n = 0; n < numSearchesRow; n++) {
			int count = 0;
			
			if (n == 0) {	
				for (int j = r; j < jDim; j++) {
					jIndex[count++] = j;
				}
			}
			else {
				for (int j = r; j > 0; j--) {
					jIndex[count++] = j;
				}
			}

			totalChange += drawLowestElevPath(elevation, shortestPath, minTotalChange, currRow, jIndex, count, currentPath);

		}

		if (totalChange < minTotalChange) {
			minTotalChange = totalChange;
			shortestPath = currentPath;
			rMin = r;
		}

		cout << "i :" << currRow << " " << "Total change: " << totalChange << endl;
	}


	int startRow = indexOfLowestElevPath(shortestPath, minTotalChange, rMin);

	drawBestPath(startRow, shortestPath);


	cout << endl;
	for (int i = 0; i < COL; i++)
		cout << shortestPath[i] << " ";

	al_rest(10000);
	
}





void levelThree(const int &min, const int &max, const apmatrix<int> &elevation, ALLEGRO_DISPLAY * display) {

	apmatrix<int> gradient(ROW, COL);
	vector<int> shortestPath(COL);
	int minTotalChange = INT_MAX;
	int jIndex[COL - 1];
	int jDim = COL - 1;
	int iIndex[ROW];
	int iDim = ROW;
	int numSearchesRow = 1;
	int rMin = 0;

	vector<int> currentPath(COL, -1);

	drawMap(min, max, elevation, display);

	for (int i = 0; i < iDim; i++) {
		iIndex[i] = i;
	}

	for (int ii = 0; ii < iDim; ii++) {
		int currRow = iIndex[ii];
		int totalChange = 0;

		for (int n = 0; n < numSearchesRow; n++) {
			for (int j = 0; j < jDim; j++) {
				jIndex[j] = j;
			}

			totalChange += drawLowestElevPath(elevation, shortestPath, minTotalChange, currRow, jIndex, jDim, currentPath);

		}

		if (totalChange < minTotalChange) {
			minTotalChange = totalChange;
			shortestPath = currentPath;
		}

		cout << "i :" << currRow << " " << "Total change: " << totalChange << endl;
	}


	int startRow = indexOfLowestElevPath(shortestPath, minTotalChange, rMin);

	drawBestPath(startRow, shortestPath);


	cout << endl;
	for (int i = 0; i < COL; i++)
		cout << shortestPath[i] << " ";

	al_rest(10000);
}


int readElevation(apmatrix<int> &elevation) {
	ifstream fin("colorado.dat");

	if (!fin) {
		cout << "no file";
		return -1;
	}

	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			fin >> elevation[i][j];
		}
	}
	return 0; 
}

int findMin(const apmatrix<int> &elevation){
	int min = 32700;
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (elevation[i][j] < min)
				min = elevation[i][j];
		}
	}
	cout << "MIN: " << min<<endl;
	return min;
}

int findMax(const apmatrix<int> &elevation) {
	int max = 0; 
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (elevation[i][j] > max)
				max = elevation[i][j];
		}
	}
	cout << "MAX: " << max; 
	return max;
}


inline void drawMap(const int &min, const int &max, const apmatrix<int> &elevation, ALLEGRO_DISPLAY * display) {
	int c; //calculated grayscale value 
	int i, j;
	float weightedShade;

	if (max != min)
		weightedShade = 255. / (max - min);

	for (i = 0; i < ROW; i++) {
		for (j = 0; j < COL; j++) {
			c = round((elevation[i][j] - min)*weightedShade);
		

			//ALLEGRO_COLOR color = al_map_rgb(c, c, c); 
			//al_draw_filled_rectangle(j, i, j + 1, i + 1, color);
			//al_flip_display();
		}
		cout << i << endl;
	}
}

int drawLowestElevPath(const apmatrix<int> &elevation, vector<int> &shortestPath, int &minTotalChange, 
	int currRow, int * jIndex, int jDim, vector<int> &currentPath) {

	int  j, jj;

	int currCol = jIndex[0];

	int totalChange = 0;		
	int jShift = jIndex[1] - jIndex[0];

	//cout << "currCol: " << currCol << endl << "currRow: " << currRow << endl; 
	//cout << "jIndex[0]: " << jIndex[0] << endl; 

	currentPath[jIndex[0]] = currCol + COL * currRow;

	for (jj = 0; jj < jDim; jj++) {
			
		j = jIndex[jj];

		currCol = j;

		//current vertex
		int currV = currCol + COL * currRow; 
		int ip1 = currRow + 1;
		int im1 = currRow - 1;
		int lowestNextChange = INT_MAX;
		int nextChangeIndex  = 0;

		int nextV[3] = { -1,-1,-1 };
		int nextI[3] = { -1,-1,-1 };	//i = row number 
		int nextChange[3] = { INT_MAX, INT_MAX, INT_MAX };

		//1 = straight ahead 
		nextV[0] = currV + jShift; 
		nextI[0] = currRow;

		if (im1 > -1) {
			nextV[1] = nextV[0] - COL;
			nextI[1] = im1;
		}

		if (ip1 < ROW) {
		nextV[2] = nextV[0] + COL;
				nextI[2] = ip1;
		}
			
		ALLEGRO_COLOR color = al_map_rgb(255, 0, 0);
		al_draw_filled_rectangle(currCol, currRow, currCol + 1, currRow + 1, color);
		al_flip_display(); 

		for (int n = 0; n < 3; n++) {
			if (nextV[n] > -1) {
				int elevChange = abs(elevation[nextI[n]][currCol + jShift] - elevation[currRow][currCol]);
				nextChange[n]  = elevChange;

				if (lowestNextChange > elevChange) {
					lowestNextChange = elevChange;
					nextChangeIndex = n;
				}
			}
		}

		if (nextChangeIndex != 0) {
			if (nextChange[1] == nextChange[2]) {
				nextChangeIndex = rand() % 2 + 1;
			}
		}


		totalChange += lowestNextChange;

		currRow = nextI[nextChangeIndex];

		//currentPath tracks vertices
		currentPath[j+jShift] = nextV[nextChangeIndex];
		//cout << "j+jShift: " << j + jShift << " " << "jj: " << jj << " " << "jDim: " << jDim << " " << endl; 
	}



	return totalChange; 

}


int indexOfLowestElevPath(const vector<int> &shortestPath, const int &minTotalChange, int rMin) {

	int startRow = shortestPath[rMin] / COL; 

	cout << "shortest elevation change: "<< minTotalChange<<" meters"<<endl; 
	cout << "start row: " << startRow << endl; 
	cout << "last vertex: " << shortestPath[COL - 1] << endl;

	return startRow;
}

void drawBestPath(const int &startRow, const vector<int> &shortestPath) {
	int currRow, currCol;

	for (int j = 0; j < COL; j++) {
		//convert vertex to matrix index 
		currRow = shortestPath[j] / COL;
		currCol = shortestPath[j] - (currRow * COL);	//shortesPath[j] % COL; 

		ALLEGRO_COLOR color = al_map_rgb(0, 255, 0);
		al_draw_filled_rectangle(currCol, currRow, currCol + 1, currRow + 1, color);
		al_flip_display(); 
	}
}



