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
	ALLEGRO_DISPLAY * display = NULL;
	ALLEGRO_TIMER * timer = NULL;
	ALLEGRO_EVENT_QUEUE *  event_queue = NULL;
	ALLEGRO_FONT * font = NULL; 

	apmatrix<int> elevation(ROW, COL); 

	apmatrix<int> gradient(ROW, COL);

	//apmatrix<int> path(ROW*COL, ROW*COL);	//sequence table 

	//vector<int> sequence;	//sequence of moves to take (Floyd-Warshall shortest path)

	vector<int> shortestPath(COL); 
	int minTotalChange = INT_MAX;


	initAllegro(&display, &timer, &event_queue, &font); 
	readElevation(elevation); 
	int min = findMin(elevation);
	int max = findMax(elevation);


	drawMap(min, max, elevation, display);

	drawLowestElevPath(elevation, shortestPath, minTotalChange);

	int startRow = indexOfLowestElevPath(shortestPath, minTotalChange);

	drawBestPath(startRow, shortestPath);

	cout << endl; 
	for (int i = 0; i < COL; i++)
		cout << shortestPath[i] << " ";

	//calcFW(elevation, path, sequence);	//calculate Floyd-Warshall shortest path 

	//drawFW(sequence);

	/*bool done = false;
	while (!done) {
		ALLEGRO_EVENT ev;
		al_wait_for_event(event_queue, &ev);
		if (ev.type == ALLEGRO_EVENT_TIMER) {
			//drawFW(path);	//draw Floyd-Warhsall shortest path 
			al_flip_display(); 
			//DONT USE CLEAR DISPLAY SINCE WANT TO KEEP PREVIOUS LINE
		}

	}
	*/
	
	al_rest(10000);
 
    return 0;
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
			cout << i << endl;

			ALLEGRO_COLOR color = al_map_rgb(c, c, c); 
			al_draw_filled_rectangle(j, i, j + 1, i + 1, color);
			al_flip_display();
		}
	}
}

void drawLowestElevPath(const apmatrix<int> &elevation, vector<int> &shortestPath, int &minTotalChange) {
	int currRow, currCol;
	int i, j;

	vector<int> currentPath(COL, -1);

	for (i = 0; i < ROW; i++) {
		currRow = i;
		currCol = 0;

		int totalChange = 0;
		

		currentPath[0] = currCol + COL * currRow;

		for (j = 0; j < COL-1; j++) {
			
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
			nextV[0] = currV + 1; 
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
					int elevChange = abs(elevation[nextI[n]][currCol + 1] - elevation[currRow][currCol]);
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
			currentPath[j + 1] = nextV[nextChangeIndex];


		/*	//0
			elevChange[0] = abs(elevation[currRow][currCol] - elevation[currRow][currCol + 1]);

			if (currRow > 0)
				//1
				elevChange[1] = abs(elevation[currRow][currCol] - elevation[currRow - 1][currCol + 1]);

			if(currRow < ROW-1)
				//2
				elevChangee[2] = abs(elevation[currRow][currCol] - elevation[currRow + 1][currCol + 1]);	*/


			/*sortedChange = elevChange; 
			sort(sortedChange.begin(), sortedChange.end());

			lowestNextChange = sortedChange[0];


			if (elevChange[0] != lowestNextChange) {
				if (elevChange[1] == elevChange[2]) {
					int n = rand() % 2 + 1;
					if (n == 1) {
						if (currRow + 1 < ROW)
							currRow += 1;
						else
							currRow -= 1; 
					}
					else { 
						if (currRow - 1 > -1)
							currRow -= 1;
						else
							currRow += 1;
					}
		
				}
			
				else if (elevationChange[1] == lowestNextChange)
					currRow -= 1;

				else if (elevationChange[2] == lowestNextChange)
					currRow += 1;
			} */

			//totalChange += lowestNextChange;

			/*//convert [currRow][currCol] to vertex sequence for current path 
			currentPath[j] = currCol + COL * currRow;*/

			//convert [currRow][currCol] to vertex sequence for current path 
			//currentPath[j + 1] = (currCol+1) + COL * currRow;
		}


		cout << "i :" << i <<" "<<"Total change: " << totalChange <<endl ;

		if (totalChange < minTotalChange) {
			minTotalChange = totalChange;
			shortestPath = currentPath;
		}

	}

}


int indexOfLowestElevPath(const vector<int> &shortestPath, const int &minTotalChange) {

	int startRow = shortestPath[0] / COL; 

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



