#ifndef HEADER_NHNT_INCLUDED
#define HEADER_NHNT_INCLUDED

#include <stdint.h>
#include <ncurses.h>

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
} NT_DATA;

NT_DATA* nt_create(char *plr_name);
uint8_t  nt_destroy(NT_DATA *data);

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

#endif /* HEADER_NHNT_INCLUDED */
