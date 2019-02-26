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

//calculates path starting from column 0 of every row 
void levelThree(const int &min, const int &max, const apmatrix<int> &elevation, ALLEGRO_DISPLAY * display) {

	apmatrix<int> gradient(ROW, COL);

	//stores the sequence of vertices (not matrix index) that form the shortes path 
	vector<int> shortestPath(COL);

	//total elevation change of path with lowest elevation change 
	int minTotalChange = INT_MAX;


	int jIndex[COL - 1];

	int jDim = COL - 1;
	int iIndex[ROW];

	//i dimension 
	int iDim = ROW;

	//number of searches to perform for each row
	//for level 3, starting column is always 0, so only need to perform one search eastward (right)
	int searchCount = 1;
	int rMin = 0;

	//stores the sequence of vertices that form the current path, which starts from a given row 
	vector<int> currentPath(COL, -1);

	//draw mountains
	drawMap(min, max, elevation, display);

	//initialize iIndex
	for (int i = 0; i < iDim; i++) {
		iIndex[i] = i;
	}

	for (int ii = 0; ii < iDim; ii++) {

		//currRow
		int currRow = iIndex[ii];
		int totalChange = 0;

		for (int n = 0; n < searchCount; n++) {
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