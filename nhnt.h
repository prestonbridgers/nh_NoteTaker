#ifndef HEADER_NHNT_INCLUDED
#define HEADER_NHNT_INCLUDED

#include <stdint.h>
#include <ncurses.h>
#include <string.h>
#include <panel.h>

#define COLOR_RESISTANCES 1
#define COLOR_ABILITIES 2
#define COLOR_MAIN_TITLE 3

/*------------*/
/* BEGIN DATA */
/*------------*/

typedef struct NT_TODO_T {
    char *text;
    char letter;
    size_t max_len;
    uint8_t is_complete;
    struct NT_TODO_T *next;
} NT_TODO;

typedef struct {
    char *plr_name;
    // Resistances
    uint8_t has_shock_resistance;
    uint8_t has_fire_resistance;
    uint8_t has_cold_resistance;
    uint8_t has_poison_resistance;
    uint8_t has_sleep_resistance;
    uint8_t has_disintegration_resistance;
    uint8_t has_magic_resistance;
    // Abilities
    uint8_t has_infravision;
    uint8_t has_invisibility;
    uint8_t has_reflection;
    uint8_t has_searching;
    uint8_t has_see_invisible;
    uint8_t has_speed1;
    uint8_t has_speed2;
    uint8_t has_stealth;
    uint8_t has_telepathy;
    uint8_t has_teleport_control;
    uint8_t has_teleportitis;
    uint8_t has_warning;
    // Other
    uint16_t divine_protection;
    uint32_t last_turn_prayed;
    // ToDo List
    NT_TODO *todo_head;
} NT_DATA;

NT_TODO* nt_todo_create(char *text, int is_complete, NT_TODO *next);
uint8_t nt_todo_destroy(NT_TODO *todo);
uint8_t nt_todo_toggle_complete(NT_TODO *head, char letter);
uint8_t nt_todo_text_set(NT_TODO *todo, char *text);
uint8_t nt_todo_print(NT_DATA *d);
uint8_t nt_todo_add(NT_DATA *data, char *text, uint8_t is_complete);
uint8_t nt_todo_letter_refresh(NT_TODO *head);

NT_DATA* nt_create(char *plr_name);
uint8_t nt_destroy(NT_DATA *data);

uint8_t nt_data_serialize(NT_DATA *data);
NT_DATA* nt_data_load(NT_DATA *data, char *filename);

uint8_t nt_shock_toggle(NT_DATA *data);
uint8_t nt_fire_toggle(NT_DATA *data);
uint8_t nt_cold_toggle(NT_DATA *data);
uint8_t nt_poison_toggle(NT_DATA *data);
uint8_t nt_sleep_toggle(NT_DATA *data);
uint8_t nt_disintegration_toggle(NT_DATA *data);
uint8_t nt_magic_toggle(NT_DATA *data);

uint8_t nt_infravision_toggle(NT_DATA *data);
uint8_t nt_invisibility_toggle(NT_DATA *data);
uint8_t nt_reflection_toggle(NT_DATA *data);
uint8_t nt_searching_toggle(NT_DATA *data);
uint8_t nt_see_invisible_toggle(NT_DATA *data);
uint8_t nt_speed1_toggle(NT_DATA *data);
uint8_t nt_speed2_toggle(NT_DATA *data);
uint8_t nt_stealth_toggle(NT_DATA *data);
uint8_t nt_telepathy_toggle(NT_DATA *data);
uint8_t nt_teleport_control_toggle(NT_DATA *data);
uint8_t nt_teleportitis_toggle(NT_DATA *data);
uint8_t nt_warning_toggle(NT_DATA *data);
uint8_t nt_divine_protection_set(NT_DATA *data, uint16_t num);
uint8_t nt_last_turn_prayed_set(NT_DATA *data, uint32_t num);

uint8_t nt_data_print(NT_DATA *data);

/*------------*/
/*  END DATA  */
/*------------*/

/*------------*/
/*  BEGIN UI  */
/*------------*/

typedef struct {
    NT_DATA *data;
    // Main Window
    PANEL *P_main;
    WINDOW *W_main;
    uint32_t main_height;
    uint32_t main_width;
    uint32_t main_y;
    uint32_t main_x;
    // Help Window
    PANEL *P_help;
    WINDOW *W_help;
    uint32_t help_height;
    uint32_t help_width;
    uint32_t help_y;
    uint32_t help_x;
    // Resistances SubWindow
    WINDOW *W_resistances;
    uint32_t resistances_height;
    uint32_t resistances_width;
    uint32_t resistances_y;
    uint32_t resistances_x;
    // Abilities SubWindow
    WINDOW *W_abilities;
    uint32_t abilities_height;
    uint32_t abilities_width;
    uint32_t abilities_y;
    uint32_t abilities_x;
    // Other SubWindow
    WINDOW *W_other;
    uint32_t other_height;
    uint32_t other_width;
    uint32_t other_y;
    uint32_t other_x;
    // ToDo SubWindow
    WINDOW *W_todo;
    uint32_t todo_height;
    uint32_t todo_width;
    uint32_t todo_y;
    uint32_t todo_x;
} NT_UI;

NT_UI* nt_ui_create(NT_DATA *data);
uint8_t nt_ui_destroy(NT_UI *ui);
uint8_t nt_ui_data_draw(NT_UI *ui);
uint8_t nt_ui_draw_list(WINDOW *win, uint32_t y, uint32_t x, char *text,
    uint32_t has, char letter);
uint8_t nt_ui_draw_other(WINDOW *win, uint32_t y, uint32_t x, char *text,
    uint32_t has, char letter);
uint8_t nt_ui_interact_loop(NT_UI *ui);
uint8_t nt_ui_toggle_resistance(NT_UI *ui, char res);
uint8_t nt_ui_draw_title(WINDOW *win, uint32_t win_width, char *text,
    size_t len, int color);
uint8_t nt_ui_draw_main_title(NT_UI *ui);
uint8_t nt_ui_draw_main_hints(NT_UI *ui, char *text, size_t len);
uint8_t nt_ui_toggle_ability(NT_UI *ui, char abil);

uint8_t init_curses(void);
uint8_t cleanup_curses(void);

/*------------*/
/*   END UI   */
/*------------*/



#endif /* HEADER_NHNT_INCLUDED */
