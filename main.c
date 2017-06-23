#include "common.h"
#include <stdlib.h>
#include <cairo.h>
#include "KeyProcessor.h"

#define PROGRAM_TITLE "Lost Path"

char * ltoa(long val, char *buf, unsigned radix);
gboolean expose(GtkWidget *widget, GdkEventExpose *event, gpointer user_data);

GAME_DATA GAMEDATA;

static WINDOW_INFO* NewWindowInfo()
{
	WINDOW_INFO* windowInfo = (WINDOW_INFO*)malloc(sizeof(WINDOW_INFO));
	memset(windowInfo,0,sizeof(WINDOW_INFO));
	return windowInfo;
}
gint OnTimerDrawer(gpointer data)
{
    if(GAMEDATA.isEnd)
    {
        EndGame(data);
        return 0;
    }
    GtkWidget *widget = (GtkWidget*)data;
    
    cairo_t* cr = gdk_cairo_create(widget->window);
    cairo_t* mem_cr;

    cairo_surface_t* img = GAMEDATA.imgData.bgGame;
    int w = cairo_image_surface_get_width (img);
    int h = cairo_image_surface_get_height (img);
    
    cairo_surface_t* mem_img = cairo_image_surface_create(CAIRO_FORMAT_ARGB32,w,h);
    mem_cr = cairo_create(mem_img);
    //create double buffer

    cairo_set_source_surface(mem_cr, img,0, 0);
    cairo_paint(mem_cr);
    //draw map

    int x,y;
    cairo_surface_t* img1 = GAMEDATA.imgData.imgBlock;
    for(x = 0 ; x < MAPPOS_MAXIMUM_X ; x++)
    {
	for(y= 0 ; y<MAPPOS_MAXIMUM_Y ; y++)
	{
	    if(GAMEDATA.arrMapInfo[y][x] == 1)
	    {
		
    		cairo_set_source_surface(mem_cr, img1, POS_ONBOARD_X+x*21, POS_ONBOARD_Y+y*21); 
                //21x21 = block size
    		cairo_paint(mem_cr);
		
	    }
	}
    }
    //draw block

    cairo_surface_t* img2 = GAMEDATA.imgData.imgHero;
    cairo_set_source_surface(mem_cr, img2, GAMEDATA.pos_hero_x, GAMEDATA.pos_hero_y);
    cairo_paint(mem_cr);
    //draw hero
    char strTime[20];
    char strUSec[3];
    char strSec[3];
    //char strHour[3];
    char strMin[3];

    int elapsedSec;
    long elapsedUSec;
    elapsedSec = g_timer_elapsed(GAMEDATA.tmrTime,&elapsedUSec);
    
 
    int min = elapsedSec / 60;
    elapsedSec = elapsedSec % 60;

    unsigned long usec = elapsedUSec / 10000;
    sprintf(strTime,"%02d:%02d:%02d",min,elapsedSec,usec);   

    cairo_select_font_face (mem_cr, "Sans", CAIRO_FONT_SLANT_NORMAL, CAIRO_FONT_WEIGHT_BOLD);
    cairo_set_font_size (mem_cr, 30.0);
    cairo_move_to (mem_cr, 788, 67);
    cairo_set_source_rgb (mem_cr, 0, 0, 0);
    cairo_show_text (mem_cr, strTime);
    


    cairo_set_source_surface(cr, mem_img, 0, 0);
    cairo_paint(cr);
    //draw double buffered img(mem)
    
    cairo_destroy(cr);    
    cairo_destroy(mem_cr);
    cairo_surface_destroy(mem_img);
}
void EndGame(gpointer data)
{
   GtkWidget *widget = (GtkWidget*)data;
   GtkWidget *dialog, *label, *content_area;
   
   
   dialog = gtk_dialog_new_with_buttons( "WOW",
                                              widget->window,
                                              GTK_DIALOG_MODAL,
                                              NULL, 0,
                                              NULL, 0,
                                              NULL );
   content_area = gtk_dialog_get_content_area(GTK_DIALOG (dialog));
   label = gtk_label_new("     Congratulation!     ");
   //gtk_container_add (GTK_CONTAINER (content_area), label);
   
   //g_signal_connect (GTK_DIALOG (dialog), "response", G_CALLBACK (on_msgdlgEvent), NULL);
   
   /* Add the label, and show everything we've added to the dialog. */
   gtk_box_pack_start( GTK_BOX( content_area), label, TRUE, TRUE, 0 );

   
   gtk_widget_show_all(dialog);
   
   WINDOW_INFO* windowInfo = GAMEDATA.windowInfo;
   gtk_widget_show(windowInfo->boxMain);
   gtk_window_resize(windowInfo->window,497,314);
}
static void OnBtnStartGame( GtkWidget *widget, gpointer   data )
{
    GAMEDATA.pos_hero_x = 36+21;
    GAMEDATA.pos_hero_y = 62;
    GAMEDATA.isEnd = 0;
    GAMEDATA.tmrTime = g_timer_new();
    memset(&GAMEDATA.KeyPushInfo,0,sizeof(GAMEDATA.KeyPushInfo));
    g_timer_start(GAMEDATA.tmrTime);
    //initialization game data

    //g_print ("%s was pressed\n", (gchar *) data);
    WINDOW_INFO* windowInfo = (WINDOW_INFO*)data;

    gtk_widget_hide(windowInfo->boxMain);
    gtk_widget_set_size_request(GTK_CONTAINER (windowInfo->window), 955,632);
    CreateMaze(MAPPOS_MAXIMUM_X,MAPPOS_MAXIMUM_Y);
    
    windowInfo->idDrawingTimer  = g_timeout_add (30, OnTimerDrawer, widget); //  1000/30 =  33frame/sec 
    g_signal_connect(windowInfo->window, "key-press-event", G_CALLBACK(OnKeyDown), NULL);
    g_signal_connect(windowInfo->window, "key-release-event", G_CALLBACK(OnKeyUp), NULL);
    //key-input event handler
        

    GThread    *thread2 = g_thread_new ("KeyProcessor", KeyProcessor, data);   
}

static gboolean delete_event( GtkWidget *widget, GdkEvent  *event, gpointer   data )
{
    gtk_main_quit ();
    return FALSE;
}

gboolean expose(GtkWidget *widget, GdkEventExpose *event, gpointer user_data)
{
    cairo_t* cr = gdk_cairo_create(widget->window);
    cairo_surface_t* img = GAMEDATA.imgData.bgMain;
    cairo_set_source_surface(cr, img, 0,0);
    cairo_paint(cr);
    cairo_destroy(cr);
}
void AppStart(WINDOW_INFO* windowInfo)
{
    GAMEDATA.imgData.bgMain = cairo_image_surface_create_from_png("./res/bgMenu.png");
    GAMEDATA.imgData.bgGame = cairo_image_surface_create_from_png("./res/bgGame.png");
    GAMEDATA.imgData.imgBlock = cairo_image_surface_create_from_png("./res/block_1.png");
    GAMEDATA.imgData.imgHero = cairo_image_surface_create_from_png("./res/imgHero.png");
    GAMEDATA.windowInfo = windowInfo;
    
    GtkWidget *button;

    GtkWidget *boxMain = gtk_fixed_new();  
    windowInfo->boxMain = boxMain;
    gtk_widget_set_size_request (boxMain, 497, 314);
    gtk_container_add (GTK_CONTAINER (windowInfo->window), boxMain);

    button = gtk_button_new_with_label ("GameStart");
    gtk_widget_set_size_request (button, 95,30);
    g_signal_connect (button, "clicked", G_CALLBACK (OnBtnStartGame), windowInfo);
    gtk_fixed_put (GTK_FIXED (boxMain), button, 204, 235);  
    gtk_widget_show(button);
    gtk_widget_show(boxMain);
    
    gtk_widget_set_app_paintable(windowInfo->window, TRUE);
    g_signal_connect(windowInfo->window, "expose-event", G_CALLBACK(expose), NULL);
   
    g_signal_new("GameEndEvent",
             G_TYPE_OBJECT, G_SIGNAL_RUN_FIRST,
             0, NULL, NULL,
             g_cclosure_marshal_VOID__POINTER,
             G_TYPE_NONE, 1, G_TYPE_POINTER);
    
}
int main(int argc, char *argv[])
{
    GtkWidget *window;
    WINDOW_INFO* windowInfo = NewWindowInfo();
    gtk_init (&argc, &argv);

    window = gtk_window_new (GTK_WINDOW_TOPLEVEL);
    gtk_window_set_title (GTK_WINDOW (window), PROGRAM_TITLE);
   gtk_window_set_default_size (GTK_CONTAINER(window),497, 314);
   //gtk_widget_set_size_request(GTK_WINDOW (window), 497,314);
   windowInfo->window = window;
   
   AppStart(windowInfo);
   gtk_widget_show(window);
   gtk_main ();
   return 0;
}

char * ltoa(long val, char *buf, unsigned radix)
{
    char *p;            /* pointer to traverse string */
    char *firstdig;        /* pointer to first digit */ 
    char temp;            /* temp char */ 
    unsigned digval;    /* value of digit */

    p = buf; 

    if (radix == 10 && val < 0) { /* negative, so output '-' and negate */
        *p++ = '-';
        val = (unsigned long)(-(long)val);
    }

    firstdig = p;    /* save pointer to first digit */

    do {
        digval = (unsigned) (val % radix);
        val /= radix; /* get next digit */

        /* convert to ascii and store */
        if (digval > 9)
            *p++ = (char) (digval - 10 + 'a');    /* a letter */
        else
            *p++ = (char) (digval + '0');        /* a digit */
    } while (val > 0);

    /* We now have the digit of the number in the buffer, but in reverse order. Thus we reverse them now. */

    *p-- = '\0';    /* terminate string; p points to last digit */

    do {
        temp = *p;
        *p = *firstdig;
        *firstdig = temp; /* swap *p and *firstdig */
        --p;
        ++firstdig; /* advance to next two digits */
        } while (firstdig < p); /* repeat until halfway */

    return buf;
}



