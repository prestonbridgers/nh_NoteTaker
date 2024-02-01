#include <stdlib.h>
#include <stdio.h>

#include "nhnt.h"

uint8_t
init_curses(void)
{
	initscr();
    raw();
    noecho();
    curs_set(0);
    keypad(stdscr, TRUE);
    return EXIT_SUCCESS;
}

uint8_t
cleanup_curses(void)
{
    endwin();
    return EXIT_SUCCESS;
}

NT_UI*
nt_ui_create(NT_DATA *data)
{
    NT_UI *ui = calloc(1, sizeof *ui);

    ui->data = data;

    // Main Window Initialization
    ui->main_height = LINES;
    ui->main_width = COLS;
    ui->main_y = (LINES - ui->main_height) / 2;
    ui->main_x = (COLS - ui->main_width) / 2;
    ui->W_main = newwin(
        ui->main_height, ui->main_width,
        ui->main_y, ui->main_x
    );

    // Help Window Initialization
    ui->help_height = 10;
    ui->help_width = 10;
    ui->help_y = (LINES - ui->help_height) / 2;
    ui->help_x = (COLS - ui->help_width) / 2;
    ui->W_help = newwin(
        ui->help_height, ui->help_width,
        ui->help_y, ui->help_x
    );

    // Resistances Window Initialization
    ui->resistances_height = ui->main_height - 8;
    ui->resistances_width = ui->main_width / 6;
    ui->resistances_y = (ui->main_height - ui->resistances_height) / 2;
    ui->resistances_x = 4;
    ui->W_resistances = derwin(
        ui->W_main,
        ui->resistances_height, ui->resistances_width,
        ui->resistances_y, ui->resistances_x
    );

    // Panel Creation
    ui->P_help = new_panel(ui->W_help);
    ui->P_main = new_panel(ui->W_main);

    return ui;
}

uint8_t
nt_ui_destroy(NT_UI *ui)
{
    nt_destroy(ui->data);
    del_panel(ui->P_main);
    del_panel(ui->P_help);
    delwin(ui->W_main);
    delwin(ui->W_help);
    free(ui);
    return EXIT_SUCCESS;
}

uint8_t
nt_ui_draw_title(WINDOW *win, uint32_t win_width, char *text, size_t len)
{
    uint16_t title_x = (win_width - len) / 2;
    uint16_t title_y = 0;

    mvwprintw(win, title_y, title_x, "%s", text);

    return EXIT_SUCCESS;
}

uint8_t
nt_ui_draw_list(WINDOW *win, uint32_t y, uint32_t x, char *text, uint32_t has, char letter)
{
    char buffer[256];

    if (has) {
        snprintf(buffer, 256, "[x] %c - %s", letter, text);
    } else {
        snprintf(buffer, 256, "[ ] %c - %s", letter, text);
    }

    mvwprintw(
        win, y, x,
        "%s", buffer
    );

    return EXIT_SUCCESS;
}

uint8_t
nt_ui_draw_main_title(NT_UI *ui)
{
    uint32_t col;
    
    for (col = 1; col < ui->main_width - 2; col++) {
        mvwaddch(ui->W_main, 2, col, ACS_HLINE);
    }

    mvwprintw(ui->W_main, 1, (ui->main_width - 24) / 2,
        "NetHack NoteTaker v0.1.0");

    return EXIT_SUCCESS;
}

uint8_t
nt_ui_data_draw(NT_UI *ui)
{
    uint8_t list_y = 1;
    uint8_t list_x = 2;

    // Draw boxes first so nothing overlaps them
    box(ui->W_main, ACS_VLINE, ACS_HLINE);
    box(ui->W_help, ACS_VLINE, ACS_HLINE);
    box(ui->W_resistances, ACS_VLINE, ACS_HLINE);

    nt_ui_draw_main_title(ui);

    // Drawing Resistances State
    nt_ui_draw_title(
        ui->W_resistances, ui->resistances_width,
        "Resistances", 11);
    nt_ui_draw_list(ui->W_resistances, list_y + 1, list_x, "Shock",
        ui->data->has_shock_resistance,
        's');
    nt_ui_draw_list(ui->W_resistances, list_y + 2, list_x, "Fire",
        ui->data->has_fire_resistance,
        'f');
    nt_ui_draw_list(ui->W_resistances, list_y + 3, list_x, "Cold",
        ui->data->has_cold_resistance,
        'c');
    nt_ui_draw_list(ui->W_resistances, list_y + 4, list_x, "Poison",
        ui->data->has_poison_resistance,
        'p');
    nt_ui_draw_list(ui->W_resistances, list_y + 5, list_x, "Sleep",
        ui->data->has_sleep_resistance,
        'l');
    nt_ui_draw_list(ui->W_resistances, list_y + 6, list_x, "Disintegration",
        ui->data->has_disintegration_resistance,
        'd');
    nt_ui_draw_list(ui->W_resistances, list_y + 7, list_x, "Magic",
        ui->data->has_magic_resistance,
        'm');

    update_panels();
    doupdate();

    return EXIT_SUCCESS;
}

uint8_t
nt_ui_interact_loop(NT_UI *ui) {
    uint8_t input;
    uint8_t is_running = 1;

    // Initial render
    nt_ui_data_draw(ui);

    while (is_running) {
        input = getch();
        switch (input) {
            case 't':
                nt_ui_toggle_resistance(ui, getch());
                break;
            case 'q':
                is_running = 0;
                break;
            default:
                break;
        }
        nt_ui_data_draw(ui);
    }
    return EXIT_SUCCESS;
}

uint8_t
nt_ui_toggle_resistance(NT_UI *ui, char res)
{
    switch (res) {
        case 'p':
            nt_poison_toggle(ui->data);
            break;
        case 'c':
            nt_cold_toggle(ui->data);
            break;
        case 'd':
            nt_disintegration_toggle(ui->data);
            break;
        case 's':
            nt_shock_toggle(ui->data);
            break;
        case 'l':
            nt_sleep_toggle(ui->data);
            break;
        case 'm':
            nt_magic_toggle(ui->data);
            break;
        case 'f':
            nt_fire_toggle(ui->data);
            break;
        default:
            break;
    }
    return EXIT_SUCCESS;
}






