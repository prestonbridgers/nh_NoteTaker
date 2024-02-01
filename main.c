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
    
    nt_ui_interact_loop(ui);

    nt_data_serialize(ui->data);

    nt_ui_destroy(ui);
    cleanup_curses();
	return EXIT_SUCCESS;
}
