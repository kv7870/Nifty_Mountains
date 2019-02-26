#include "stdafx.h"
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

//initialize Allegro
int initAllegro(ALLEGRO_DISPLAY ** display, ALLEGRO_EVENT_QUEUE ** event_queue) {

	al_init();

	al_init_primitives_addon();

	*display = al_create_display(844, 480);
	*event_queue = al_create_event_queue(); 

	//register 
	al_register_event_source(*event_queue, al_get_display_event_source(*display));

	al_clear_to_color(al_map_rgb(0, 0, 0));

	return 0; 
}


//read in elevation data 
int readElevation(apmatrix<int> &elevation) {
	ifstream fin("colorado.dat");

	if (!fin) {
		cout << "No file";
		return -1;
	}

	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			fin >> elevation[i][j];
		}
	}
	return 0;
}

//return minimum elevation 
int findMin(const apmatrix<int> &elevation) {
	int min = 32700;
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (elevation[i][j] < min)
				min = elevation[i][j];
		}
	}
	return min;
}

//return maximimum elevation 
int findMax(const apmatrix<int> &elevation) {
	int max = 0;
	for (int i = 0; i < ROW; i++) {
		for (int j = 0; j < COL; j++) {
			if (elevation[i][j] > max)
				max = elevation[i][j];
		}
	}
	return max;
}
