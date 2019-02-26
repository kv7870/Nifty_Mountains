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

//draws the shortest path
void drawBestPath(const int &startRow, const vector<int> &shortestPath) {

	//current position
	int currRow, currCol;

	for (int j = 0; j < COL; j++) {

		//converts vertex number to matrix index 
		currRow = shortestPath[j] / COL;
		currCol = shortestPath[j] - (currRow * COL);	//same as: shortesPath[j] % COL; 

		ALLEGRO_COLOR color = al_map_rgb(0, 255, 0);
		al_draw_filled_rectangle(currCol, currRow, currCol + 1, currRow + 1, color);
		al_flip_display();
	}
}

void drawMap(const int &min, const int &max, const apmatrix<int> &elevation, ALLEGRO_DISPLAY * display) {
	int c; //calculated grayscale value 
	int i, j;
	float weightedShade;

	if (max != min)
		weightedShade = 255. / (max - min);

	for (i = 0; i < ROW; i++) {
		for (j = 0; j < COL; j++) {
			c = round((elevation[i][j] - min)*weightedShade);

			ALLEGRO_COLOR color = al_map_rgb(c, c, c);
			al_draw_filled_rectangle(j, i, j + 1, i + 1, color);
			al_flip_display();
		}
	}
}
		



