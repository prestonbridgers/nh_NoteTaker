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
    d->plr_name = plr_name;
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

uint8_t
nt_data_serialize(NT_DATA *data)
{
    FILE *f;
    char filename[256];

    snprintf(filename, 256, "%s.txt", data->plr_name);
    f = fopen(filename, "w");

    fprintf(f, "%s|%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d%d|%d|%d\n",
        data->plr_name,
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
nt_data_load(NT_DATA *data)
{
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


