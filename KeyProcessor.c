#include "KeyProcessor.h"
extern void EndGame();

int isEnd(int hero_pos_x,int hero_pos_y)
{
   int onboard_x = hero_pos_x - POS_ONBOARD_X + 10;
   int onboard_y =  hero_pos_y - POS_ONBOARD_Y + 10;
   
   const int MAP_WIDTH = 21*MAPPOS_MAXIMUM_X; // 21 is block size
   const int MAP_HEIGHT = 21*MAPPOS_MAXIMUM_Y;

   int idx_x = onboard_x/21;
   int idx_y = onboard_y/21;
	if(idx_x == MAPPOS_MAXIMUM_X-2 && idx_y == MAPPOS_MAXIMUM_Y-1)
		return 1;
	else
		return 0;
}
int isSmash(int hero_pos_x, int hero_pos_y)
{
   int onboard_x = hero_pos_x - POS_ONBOARD_X + 10;
   int onboard_y =  hero_pos_y - POS_ONBOARD_Y + 10;
   
   const int MAP_WIDTH = 21*MAPPOS_MAXIMUM_X; // 21 is block size
   const int MAP_HEIGHT = 21*MAPPOS_MAXIMUM_Y;

   int idx_x = onboard_x/21;
   int idx_y = onboard_y/21;
   
   
   if(0 > onboard_x || MAP_WIDTH < onboard_x || 0 > onboard_y || MAP_HEIGHT < onboard_y )     
      return 1;
   else if(GAMEDATA.arrMapInfo[idx_y][idx_x] == 1)
	return 1; // smashed
   else
	return 0;
}
gboolean KeyProcessor(gpointer data)
{
  int x,y;
  while(1){
	  if(GAMEDATA.KeyPushInfo.RIGHTKEY)
	  { 
                x = GAMEDATA.pos_hero_x + 1;
                y = GAMEDATA.pos_hero_y;
                if(!isSmash(x+5,y))
                  GAMEDATA.pos_hero_x = x;
	  }
	  if(GAMEDATA.KeyPushInfo.LEFTKEY)
	  {
                x = GAMEDATA.pos_hero_x - 1;
                y = GAMEDATA.pos_hero_y;
                if(!isSmash(x-5,y))
                  GAMEDATA.pos_hero_x = x;
	  }
	  if(GAMEDATA.KeyPushInfo.DOWNKEY)
	  {
                x = GAMEDATA.pos_hero_x ;
                y = GAMEDATA.pos_hero_y+1;
                if(!isSmash(x,y+5))
                  GAMEDATA.pos_hero_y = y;
	  }
	  if(GAMEDATA.KeyPushInfo.UPKEY)
	  {
                x = GAMEDATA.pos_hero_x;
                y = GAMEDATA.pos_hero_y-1;
                if(!isSmash(x,y-5))
                  GAMEDATA.pos_hero_y = y;
	  }
	
	  if(isEnd(x,y))
	  {
		break;
	  }
		
  g_usleep (5000); 
  }
  GAMEDATA.isEnd = 1;
}

gboolean OnKeyDown(GtkWidget *widget, GdkEventKey *event)
{
    //g_printerr("%s keyvalue:%d down!\n", gdk_keyval_name (event->keyval), event->keyval);
    //testCode
   
    switch(event->keyval)
    {
	case RIGHT_KEY:
		GAMEDATA.KeyPushInfo.RIGHTKEY =1;
		break;
	case LEFT_KEY:
		GAMEDATA.KeyPushInfo.LEFTKEY =1;
		break;
	case DOWN_KEY:
		GAMEDATA.KeyPushInfo.DOWNKEY =1;
		break;
	case UP_KEY:
		GAMEDATA.KeyPushInfo.UPKEY =1;
		break;
	
    }

    return FALSE;
}
gboolean OnKeyUp(GtkWidget *widget, GdkEventKey *event)
{
    //g_printerr("%s keyvalue:%d up!\n", gdk_keyval_name (event->keyval), event->keyval);
    //testCode
   
    switch(event->keyval)
    {
	case RIGHT_KEY:
		GAMEDATA.KeyPushInfo.RIGHTKEY =0;
		break;
	case LEFT_KEY:
		GAMEDATA.KeyPushInfo.LEFTKEY =0;
		break;
	case DOWN_KEY:
		GAMEDATA.KeyPushInfo.DOWNKEY =0;
		break;
	case UP_KEY:
		GAMEDATA.KeyPushInfo.UPKEY =0;
		break;
	
    }

    return FALSE;
}
