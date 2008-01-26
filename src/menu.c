/*
 * See Licensing and Copyright notice in naev.h
 */



#include "menu.h"

#include <string.h>

#include "SDL.h"

#include "toolkit.h"
#include "log.h"
#include "naev.h"
#include "pause.h"
#include "pilot.h"
#include "space.h"
#include "player.h"
#include "plasmaf.h"


#define MAIN_WIDTH		130
#define MAIN_HEIGHT		250

#define MENU_WIDTH		130
#define MENU_HEIGHT		200

#define INFO_WIDTH		360
#define INFO_HEIGHT		280

#define OUTFITS_WIDTH	400
#define OUTFITS_HEIGHT	200

#define DEATH_WIDTH		130
#define DEATH_HEIGHT		150

#define BUTTON_WIDTH  	90
#define BUTTON_HEIGHT 	30

#define menu_Open(f)		(menu_open |= (f))
#define menu_Close(f)	(menu_open ^= (f))
int menu_open = 0;


/*
 * prototypes
 */
static void menu_main_close (void);
static void menu_main_new( char* str );
static void menu_small_close( char* str );
static void edit_options (void);
static void exit_game (void);
static void menu_info_close( char* str );
static void info_outfits_menu( char* str );
static void info_outfits_menu_close( char* str );
static void menu_death_main( char* str );



void menu_main (void)
{
	unsigned int bwid, wid;
	glTexture *tex;

	tex = pf_genFractal( gl_screen.w, gl_screen.h, 10. );

	/* create background image window */
	bwid = window_create( "BG", -1, -1, gl_screen.w, gl_screen.h );
	window_addRect( bwid, 0, 0, gl_screen.w, gl_screen.h, "rctBG", &cBlack, 0 );
	window_addImage( bwid, 0, 0, "imgBG", tex, 0 );
	window_imgColour( bwid, "imgBG", &cPurple );

	/* create menu window */
	wid = window_create( "Main Menu", -1, -1, MAIN_WIDTH, MAIN_HEIGHT );
	window_addButton( wid, 20, 20 + (BUTTON_HEIGHT+20)*3,
			BUTTON_WIDTH, BUTTON_HEIGHT,
			"btnLoad", "Load Game", NULL );
	window_addButton( wid, 20, 20 + (BUTTON_HEIGHT+20)*2,
			BUTTON_WIDTH, BUTTON_HEIGHT,
			"btnNew", "New Game", menu_main_new );
	window_addButton( wid, 20, 20 + (BUTTON_HEIGHT+20),
			BUTTON_WIDTH, BUTTON_HEIGHT,
			"btnOptions", "Options", (void(*)(char*)) edit_options );
	window_addButton( wid, 20, 20, BUTTON_WIDTH, BUTTON_HEIGHT,
			"btnExit", "Exit", (void(*)(char*)) exit_game );

	menu_Open(MENU_MAIN);
}
static void menu_main_close (void)
{
	window_destroy( window_get("Main Menu") );

	gl_freeTexture( window_getImage( window_get("BG"), "imgBG" ) );
	window_destroy( window_get("BG") );

	menu_Close(MENU_MAIN);
}
static void menu_main_new( char* str )
{
	(void)str;

	menu_main_close();
	player_new();
}


/*
 *
 * ingame menu
 *
 */
/*
 * the small ingame menu
 */
void menu_small (void)
{
	if ( menu_isOpen(MENU_MAIN) ||
			menu_isOpen(MENU_SMALL) ||
			menu_isOpen(MENU_DEATH) )
		return; /* menu is already open */
	pause();

	unsigned int wid;
	wid = window_create( "Menu", -1, -1, MENU_WIDTH, MENU_HEIGHT );


	window_addButton( wid, 20, 20 + BUTTON_HEIGHT*2 + 20*2,
			BUTTON_WIDTH, BUTTON_HEIGHT,
			"btnResume", "Resume", menu_small_close );
	window_addButton( wid, 20, 20 + BUTTON_HEIGHT + 20,
			BUTTON_WIDTH, BUTTON_HEIGHT,
			"btnOptions", "Options", (void(*)(char*)) edit_options );
	window_addButton( wid, 20, 20, BUTTON_WIDTH, BUTTON_HEIGHT, 
			"btnExit", "Exit", (void(*)(char*)) exit_game );

	menu_Open(MENU_SMALL);
}
static void menu_small_close( char* str )
{
	if (strcmp(str,"btnResume")==0)
		window_destroy( window_get("Menu") );

	unpause();
	menu_Close(MENU_SMALL);
}

/*
 * edits the options
 */
static void edit_options (void)
{
	/* TODO make options menu */
}


/*
 * exits the game
 */
static void exit_game (void)
{
	SDL_Event quit;
	quit.type = SDL_QUIT;
	SDL_PushEvent(&quit);
}



/*
 * 
 * information menu
 *
 */
void menu_info (void)
{
	if (menu_isOpen(MENU_INFO)) return;
	pause();

	char str[128];
	unsigned int wid;
	wid = window_create( "Info", -1, -1, INFO_WIDTH, INFO_HEIGHT );

	/* pilot generics */
	window_addText( wid, 20, 20, 120, INFO_HEIGHT-60,
			0, "txtDPilot", &gl_smallFont, &cDConsole,
			"Pilot:\n"
			"Combat\n"
			" Rating:\n"
			"\n"
			"Ship:\n"
			);
	snprintf( str, 128, 
			"%s\n"
			"\n"
			"%s\n"
			"\n"
			"%s\n"
			, player_name, player_rating(), player->name );
	window_addText( wid, 80, 20,
			INFO_WIDTH-120-BUTTON_WIDTH, INFO_HEIGHT-60,
			0, "txtPilot", &gl_smallFont, &cBlack, str );

	/* menu */
	window_addButton( wid, -20, (20 + BUTTON_HEIGHT)*4 + 20,
			BUTTON_WIDTH, BUTTON_HEIGHT,
			player->ship->name, "Ship", ship_view );
	window_addButton( wid, -20, (20 + BUTTON_HEIGHT)*3 + 20,
			BUTTON_WIDTH, BUTTON_HEIGHT,
			"btnOutfits", "Outfts", info_outfits_menu );
	window_addButton( wid, -20, (20 + BUTTON_HEIGHT)*2 + 20,      
			BUTTON_WIDTH, BUTTON_HEIGHT,    
			"btnCargo", "Cargo", NULL );
	window_addButton( wid, -20, 20 + BUTTON_HEIGHT + 20,
			BUTTON_WIDTH, BUTTON_HEIGHT,
			"btnMissions", "Missions", NULL );
	window_addButton( wid, -20, 20,
			BUTTON_WIDTH, BUTTON_HEIGHT,
			"btnClose", "Close", menu_info_close );

	menu_Open(MENU_INFO);
}
static void menu_info_close( char* str )
{
	if (strcmp(str,"btnClose")==0)
		window_destroy( window_get("Info") );

	menu_Close(MENU_INFO);
	unpause();
}


/*
 * shows the player what outfits he has
 */
static void info_outfits_menu( char* str )
{
	(void)str;
	char *buf;
	unsigned int wid;
	wid = window_create( "Outfits", -1, -1, OUTFITS_WIDTH, OUTFITS_HEIGHT );

	window_addText( wid, 20, -40, 100, OUTFITS_HEIGHT-40,
			0, "txtLabel", &gl_smallFont, &cDConsole,
			"Ship Outfits:" );

	buf = pilot_getOutfits( player );
	window_addText( wid, 20, -45-gl_smallFont.h,
			OUTFITS_WIDTH-40, OUTFITS_HEIGHT-60,
			0, "txtOutfits", &gl_smallFont, &cBlack, buf );
	free(buf);
	
	window_addButton( wid, -20, 20,
			BUTTON_WIDTH, BUTTON_HEIGHT,
			"closeOutfits", "Close", info_outfits_menu_close );
}
static void info_outfits_menu_close( char* str )
{
	window_destroy( window_get( str+5 /* "closeFoo -> Foo" */ ) );
}


/*
 * pilot died
 */
void menu_death (void)
{
	unsigned int wid;
	
	wid = window_create( "Death", -1, -1, DEATH_WIDTH, DEATH_HEIGHT );
	window_addButton( wid, 20, 20 + (BUTTON_HEIGHT+20),
			BUTTON_WIDTH, BUTTON_HEIGHT,
			"btnMain", "MainMenu", menu_death_main );
	window_addButton( wid, 20, 20, BUTTON_WIDTH, BUTTON_HEIGHT,
			"btnExit", "Exit", (void(*)(char*)) exit_game );
	menu_Open(MENU_DEATH);
}
static void menu_death_main( char* str )
{
	(void)str;
	unsigned int wid;

	wid = window_get( "Death" );
	window_destroy( wid );
	menu_Close(MENU_DEATH);

	menu_main();
}


