#ifndef __COMMON_H__
#define __COMMON_H__

#ifndef NULL
#define NULL 0
#endif

#include <gtk/gtk.h>
#include "KeyProcessor.h"

#define MAPPOS_MAXIMUM_X 35
#define MAPPOS_MAXIMUM_Y 25

#define POS_ONBOARD_X 36
#define POS_ONBOARD_Y 62

typedef struct ST_GAME_DATA GAME_DATA;
typedef struct ST_WINDOW_INFO WINDOW_INFO;
typedef struct ST_IMG_DATA IMG_DATA;

struct ST_IMG_DATA
{
	cairo_surface_t* bgMain;
	cairo_surface_t* bgGame;
	cairo_surface_t* imgBlock;
	cairo_surface_t* imgHero;
};
struct ST_GAME_DATA
{
	int pos_hero_x;
	int pos_hero_y;
	int arrMapInfo[MAPPOS_MAXIMUM_Y][MAPPOS_MAXIMUM_X];
	KEYPUSH_INFO KeyPushInfo;
	IMG_DATA imgData;
        GTimer* tmrTime;
	int isEnd;
        WINDOW_INFO* windowInfo;
};


struct ST_WINDOW_INFO
{ 
    GtkWidget *window;
    GtkWidget *boxMain;
    GtkWidget *boxGame;
    int idDrawingTimer;
};


extern GAME_DATA GAMEDATA;

#endif

