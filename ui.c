#include <stdlib.h>
#include <stdio.h>

#include "nhnt.h"

uint8_t
init_curses(void)
{
	initscr();
    if (has_colors() == FALSE) {
        endwin();
        fprintf(stderr, "Terminal does not support colors. Exiting...\n");
        exit(1);
    }

    start_color();
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
    ui->resistances_height = (ui->main_height - 8) / 2;
    ui->resistances_width = ui->main_width / 6;
    ui->resistances_y = (ui->main_height - ui->resistances_height) / 2 - 8;
    ui->resistances_x = 4;
    ui->W_resistances = derwin(
        ui->W_main,
        ui->resistances_height, ui->resistances_width,
        ui->resistances_y, ui->resistances_x
    );

    // Abilities Window Initialization
    ui->abilities_height = (ui->main_height - 8) / 2;
    ui->abilities_width = ui->main_width / 6;
    ui->abilities_y = (ui->main_height - ui->abilities_height) / 2 - 8 + ui->abilities_height;
    ui->abilities_x = 4;
    ui->W_abilities = derwin(
        ui->W_main,
        ui->abilities_height, ui->abilities_width,
        ui->abilities_y, ui->abilities_x
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
nt_ui_draw_list(WINDOW *win, uint32_t y, uint32_t x, char *text, uint32_t has,
    char letter)
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
nt_ui_draw_main_hints(NT_UI *ui, char *text, size_t len)
{
    uint32_t col;
    
    for (col = 1; col < ui->main_width - 2; col++) {
        mvwaddch(ui->W_main, ui->main_height - 3, col, ACS_HLINE);
        mvwaddch(ui->W_main, ui->main_height - 2, col, ' ');
    }

    if (text == NULL) {
        mvwprintw(ui->W_main, ui->main_height - 2,
            (ui->main_width - 55) / 2,
            "r - toggle resistance  | a - toggle ability |  q - quit");
    } else {
        mvwprintw(ui->W_main, ui->main_height - 2,
            (ui->main_width - len) / 2,
            "%s", text);
    }

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
    box(ui->W_abilities, ACS_VLINE, ACS_HLINE);

    nt_ui_draw_main_title(ui);
    nt_ui_draw_main_hints(ui, NULL, 0);

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

    // Drawing Ability State
    nt_ui_draw_title(
        ui->W_abilities, ui->abilities_width,
        "Abilities", 9);
    nt_ui_draw_list(ui->W_abilities, list_y + 1, list_x, "Infravision",
        ui->data->has_infravision,
        'a');
    nt_ui_draw_list(ui->W_abilities, list_y + 2, list_x, "Invisibility",
        ui->data->has_invisibility,
        'b');
    nt_ui_draw_list(ui->W_abilities, list_y + 3, list_x, "Reflection",
        ui->data->has_reflection,
        'c');
    nt_ui_draw_list(ui->W_abilities, list_y + 4, list_x, "Searching",
        ui->data->has_searching,
        'd');
    nt_ui_draw_list(ui->W_abilities, list_y + 5, list_x, "See Invisible",
        ui->data->has_see_invisible,
        'e');
    nt_ui_draw_list(ui->W_abilities, list_y + 6, list_x, "Speed 1",
        ui->data->has_speed1,
        'f');
    nt_ui_draw_list(ui->W_abilities, list_y + 7, list_x, "Speed 2",
        ui->data->has_speed2,
        'g');
    nt_ui_draw_list(ui->W_abilities, list_y + 8, list_x, "Stealth",
        ui->data->has_stealth,
        'h');
    nt_ui_draw_list(ui->W_abilities, list_y + 9, list_x, "Telepathy",
        ui->data->has_telepathy,
        'i');
    nt_ui_draw_list(ui->W_abilities, list_y + 10, list_x, "Teleport Control",
        ui->data->has_teleport_control,
        'j');
    nt_ui_draw_list(ui->W_abilities, list_y + 11, list_x, "Teleportitis",
        ui->data->has_teleportitis,
        'k');
    nt_ui_draw_list(ui->W_abilities, list_y + 12, list_x, "Warning",
        ui->data->has_warning,
        'l');

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
            case 'r':
                nt_ui_draw_main_hints(ui,
                    "Toggling Resistance: Choose a resistance",
                    40);
                update_panels();
                doupdate();
                nt_ui_toggle_resistance(ui, getch());
                break;
            case 'a':
                nt_ui_draw_main_hints(ui,
                    "Toggling Ability: Choose an ability",
                    35);
                update_panels();
                doupdate();
                nt_ui_toggle_ability(ui, getch());
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
nt_ui_toggle_resistance(NT_UI *ui, char abil)
{
    switch (abil) {
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

uint8_t
nt_ui_toggle_ability(NT_UI *ui, char res)
{
    switch (res) {
        case 'a':
            nt_infravision_toggle(ui->data);
            break;
        case 'b':
            nt_invisibility_toggle(ui->data);
            break;
        case 'c':
            nt_reflection_toggle(ui->data);
            break;
        case 'd':
            nt_searching_toggle(ui->data);
            break;
        case 'e':
            nt_see_invisible_toggle(ui->data);
            break;
        case 'f':
            nt_speed1_toggle(ui->data);
            break;
        case 'g':
            nt_speed2_toggle(ui->data);
            break;
        case 'h':
            nt_stealth_toggle(ui->data);
            break;
        case 'i':
            nt_telepathy_toggle(ui->data);
            break;
        case 'j':
            nt_teleport_control_toggle(ui->data);
            break;
        case 'k':
            nt_teleportitis_toggle(ui->data);
            break;
        case 'l':
            nt_warning_toggle(ui->data);
            break;
        default:
            break;
    }
    return EXIT_SUCCESS;
}






