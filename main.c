#include <stdio.h>
#include <stdlib.h>
#include <ncurses.h>

int
init_curses(void)
{
	initscr();
    raw();
    noecho();
    keypad(stdscr, TRUE);
    return EXIT_SUCCESS;
}

int
cleanup_curses(void)
{
    endwin();
    return EXIT_SUCCESS;
}

int
main(int argc, char *argv[])
{
	// nCurses init
    init_curses();

	printw("Hello World!");
	refresh();
	getch();

    cleanup_curses();

	return EXIT_SUCCESS;
}
