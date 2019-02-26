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



void levelFour_1(const int &min, const int &max, const apmatrix<int> &elevation, ALLEGRO_DISPLAY * display) {
	apmatrix<int> gradient(ROW, COL);
	vector<int> shortestPath(COL);
	int minTotalChange = INT_MAX;

	//purpose of jIndex: adjacent elements in this array are used by the drawLowestElevPath function to determine
	//whether to move eastward or westward; if elements are in ascending order, move east; if elements are in descending order, move west
	int jIndex[COL - 1];
	
	int jDim = COL - 1;
	int iIndex[ROW];

	//i dimension
	int iDim = ROW;

	//number of searches to perform for each row
	//for level 4, starting column is arbitrarily chosen, so must search 
	//both eastward and westward from that point 
	int searchCount = 2;

	//stores the starting row from which the shortest path originates
	int rMin = 0;

	//store the sequence of vertices that comprise the current path 
	vector<int> currentPath(COL, -1);

	//draw mountains 
	drawMap(min, max, elevation, display);

	for (int i = 0; i < iDim; i++) {
		iIndex[i] = i;
	}

	//calculate path for every row (i dimension is simply ROW) 
	for (int i = 0; i < iDim; i++) {

		//currRow is initialized to the starting row 
		int currRow = iIndex[i];
		int totalChange = 0;

		//generate random column to act as starting column; path will propagate from either side of this column (since column can't be 0 or 843)
		int r = rand() % (COL - 2) + 1;

		//this loops first generates a path eastward from the starting column (iteration 1), then generates a path westward from starting column (iteration 2) 
		for (int n = 0; n < searchCount; n++) {
			int count = 0;

			//when n == 0, search will be eastward
			if (n == 0) {
				//starting from jIndex[0], populate subsequent (jDim - r) number of elements of jIndex with values that 
				//represent each column to the RIGHT of the starting point
				for (int j = r; j < jDim; j++) {
					jIndex[count++] = j;
				}
			}

			//when n == 1, search will be westward 
			else {
				for (int j = r; j > 0; j--) {
					//starting from jIndex[0], populate subsequent (jDim - r) number of elements of jIndex with values that 
					//represent each column to the LEFT of the starting point
					jIndex[count++] = j;
				}
			}

			//update total elevation change for current row 
			//note: when 'count' is passed to drawLowestElevPath, it becomes jDim, i.e. the new j dimension, which is the # of columns
			//from starting column to column 0 or 843 (which denote the map boundaries)
			totalChange += drawLowestElevPath(elevation, shortestPath, minTotalChange, currRow, jIndex, count, currentPath);
		}


		//remember current path if it yields smaller elevation change than previous paths 
		if (totalChange < minTotalChange) {
			minTotalChange = totalChange;
			shortestPath = currentPath;

			//on n==2 (moving westward), the rth element of shortestPath will yield the row # at the westmost column, since shortestPath stores column sequence as follows: random starting column (RSC), RSC+1, RSC+2 ... 843, RSC -1, RSC -2 ... R
			rMin = r;	
		}

		cout << "i :" << currRow << " " << "Total change: " << totalChange << endl;
	}


	//find the row when column = 0
	int startRow = indexOfLowestElevPath(shortestPath, minTotalChange, rMin);

	//draw shortest path (in green) 
	drawBestPath(startRow, shortestPath);

	cout << endl;
	for (int i = 0; i < COL; i++)
		cout << shortestPath[i] << " ";

	al_rest(10000);
}