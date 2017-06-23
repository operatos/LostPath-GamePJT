#include "common.h"

#ifndef _KEY_PROCESSOR_H__
#define _KEY_PROCESSOR_H__

#define RIGHT_KEY 65363
#define LEFT_KEY 65361
#define DOWN_KEY 65364
#define UP_KEY 65362
//#define D_KEY 100

typedef struct ST_KEYPUSH_INFO KEYPUSH_INFO;

struct ST_KEYPUSH_INFO
{
	int LEFTKEY;
	int RIGHTKEY;
	int DOWNKEY;
	int UPKEY;
	//int DKEY;
};

gboolean OnKeyDown(GtkWidget *widget, GdkEventKey *event);
gboolean OnKeyUp(GtkWidget *widget, GdkEventKey *event);
gboolean KeyProcessor(gpointer data);
gint KeyProcessor2(gpointer data);
#endif
