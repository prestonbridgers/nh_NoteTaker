#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

int
main(int argc, char *argv[])
{
	// nCurses init
	initscr();
	printw("Hello World!");
	refresh();
	getch();
	endwin();

	return EXIT_SUCCESS;
}
