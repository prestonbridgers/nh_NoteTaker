#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <getopt.h>

#include "nhnt.h"

int
main(int argc, char *argv[])
{
    int opt;
    char *name = calloc(64, sizeof *name);
    int name_found = 0;

    while ((opt = getopt(argc, argv, "n:")) != -1) {
        switch(opt) {
            case 'n':
                strncpy(name, optarg, 64);
                name_found = 1;
                break;
            default:
                fprintf(stderr, "Usage: %s -n {character_name}\n", argv[0]);
                exit(1);
                break;
        }
    }
    if (!name_found) {
        fprintf(stderr, "Usage: %s -n {character_name}\n", argv[0]);
        exit(1);
    }

	// nCurses init
    init_curses();

	/* printw("Hello World!"); */
	/* refresh(); */
	/* getch(); */

    /* cleanup_curses(); */

    NT_DATA *data = nt_create(name);
    NT_UI *ui = nt_ui_create(data);
    FILE *f;
    if (name_found) {
        strncat(name, ".txt", 5);
        if ((f = fopen(name, "r")) != NULL) {
            nt_data_load(ui->data, name);
        }
    }
    free(name);

    nt_ui_interact_loop(ui);

    nt_todo_print(ui->data);

    nt_data_serialize(ui->data);

    nt_ui_destroy(ui);
    cleanup_curses();
	return EXIT_SUCCESS;
}
