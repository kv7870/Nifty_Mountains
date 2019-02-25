#include "stdafx.h"
#include <iostream>
#include <vector>
#include <allegro5/allegro.h> 
#include <allegro5/allegro_font.h>
#include <allegro5/allegro_ttf.h>
#include <allegro5/allegro_image.h>
#include <allegro5/allegro_native_dialog.h>
#include <allegro5/allegro_primitives.h>
#include "header.h"		

int initAllegro(ALLEGRO_DISPLAY ** display, ALLEGRO_TIMER ** timer,
ALLEGRO_EVENT_QUEUE ** event_queue, ALLEGRO_FONT ** font) {

	al_init();

	al_install_keyboard();
	al_install_mouse();
	al_init_font_addon();
	al_init_ttf_addon();
	al_init_primitives_addon();
	al_init_image_addon();
	al_init_font_addon();
	al_init_ttf_addon();

	*display = al_create_display(844, 480);
	*timer = al_create_timer(1.0 / 60);   //1/60 seconds per frame, i.e. 60 fps
	*event_queue = al_create_event_queue(); 
	*font = al_load_ttf_font("OpenSans-Regular", 24, 0);



	//register 
	al_register_event_source(*event_queue, al_get_display_event_source(*display));
	al_register_event_source(*event_queue, al_get_timer_event_source(*timer));
	al_register_event_source(*event_queue, al_get_keyboard_event_source());
	al_register_event_source(*event_queue, al_get_mouse_event_source());

	al_clear_to_color(al_map_rgb(0, 0, 0));
	al_start_timer(*timer);
	
	return 0; 

}