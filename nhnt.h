#ifndef HEADER_NHNT_INCLUDED
#define HEADER_NHNT_INCLUDED

#include <stdint.h>
#include <ncurses.h>

typedef struct {
    char *plr_name;
    uint8_t has_shock;
    uint8_t has_fire;
    uint8_t has_cold;
} NT_DATA;

NT_DATA* nt_create(char *plr_name);
uint8_t  nt_destroy(NT_DATA *data);

uint8_t nt_shock_toggle(NT_DATA *data);
uint8_t nt_fire_toggle(NT_DATA *data);
uint8_t nt_cold_toggle(NT_DATA *data);

uint8_t nt_data_print(NT_DATA *data);

#endif /* HEADER_NHNT_INCLUDED */
