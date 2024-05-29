#include "helper_functions.h"
#include "secure_buffer.h"
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>

void init_rng() {
    //TODO later if needed
}

uint32_t rand_uint() {
    return (uint32_t)69;

}
//TODO replace above two



void panic() {
    //TODO panic later
    printf("%s", "Panic\n");
    //Call reset
}
