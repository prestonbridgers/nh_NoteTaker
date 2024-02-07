/* [filename.c]
 * author: Curt Bridgers
 * email: prestonbridgers@gmail.com
*/

#include <stdio.h>
#include <stdlib.h>

#include "nhnt.h"

NT_DATA*
nt_create(char *plr_name)
{
    NT_DATA *d = calloc(1, sizeof *d);
    d->plr_name = calloc(64, sizeof *d->plr_name);
    strncpy(d->plr_name, plr_name, 64);
    d->has_shock_resistance = 0;
    d->has_fire_resistance = 0;
    d->has_cold_resistance = 0;
    d->has_poison_resistance = 0;
    d->has_sleep_resistance = 0;
    d->has_disintegration_resistance = 0;
    d->has_magic_resistance = 0;
    d->has_infravision = 0;
    d->has_invisibility = 0;
    d->has_reflection = 0;
    d->has_searching = 0;
    d->has_see_invisible = 0;
    d->has_speed1 = 0;
    d->has_speed2 = 0;
    d->has_stealth = 0;
    d->has_telepathy = 0;
    d->has_teleport_control = 0;
    d->has_teleportitis = 0;
    d->has_warning = 0;
    d->divine_protection = 0;
    d->last_turn_prayed = 0;
    d->todo_head = NULL;
    return d;
}

uint8_t
nt_destroy(NT_DATA *data)
{
    if (data == NULL) {
       return EXIT_FAILURE;
    }
    free(data);
    return EXIT_SUCCESS;
}

NT_TODO*
nt_todo_create(char *text, int is_complete, NT_TODO *next)
{
    NT_TODO *td = calloc(1, sizeof *td);
    td->max_len = 1024;
    td->text = calloc(td->max_len, sizeof *td->text);
    strncpy(td->text, text, td->max_len);
    td->is_complete = is_complete;
    td->next = next;
    return td;
}

uint8_t
nt_todo_remove(NT_DATA *data, char letter)
{
    NT_TODO *cur = data->todo_head;
    NT_TODO *prev = cur;

    // Handling case where target is the head
    if (cur->letter == letter) {
        data->todo_head = data->todo_head->next;
        free(cur);
        return EXIT_SUCCESS;
    }

    // All other cases
    while (cur->letter != letter) {
        prev = cur;
        cur = cur->next;
    }

    prev->next = cur->next;
    free(cur);
    nt_todo_letter_refresh(data->todo_head);
    return EXIT_SUCCESS;
}

uint8_t
nt_todo_add(NT_DATA *data, char *text, uint8_t is_complete)
{
    NT_TODO *new_head = nt_todo_create(text, is_complete, data->todo_head);
    data->todo_head = new_head;
    nt_todo_letter_refresh(data->todo_head);
    return EXIT_SUCCESS;
}

uint8_t
nt_todo_letter_refresh(NT_TODO *head)
{
    char letters[] = "abcdefghijklmnopqrstuvwxyz";
    int count = 0;
    NT_TODO *cur = head;

    while (cur != NULL) {
        cur->letter = letters[count];
        count++;
        cur = cur->next;
    }
    return EXIT_SUCCESS;
}

uint8_t
nt_todo_print(NT_DATA *d)
{
    NT_TODO *cur = d->todo_head;
    while (cur != NULL) {
        if (cur->is_complete) {
            fprintf(stderr, "[x] %c - %s\n", cur->letter, cur->text);
        } else {
            fprintf(stderr, "[ ] %c - %s\n", cur->letter, cur->text);
        }
        cur = cur->next;
    }
    return EXIT_SUCCESS;
}

uint8_t
nt_todo_destroy(NT_TODO *todo)
{
    free(todo->text);
    free(todo);
    return EXIT_SUCCESS;
}

uint8_t
nt_todo_toggle_complete(NT_TODO *head, char letter)
{
    NT_TODO *cur = head;
    while (cur->letter != letter) {
        cur = cur->next;
        if (cur == NULL) {
            return EXIT_FAILURE;
        }
    }
    cur->is_complete = !cur->is_complete;
    return EXIT_SUCCESS;
}

uint8_t
nt_todo_text_set(NT_TODO *todo, char *text)
{
    free(todo->text);
    todo->text = calloc(todo->max_len, sizeof *todo->text);
    strncpy(todo->text, text, todo->max_len);
    return EXIT_SUCCESS;
}

uint8_t
nt_data_serialize(NT_DATA *data)
{
    FILE *f;
    char filename[256];

    snprintf(filename, 256, "%s.txt", data->plr_name);
    f = fopen(filename, "w");

    fprintf(f, "%s|%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d|%d|%d\n",
        data->plr_name, //19
        
        data->has_shock_resistance,
        data->has_fire_resistance,
        data->has_cold_resistance,
        data->has_poison_resistance,
        data->has_sleep_resistance,
        data->has_disintegration_resistance,
        data->has_magic_resistance,

        data->has_infravision,
        data->has_invisibility,
        data->has_reflection,
        data->has_searching,
        data->has_see_invisible,
        data->has_speed1,
        data->has_speed2,
        data->has_stealth,
        data->has_telepathy,
        data->has_teleport_control,
        data->has_teleportitis,
        data->has_warning,

        data->divine_protection,
        data->last_turn_prayed);

    fclose(f);
    return EXIT_SUCCESS;
}

NT_DATA*
nt_data_load(NT_DATA *data, char *filename)
{
    FILE *f;
    char data_str[1024];
    char *token;
    uint32_t i;

    f = fopen(filename, "r");

    fgets(data_str, 1024, f);

    // Name
    token = strtok(data_str, "|");
    data->plr_name = calloc(64, sizeof *data->plr_name);
    strncpy(data->plr_name, token, 64);

    // Resistances & Abilities
    token = strtok(NULL, "|");
    i = 0;
    if (token[i] == '1') { data->has_shock_resistance = 1; } i++;
    if (token[i] == '1') { data->has_fire_resistance = 1; } i++;
    if (token[i] == '1') { data->has_cold_resistance = 1; } i++;
    if (token[i] == '1') { data->has_poison_resistance = 1; } i++;
    if (token[i] == '1') { data->has_sleep_resistance = 1; } i++;
    if (token[i] == '1') { data->has_disintegration_resistance = 1; } i++;
    if (token[i] == '1') { data->has_magic_resistance = 1; } i++;
    if (token[i] == '1') { data->has_infravision = 1; } i++;
    if (token[i] == '1') { data->has_invisibility = 1; } i++;
    if (token[i] == '1') { data->has_reflection = 1; } i++;
    if (token[i] == '1') { data->has_searching = 1; } i++;
    if (token[i] == '1') { data->has_see_invisible = 1; } i++;
    if (token[i] == '1') { data->has_speed1 = 1; } i++;
    if (token[i] == '1') { data->has_speed2 = 1; } i++;
    if (token[i] == '1') { data->has_stealth = 1; } i++;
    if (token[i] == '1') { data->has_telepathy = 1; } i++;
    if (token[i] == '1') { data->has_teleport_control = 1; } i++;
    if (token[i] == '1') { data->has_teleportitis = 1; } i++;
    if (token[i] == '1') { data->has_warning = 1; } i++;

    // Divine protection
    token = strtok(NULL, "|");
    data->divine_protection = atoi(token);

    // Last turn prayed
    token = strtok(NULL, "|");
    data->last_turn_prayed = atoi(token);

    fclose(f);
    return NULL;
}

uint8_t
nt_shock_toggle(NT_DATA *data)
{
    if (data == NULL) {
        return EXIT_FAILURE;
    }
    data->has_shock_resistance = !data->has_shock_resistance;
    return EXIT_SUCCESS;
}

uint8_t 
nt_fire_toggle(NT_DATA *data)
{
    if (data == NULL) {
        return EXIT_FAILURE;
    }
    data->has_fire_resistance = !data->has_fire_resistance;
    return EXIT_SUCCESS;
}

uint8_t 
nt_cold_toggle(NT_DATA *data)
{
    if (data == NULL) {
        return EXIT_FAILURE;
    }
    data->has_cold_resistance = !data->has_cold_resistance;
    return EXIT_SUCCESS;
}

uint8_t
nt_poison_toggle(NT_DATA *data)
{
    if (data == NULL) {
        return EXIT_FAILURE;
    }
    data->has_poison_resistance = !data->has_poison_resistance;
    return EXIT_SUCCESS;
}

uint8_t
nt_sleep_toggle(NT_DATA *data)
{
    if (data == NULL) {
        return EXIT_FAILURE;
    }
    data->has_sleep_resistance = !data->has_sleep_resistance;
    return EXIT_SUCCESS;
}

uint8_t
nt_disintegration_toggle(NT_DATA *data)
{
    if (data == NULL) {
        return EXIT_FAILURE;
    }
    data->has_disintegration_resistance = !data->has_disintegration_resistance;
    return EXIT_SUCCESS;
}

uint8_t
nt_magic_toggle(NT_DATA *data)
{
    if (data == NULL) {
        return EXIT_FAILURE;
    }
    data->has_magic_resistance = !data->has_magic_resistance;
    return EXIT_SUCCESS;
}

uint8_t
nt_infravision_toggle(NT_DATA *data)
{
    if (data == NULL) {
        return EXIT_FAILURE;
    }
    data->has_infravision = !data->has_infravision;
    return EXIT_SUCCESS;
}

uint8_t
nt_invisibility_toggle(NT_DATA *data)
{
    if (data == NULL) {
        return EXIT_FAILURE;
    }
    data->has_invisibility = !data->has_invisibility;
    return EXIT_SUCCESS;
}

uint8_t
nt_reflection_toggle(NT_DATA *data)
{
    if (data == NULL) {
        return EXIT_FAILURE;
    }
    data->has_reflection = !data->has_reflection;
    return EXIT_SUCCESS;
}

uint8_t
nt_searching_toggle(NT_DATA *data)
{
    if (data == NULL) {
        return EXIT_FAILURE;
    }
    data->has_searching = !data->has_searching;
    return EXIT_SUCCESS;
}

uint8_t
nt_see_invisible_toggle(NT_DATA *data)
{
    if (data == NULL) {
        return EXIT_FAILURE;
    }
    data->has_see_invisible = !data->has_see_invisible;
    return EXIT_SUCCESS;
}

uint8_t
nt_speed1_toggle(NT_DATA *data)
{
    if (data == NULL) {
        return EXIT_FAILURE;
    }
    // Can't have speed2 without speed1
    if (data->has_speed2 && data->has_speed1) {
        nt_speed2_toggle(data);
    }
    data->has_speed1 = !data->has_speed1;
    return EXIT_SUCCESS;
}

uint8_t
nt_speed2_toggle(NT_DATA *data)
{
    // Can't have speed2 without speed1
    if (data == NULL || !data->has_speed1) {
        return EXIT_FAILURE;
    }
    data->has_speed2 = !data->has_speed2;
    return EXIT_SUCCESS;
}

uint8_t
nt_stealth_toggle(NT_DATA *data)
{
    if (data == NULL) {
        return EXIT_FAILURE;
    }
    data->has_stealth = !data->has_stealth;
    return EXIT_SUCCESS;
}

uint8_t
nt_telepathy_toggle(NT_DATA *data)
{
    if (data == NULL) {
        return EXIT_FAILURE;
    }
    data->has_telepathy = !data->has_telepathy;
    return EXIT_SUCCESS;
}

uint8_t
nt_teleport_control_toggle(NT_DATA *data)
{
    if (data == NULL) {
        return EXIT_FAILURE;
    }
    data->has_teleport_control = !data->has_teleport_control;
    return EXIT_SUCCESS;
}

uint8_t
nt_teleportitis_toggle(NT_DATA *data)
{
    if (data == NULL) {
        return EXIT_FAILURE;
    }
    data->has_teleportitis = !data->has_teleportitis;
    return EXIT_SUCCESS;
}

uint8_t
nt_warning_toggle(NT_DATA *data)
{
    if (data == NULL) {
        return EXIT_FAILURE;
    }
    data->has_warning = !data->has_warning;
    return EXIT_SUCCESS;
}

uint8_t
nt_divine_protection_set(NT_DATA *data, uint16_t num)
{
    if (data == NULL) {
        return EXIT_FAILURE;
    }
    data->divine_protection = num;
    return EXIT_SUCCESS;
}

uint8_t
nt_last_turn_prayed_set(NT_DATA *data, uint32_t num)
{
    if (data == NULL) {
        return EXIT_FAILURE;
    }
    data->last_turn_prayed = num;
    return EXIT_SUCCESS;
}

uint8_t
nt_data_print(NT_DATA *data)
{
    printf("------------------\n");
    printf("Name: %s\n", data->plr_name);
    printf("shock: %d\n", data->has_shock_resistance);
    printf("fire: %d\n", data->has_fire_resistance);
    printf("cold: %d\n", data->has_cold_resistance);
    return EXIT_SUCCESS;
}


