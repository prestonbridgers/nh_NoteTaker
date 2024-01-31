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
    d->has_shock = 0;
    d->has_fire = 0;
    d->has_cold = 0;
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
nt_shock_toggle(NT_DATA *data)
{
    if (data == NULL) {
        return EXIT_FAILURE;
    }
    data->has_shock = !data->has_shock;
    return EXIT_SUCCESS;
}

uint8_t 
nt_fire_toggle(NT_DATA *data)
{
    if (data == NULL) {
        return EXIT_FAILURE;
    }
    data->has_fire = !data->has_fire;
    return EXIT_SUCCESS;
}

uint8_t 
nt_cold_toggle(NT_DATA *data)
{
    if (data == NULL) {
        return EXIT_FAILURE;
    }
    data->has_cold = !data->has_cold;
    return EXIT_SUCCESS;
}

uint8_t
nt_data_print(NT_DATA *data)
{
    printf("------------------\n");
    printf("Name: %s\n", data->plr_name);
    printf("has_shock: %d\n", data->has_shock);
    printf("has_fire: %d\n", data->has_fire);
    printf("has_cold: %d\n", data->has_cold);
    return EXIT_SUCCESS;
}


