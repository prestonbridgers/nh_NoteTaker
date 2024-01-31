#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include "nhnt.h"

int
main(int argc, char *argv[])
{
	// nCurses init
    init_curses();

	/* printw("Hello World!"); */
	/* refresh(); */
	/* getch(); */

    /* cleanup_curses(); */

    NT_DATA *data = nt_create("bridgerspc");
    NT_UI *ui = nt_ui_create(data);

    nt_shock_toggle(data);
    nt_ui_data_draw(ui);
    sleep(1);

    nt_sleep_toggle(data);
    nt_ui_data_draw(ui);
    sleep(1);

    nt_fire_toggle(data);
    nt_ui_data_draw(ui);
    sleep(1);

    nt_cold_toggle(data);
    nt_ui_data_draw(ui);
    sleep(1);

    nt_poison_toggle(data);
    nt_ui_data_draw(ui);
    sleep(1);

    nt_disintegration_toggle(data);
    nt_ui_data_draw(ui);
    sleep(1);

    nt_magic_toggle(data);
    nt_ui_data_draw(ui);
    sleep(1);

    getch();
    
    nt_ui_destroy(ui);
    cleanup_curses();
	return EXIT_SUCCESS;
}
