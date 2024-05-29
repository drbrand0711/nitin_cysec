#include "ectf_params.h"

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include <string.h>

// Includes from containerized build
#include "global_secrets.h"

// Custom Packages
#include "boardlink_ap.h"
#include "helper_functions.h"
#include "secure_ap.h"
#include "secure_buffer.h"

// Crypto Library
#include "crypto_wolfssl.h"
#include "wolfssl/wolfcrypt/hash.h"


#define SUCCESS_RETURN 0
#define ERROR_RETURN -1

#if COMPONENT_CNT > 32
#error Too many components!!
#endif

#define TIVA

INIT_BUF_U8(verified, COMPONENT_CNT);
crypto_config crypto[COMPONENT_CNT];

// Buffers for board link communication
INIT_BUF_U8(receive_buffer, MAX_I2C_MESSAGE_LEN);
INIT_BUF_U8(transmit_buffer, MAX_I2C_MESSAGE_LEN);

uint8_t key_buf[] = {MASTER_KEY};
buf_u8 master_key = {.data = key_buf, .size = KEY_LEN_BYTES * MASTER_KEY_COUNT};

// POST BOOT FUNCTIONS
int secure_send(uint8_t *buffer, uint8_t len) {
    //TODO IDK WHAT TO DO HERE
    cpyin_raw_bu8(transmit_buffer, buffer, len);
    crypto_config *dest_config = NULL;

    dest_config = &crypto[0];

    if (dest_config == NULL)
        panic();

    int result = ap_send(slice_bu8(transmit_buffer, 0, len), dest_config);
    if (result != SUCCESS_RETURN)
        panic();
    return result;
}

int secure_receive(uint8_t *buffer) {
    //TODO IDK WHAT TO DO HERE
    crypto_config *dest_config = NULL;

    dest_config = &crypto[0];

    if (!dest_config)
        panic();
    int result = ap_receive(receive_buffer, dest_config);
    if (result <= 0)
        panic();
    cpyout_raw_bu8(buffer, receive_buffer, result);
    return result;
}

// Intialising device
// Need to put the keygen stuff in this function specifically when using flash
void init() {
    // __enable_irq();
    // ADD_HERE_NOP
    init_rng();

    //TODO idk
    set_u8(verified, 0, 0);
    // ADD_HERE_NOP

    // Initialize board link
    board_link_init();
}

void infinite_try_auth(){
    while(!access_u8(verified, 0)){
        if (ap_auth(0, master_key, &crypto[0]) !=
            SUCCESS_RETURN) {
            panic();
            continue;
        }
        set_u8(verified, 0, 1);
        break;
    } 
}


void setup() {
    //Setup
    init();
    infinite_try_auth();
}

void loop(){
    
    //Loop
    //write whatever the fuck u wanna send with secure_send() mamaey
    uint8_t magicsend[3] = {69, 32, 69};
    printf("Secure Send:\n");
    secure_send(magicsend, 3);


}
