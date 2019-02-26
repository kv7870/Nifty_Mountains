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

	//store elevation data in matrix 
	apmatrix<int> elevation(ROW, COL);

	//initialize Allegro
	initAllegro(&display, &timer, &event_queue, &font); 

	//read elevation data 
	readElevation(elevation); 

	//find minimum elevation
	int min = findMin(elevation);
	//find maximum elevation
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



//this function, which calculates the path for every row, is generalized to be used by both level 3 and level 4 functions
//for level 3, path always starts at column [0], whereas for level 4, starting column is arbitrarily generated for each row (as per method 1 of above and beyond) 
int drawLowestElevPath(const apmatrix<int> &elevation, vector<int> &shortestPath, int &minTotalChange, int currRow, int jIndex[], int jDim, vector<int> &currentPath) {

	int  jj;

	//currCol stores the current column where path is at, i.e. the current x position 
	int currCol = jIndex[0];

	//total elevation change
	int totalChange = 0;		

	//jShift determine whether to move east or west
	//if elements in jIndex are arranged in ascending order, then jShift will be positive and path will move eastward, & vice versa
	int jShift = jIndex[1] - jIndex[0];

	//convert current (x,y) position to its vertex number on a graph (graph theory); currenPath stores the sequence of vertices that comprise the current path 
	currentPath[jIndex[0]] = currCol + COL * currRow;

	//jDim is the number of columns from starting column to horizontal boundary (0 or 843); recall it equals the value of 'count' from level4 and level3 functions
	for (jj = 0; jj < jDim; jj++) {
		
		//update current column to the next column number, which may be greater (eastward) or smaller (westward) 
		currCol = jIndex[jj];


		//draw pixel at current location of path
		ALLEGRO_COLOR color = al_map_rgb(255, 0, 0);
		al_draw_filled_rectangle(currCol, currRow, currCol + 1, currRow + 1, color);
		al_flip_display();


		//stores the value of the current
		int currV = currCol + COL * currRow; //convert current (x,y) coordinate to corresponding vertex value (on a graph) 

		//read: "i plus 1"; stores the value of row below for convenience
		int ip1 = currRow + 1;
		//read: "i minus 1"; stores the value of row above for convenience
		int im1 = currRow - 1;

		//the lowest elevation change of the three possible next steps to take
		int lowestNextChange = INT_MAX;

		//tracks which of the three possible vertices yields the smallest elevation change 
		int nextChangeIndex  = 0;

		//read: "next vertex"; stores the values of the three possible vertices to move to (up forward, straight forward, down forward)
		//***NOTE ON CONVENTION: [0] = vertex straight ahead, [1] = vertex up forward, [2] = vertex down forward
		int nextV[3] = { -1,-1,-1 };

		//stores the corresponding row numbers (y-coordinates) of the three possible next steps 
		int nextI[3] = { -1,-1,-1 };	//i = row number 

		//the elevation change incurred by moving to each of the three possible vectors 
		int nextChange[3] = { INT_MAX, INT_MAX, INT_MAX };

		//value of vertex straight ahead is simply current vector +1 or -1, depending on direction of the path 
		nextV[0] = currV + jShift;	//moving directly ahead will never go out of bounds (when row < 0 or row > 843), so don't need guard
		//index of vertex straight ahead is simply the current row (since row doesn't change when moving directly foward)
		nextI[0] = currRow;

		//***the following two conditions guards against moving out of bounds***
		//if moving up a row is legal
		if (im1 > -1) {
			//set the up forward vertex to the correct value
			nextV[1] = nextV[0] - COL;
			nextI[1] = im1;
		}

		//if moving down a row is legal 
		if (ip1 < ROW) {
			//set the down forward vertex to the correct value
			nextV[2] = nextV[0] + COL;
			nextI[2] = ip1;
		}
			

		//draw pixel at current location of path
		/*ALLEGRO_COLOR color = al_map_rgb(255, 0, 0);
		al_draw_filled_rectangle(currCol, currRow, currCol + 1, currRow + 1, color);
		al_flip_display();*/


		//decide which vertex to move to
		//recall: [0] = ahead, [1] = up, [2] = down 
		for (int n = 0; n < 3; n++) {
			//if next vertex is within bounds (i.e. has a value different than default -1)
			if (nextV[n] > -1) {
				//calculate elevation change between that vertex and current vertex
				int elevChange = abs(elevation[nextI[n]][currCol + jShift] - elevation[currRow][currCol]);

				//store the elevation change for this possible vertex 
				nextChange[n]  = elevChange;

				if (lowestNextChange > elevChange) {
					lowestNextChange = elevChange;

					//track the vertex that yields the smallest elevation change 
					nextChangeIndex = n;
				}
			}
		}

		//if moving straight ahead does not require the least elevation change 
		if (nextChangeIndex != 0) {
			//flip a coin if moving up and moving down yield the same elevation change
			if (nextChange[1] == nextChange[2]) {
				nextChangeIndex = rand() % 2 + 1;
			}
		}
		//if tie between directly forward or diagonally forward, directly forward will be
		//automatically selected since nextChangeIndex was initialized to 0 
		

		//add elevation change of next step to total elevation change for this path 
		totalChange += lowestNextChange;


		//update current row to the next row that results in the least elevation change 
		currRow = nextI[nextChangeIndex];

		//append value of new vertex into currentPath, which stores the sequence of vertices 
		currentPath[currCol+jShift] = nextV[nextChangeIndex];
		//cout << "j+jShift: " << j + jShift << " " << "jj: " << jj << " " << "jDim: " << jDim << " " << endl; 
	}

	return totalChange;		//total elevation change for this path
}



//returns the starting row (column = 0) from which the shortest path originates 
int indexOfLowestElevPath(const vector<int> &shortestPath, const int &minTotalChange, int rMin) {

	//convert vertex of starting point to y-coordinate 
	int startRow = shortestPath[rMin] / COL; 

	cout << "shortest elevation change: "<< minTotalChange<<" meters"<<endl; 
	cout << "start row: " << startRow << endl; 
	cout << "last vertex: " << shortestPath[COL - 1] << endl;

	return startRow;
}













