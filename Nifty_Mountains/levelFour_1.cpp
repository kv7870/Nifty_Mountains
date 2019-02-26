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

//function picks an arbitrary starting column, and performs greedy walk
//to the east and to the west from that point (option 1 of above and beyond) 
void levelFour_1(const int &min, const int &max, const apmatrix<int> &elevation, ALLEGRO_DISPLAY * display) {
	
	//store the sequence of vertices (graph theory) that comprise the current path 
	vector<int> currentPath(COL, -1);

	//stores the sequence of vertices that form the shortest path 
	vector<int> shortestPath(COL);

	//total elevation change of path with lowest elevation change 
	int minTotalChange = INT_MAX;

	//stores values of subsequent columns in a given direction
	//if elements are in ascending order, path is moving eastward, and vice versa 
	int jIndex[COL - 1];
	
	//j dimension; for clarity
	int jDim = COL - 1;

	//i dimension; for clarity 
	int iDim = ROW;

	//number of searches to perform for each row;
	//for level 4 option 1, starting column is arbitrarily chosen, so must search 
	//both eastward and westward from that point, i.e. perform 2 searches per row
	int searchCount = 2;

	//stores the row from which the shortest path originates
	int minStartRow = 0;

	//draw mountains 
	drawMap(min, max, elevation, display);

	//calculate path for every row  
	for (int i = 0; i < iDim; i++) {

		//current row is initialized to the path's starting row
		int currRow = i;

		//total elevation change in current row 
		int totalChange = 0;

		//generate random column to act as starting column; path will propagate to the east and to the west of this column
		int r = rand() % (COL - 2) + 1;

		//this loop first generates a path eastward from the starting column (iteration 1),
		//then generates a path westward from the starting column (iteration 2) 
		for (int n = 0; n < searchCount; n++) {
			int count = 0;

			//when n == 0, search will be eastward
			if (n == 0) {
				//starting from jIndex[0], populate subsequent (jDim - r) number of jIndex elements with values 
				//that represent each column to the RIGHT of the starting point
				for (int j = r; j < jDim; j++) {
					jIndex[count++] = j;
				}
			}

			//when n == 1, search will be westward 
			else {
				for (int j = r; j > 0; j--) {
					//starting from jIndex[0], populate subsequent (jDim - r) number of jIndex elements with values 
					//that represent each column to the LEFT of the starting point
					jIndex[count++] = j;
				}
			}

			//update total elevation change for current row 
			//note: when 'count' is passed to drawLowestElevPath, it becomes jDim, i.e. the new j dimension, which is 
			//the # of columns from the starting column to column 0 or 843 (map boundaries)
			totalChange += drawLowestElevPath(elevation, shortestPath, minTotalChange, currRow, jIndex, count, currentPath);
		}

		//remember current path if it yields a smaller elevation change than previous paths 
		if (totalChange < minTotalChange) {
			minTotalChange = totalChange;
			shortestPath = currentPath;

			//on n==2 (moving westward), the rth element of shortestPath will yield the row # at the westmost column, 
			//since shortestPath stores column sequence as follows: random starting column (RSC), RSC+1, RSC+2 ... 843, RSC -1, RSC -2 ... R
			minStartRow = r;
		}

		
		cout << "row :" << currRow << " " << "Total elev. change: " << totalChange << endl;;
	}


	//find the row corresponding to column = 0
	int startRow = indexOfLowestElevPath(shortestPath, minTotalChange, minStartRow);

	//draw shortest path (in green) 
	drawBestPath(startRow, shortestPath);

	cout << endl;
	for (int i = 0; i < COL; i++)
		cout << shortestPath[i] << " ";

	al_rest(10000);
}
