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
    init_pair(COLOR_RESISTANCES, COLOR_RED, COLOR_BLACK);
    init_pair(COLOR_ABILITIES, COLOR_BLUE, COLOR_BLACK);
    init_pair(COLOR_MAIN_TITLE, COLOR_GREEN, COLOR_BLACK);

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
    ui->resistances_height = (ui->main_height - 8) / 2 - 2;
    ui->resistances_width = ui->main_width / 6;
    ui->resistances_y = (ui->main_height - ui->resistances_height) / 2 - 8 - 1;
    ui->resistances_x = 4;
    ui->W_resistances = derwin(
        ui->W_main,
        ui->resistances_height, ui->resistances_width,
        ui->resistances_y, ui->resistances_x
    );

    // Abilities Window Initialization
    ui->abilities_height = (ui->main_height - 8) / 2 - 2;
    ui->abilities_width = ui->main_width / 6;
    ui->abilities_y = (ui->main_height - ui->abilities_height) / 2 - 8 + ui->abilities_height + 3;
    ui->abilities_x = 4;
    ui->W_abilities = derwin(
        ui->W_main,
        ui->abilities_height, ui->abilities_width,
        ui->abilities_y, ui->abilities_x
    );

    // Other Window Initialization
    ui->other_height = 4;
    ui->other_width = ui->abilities_width;
    ui->other_y = ui->resistances_y + ui->resistances_height;
    ui->other_x = ui->resistances_x;
    ui->W_other = derwin(
        ui->W_main,
        ui->other_height, ui->other_width,
        ui->other_y, ui->other_x
    );

    // ToDo Window Initialization
    ui->todo_height = LINES - 8;
    ui->todo_width = ui->resistances_width * 5 - 5;
    ui->todo_y = ui->resistances_y;
    ui->todo_x = ui->resistances_width + 5;
    ui->W_todo = derwin(
        ui->W_main,
        ui->todo_height, ui->todo_width,
        ui->todo_y, ui->todo_x
    );

    // ToDo Form Initialization
    ui->field_todo[0] = new_field(1, 32, 0, 0, 0, 0);
    ui->field_todo[1] = NULL;

    set_field_back(ui->field_todo[0], A_UNDERLINE);

    ui->form_todo = new_form(ui->field_todo);

    scale_form(ui->form_todo, &ui->form_todo_height, &ui->form_todo_width);
    ui->W_form_todo_height = ui->form_todo_height + 4;
    ui->W_form_todo_width = ui->form_todo_width + 4;
    ui->W_form_todo_y = (LINES - ui->W_form_todo_height) / 2;
    ui->W_form_todo_x = (COLS - ui->W_form_todo_width) / 2;

    ui->W_form_todo = newwin(
        ui->W_form_todo_height, ui->W_form_todo_width,
        ui->W_form_todo_y, ui->W_form_todo_x
    );

    set_form_win(ui->form_todo, ui->W_form_todo);
    set_form_sub(ui->form_todo, derwin(ui->W_form_todo, ui->form_todo_height, ui->form_todo_width, 2, 2));


    // Panel Creation
    ui->P_help = new_panel(ui->W_help);
    ui->P_main = new_panel(ui->W_main);
    ui->P_form_todo = new_panel(ui->W_form_todo);

    // Initially hiding the form panel
    hide_panel(ui->P_form_todo);

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
nt_ui_draw_title(WINDOW *win, uint32_t win_width, char *text, size_t len,
    int color)
{
    uint16_t title_x = (win_width - len) / 2;
    uint16_t title_y = 0;

    wattron(win, COLOR_PAIR(color));
    mvwprintw(win, title_y, title_x, "%s", text);
    wattroff(win, COLOR_PAIR(color));

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
nt_ui_draw_other(WINDOW *win, uint32_t y, uint32_t x, char *text, uint32_t has,
    char letter)
{
    char buffer[256];
    snprintf(buffer, 256, "%c - %s %d      ", letter, text, has);

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

    wattron(ui->W_main, COLOR_PAIR(COLOR_MAIN_TITLE));
    mvwprintw(ui->W_main, 1, (ui->main_width - 24) / 2,
        "NetHack NoteTaker v1.0.2");
    wattroff(ui->W_main, COLOR_PAIR(COLOR_MAIN_TITLE));

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
            (ui->main_width - 124) / 2,
            "x - Remove ToDo Entry | t - Add ToDo Entry | c - Toggle ToDo Entry | r - toggle resistance  | a - toggle ability |  q - quit");
    } else {
        mvwprintw(ui->W_main, ui->main_height - 2,
            (ui->main_width - len) / 2,
            "%s", text);
    }

    return EXIT_SUCCESS;
}

uint8_t
nt_ui_draw_todo(WINDOW *win, uint32_t y, uint32_t x, NT_TODO *td)
{
    if (td->is_complete) {
        mvwprintw(
            win, y, x,
            "[x] %c - %s", td->letter, td->text
        );
    } else {
        mvwprintw(
            win, y, x,
            "[ ] %c - %s", td->letter, td->text
        );
    }


    return EXIT_SUCCESS;
}

uint8_t
nt_ui_data_draw(NT_UI *ui)
{
    uint8_t list_y = 1;
    uint8_t list_x = 2;


    nt_ui_draw_main_title(ui);
    nt_ui_draw_main_hints(ui, NULL, 0);

    // Drawing Resistances State
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

    // Drawing Other State
    nt_ui_draw_other(ui->W_other, list_y, list_x, "Protection:",
        ui->data->divine_protection,
        'd');
    nt_ui_draw_other(ui->W_other, list_y + 1, list_x, "Last Prayed On:",
        ui->data->last_turn_prayed,
        'p');

    // Drawing todo list items
    NT_TODO* cur = ui->data->todo_head;
    int y_offset = 1;
    while (cur != NULL) {
        nt_ui_draw_todo(ui->W_todo, list_y + y_offset, list_x + ui->other_x - 4, cur);
        y_offset++;
        cur = cur->next;
    }

    // Draw boxes first so nothing overlaps them
    box(ui->W_main, ACS_VLINE, ACS_HLINE);
    wattron(ui->W_resistances, COLOR_PAIR(COLOR_RESISTANCES));
    box(ui->W_resistances, ACS_VLINE, ACS_HLINE);
    wattroff(ui->W_resistances, COLOR_PAIR(COLOR_RESISTANCES));
    wattron(ui->W_abilities, COLOR_PAIR(COLOR_ABILITIES));
    box(ui->W_abilities, ACS_VLINE, ACS_HLINE);
    wattroff(ui->W_abilities, COLOR_PAIR(COLOR_ABILITIES));
    box(ui->W_other, ACS_VLINE, ACS_HLINE);
    box(ui->W_todo, ACS_VLINE, ACS_HLINE);
    box(ui->W_form_todo, ACS_VLINE, ACS_HLINE);

    // Draw Titles
    nt_ui_draw_title(
        ui->W_todo, ui->todo_width,
        " Things to be Done if not Already ", 34, 0);
    nt_ui_draw_title(
        ui->W_other, ui->other_width,
        " Misc. ", 7, 0);
    nt_ui_draw_title(
        ui->W_abilities, ui->abilities_width,
        " Abilities ", 11, COLOR_ABILITIES);
    nt_ui_draw_title(
        ui->W_resistances, ui->resistances_width,
        " Resistances ", 13, COLOR_RESISTANCES);
    nt_ui_draw_title(
        ui->W_form_todo, ui->form_todo_width,
        " Add ToDo: ", 12, 0);

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

        // Detecting entry of numbers for last turn prayed/divine protection
        char *numbers = calloc(16, sizeof *numbers);
        int count = 0;
        while (input >= '0' && input<= '9') {
            numbers[count] = input;
            count++;
            input = getch();
        }
        numbers[count] = 0x0;

        switch (input) {
            case 'x':
                nt_ui_draw_main_hints(ui,
                    "Removing ToDo Entry: Choose an entry",
                    36);
                update_panels();
                doupdate();
                nt_todo_remove(ui->data, getch());
                break;
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
            case 'p':
                {
                    if (count > 0) {
                        nt_last_turn_prayed_set(ui->data, atoi(numbers));
                    }
                }
                break;
            case 'd':
                {
                    if (count > 0) {
                        nt_divine_protection_set(ui->data, atoi(numbers));
                    }
                }
                break;
            case 'c':
                nt_ui_draw_main_hints(ui,
                    "Toggling ToDo Entry: Choose an entry",
                    36);
                update_panels();
                doupdate();
                nt_todo_toggle_complete(ui->data->todo_head, getch());
                break;
            case 't':
                // Don't know what's going on here... TODO for the rewrite.
                post_form(ui->form_todo);
                show_panel(ui->P_form_todo);
                update_panels();
                doupdate();
                nt_ui_add_todo(ui);
                hide_panel(ui->P_form_todo);
                update_panels();
                doupdate();
                break;
            default:
                break;
        }
        werase(ui->W_todo);
        nt_ui_data_draw(ui);
        free(numbers);
    }
    return EXIT_SUCCESS;
}

uint8_t
nt_ui_add_todo(NT_UI *ui)
{
    int ch;
    int quit = 0;
    char buffer[256];
    int i = 0;
    int getting_input = 1;

    form_driver(ui->form_todo, REQ_CLR_FIELD);
    update_panels();
    doupdate();

    while (getting_input) {
        ch = getch();
        switch (ch) {
            case '\n':
                getting_input = 0;
                break;
            case 'q':
                quit = 1;
                getting_input = 0;
                break;
            case 263:
                i--;
                buffer[i] = '\0';
                form_driver(ui->form_todo, REQ_DEL_PREV);
                break;
            default:
                form_driver(ui->form_todo, ch);
                buffer[i] = ch;
                i++;
                break;
        }

        update_panels();
        doupdate();
    }

    while (i < 256) {
        buffer[i] = ' ';
        i++;
    }
    buffer[i] = 0x0;
    if (!quit) {
        nt_todo_add(ui->data, buffer, 0);
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






