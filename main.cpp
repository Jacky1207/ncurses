#include "sshWindow.h"
#include <ncurses.h>

#include <signal.h>

#define ENTER		10

IWindow *g_pWindow = NULL;

void sighandler(int sig)
{
	PRINTF("SIGNAL %d", sig);
}


int main(int argc , char *argv[])
{
	#ifdef DEBUG
	initLog();
	#endif
	//init ncurses
	initscr();
    noraw();
    cbreak();
    noecho();
	keypad(stdscr, TRUE);

	/**
	 *	catch the ctrl+C
	 *
	 */
    signal(SIGABRT, sighandler);
	signal(SIGTERM, sighandler);
	signal(SIGINT, sighandler);
	signal(SIGQUIT, sighandler);
	
	PRINTF("clo = %d, lin = %d\n", COLS, LINES);
	g_pWindow = new IWindow();
	g_pWindow->init();
	
	PRINTF("start EventFilter!");

	int c = 0;
	while((c = getch()) != KEY_F(1))
	{
		switch(c)
		{
			case KEY_DOWN:	//258
			case KEY_UP:	//259
			{
				g_pWindow->startEventFilter(MAINMENU, c);
				break;
			}
			case KEY_LEFT:	//260
			case KEY_RIGHT:	//261
			case ENTER:		//10
			{
				g_pWindow->startEventFilter(SUBMENU, c);
				break;
			}
			default: //other key value
				g_pWindow->startEventFilter(MAINMENU, c);
				break;
		}
	}

	#ifdef DEBUG
	exitLog();
	#endif

	return 0;
}
