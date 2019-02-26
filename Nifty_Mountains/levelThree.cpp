#include <iostream>
#include <vector>
#include <allegro5/allegro.h> 
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

//function calculates a path for each row starting from column 0, and 
//determines which path has the least elevation change 
void levelThree(const int &min, const int &max, const apmatrix<int> &elevation, ALLEGRO_DISPLAY * display) {

	//stores the sequence of vertices (not matrix index) that form the shortest path 
	vector<int> shortestPath(COL);

	//total elevation change of path with lowest elevation change 
	int minTotalChange = INT_MAX;

	//jIndex stores values of all columns east of column 0
	//this array is unnecessary for level 3, but required for level 4;
	//it is used here for compatability with the generalized drawLowestPath function
	int jIndex[COL - 1];

	//j dimension; for clarity
	int jDim = COL - 1;

	//i dimension; for clarity 
	int iDim = ROW;

	//number of searches to perform for each row
	//for level 3, starting column is always 0, so only need to perform one search eastward (right)
	int searchCount = 1;

	//stores the row from which the shortest path originates
	int minStartRow = 0;

	//stores the sequence of vertices that form the current path, which starts from a given row 
	vector<int> currentPath(COL, -1);

	//draw mountains
	drawMap(min, max, elevation, display);

	//calculate path for every row  
	for (int i = 0; i < iDim; i++) {

		//currRow is initialized to the starting row 
		int currRow = i;

		//total elevation change for eurrent row 
		int totalChange = 0;

		for (int n = 0; n < searchCount; n++) {
			for (int j = 0; j < jDim; j++) {
				jIndex[j] = j;
			}

			totalChange += drawLowestElevPath(elevation, shortestPath, minTotalChange, currRow, jIndex, jDim, currentPath);
		}

		//remember current path if it yields a smaller elevation change than previous paths 
		if (totalChange < minTotalChange) {
			minTotalChange = totalChange;
			shortestPath = currentPath;
		}

		cout << "Row :" << currRow << "    " << "Total elev. change: " << totalChange << endl;
	}


	//find the row corresponding to column = 0
	int startRow = indexOfLowestElevPath(shortestPath, minTotalChange, minStartRow);

	//draw shortest path (in green) 
	drawBestPath(startRow, shortestPath);

	al_rest(10000);
}
