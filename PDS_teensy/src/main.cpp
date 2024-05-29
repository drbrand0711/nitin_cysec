//TODO
#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>

// deployment
#include <ectf_params.h>
#include <global_secrets.h>

// custom packages
#include <crypto_wolfssl.h>
#include <helper_functions.h>
#include <secure_buffer.h>

// component libraries
#include <boardlink_component.h>
#include <secure_component.h>

#include <stdint.h>

#define TEENSY

void init() {
    // __enable_irq();
    init_rng();
    
    board_link_init(0);
}

// ==== COMMUNICATION ====
/**
 * Key generated after authenticating with AP.
 */
crypto_config key;

INIT_BUF_U8(receive_buffer, MAX_I2C_MESSAGE_LEN);
INIT_BUF_U8(transmit_buffer, MAX_I2C_MESSAGE_LEN);

uint8_t key_buf[] = {MASTER_KEY};
buf_u8 master_key = {.data = key_buf, .size = MASTER_KEY_COUNT * KEY_LEN_BYTES};

// Secure Communication
// These functions will be externed from the secure library

void secure_send(uint8_t *data, uint8_t len) {
    cpyin_raw_bu8(transmit_buffer, data, len);
    if (component_send(slice_bu8(transmit_buffer, 0, len), &key) !=
        SUCCESS_RETURN)
        panic();
}

int secure_receive(uint8_t *buffer) {
    int len = wait_and_receive_packet(receive_buffer);
    if (len <= 0)
        panic();
    int ret = component_receive(slice_bu8(receive_buffer, 0, len), &key);
    if (ret <= 0)
        panic();
    cpyout_raw_bu8(buffer, receive_buffer, ret);
    return ret;
}

void clear_buffer(char *buffer, uint32_t size) {
    memset(buffer, 0, size);
}


bool check_auth() {
    INIT_BUF_U8(auth_buf, 4);
    set_u8(auth_buf, 0, 'A');
    set_u8(auth_buf, 1, 'U');
    set_u8(auth_buf, 2, 'T');
    set_u8(auth_buf, 3, 'H');
    return cmp_bu8(slice_bu8(receive_buffer, 0, 4), auth_buf);
}


int auth = ERROR_RETURN;

void infinite_try_auth(){
    while (1) {
        int len = wait_and_receive_packet(receive_buffer);
        if (auth == ERROR_RETURN && check_auth() == true) {
            auth =
                component_auth(0, slice_bu8(receive_buffer, 0, len),
                               master_key, &key);
            if(auth == SUCCESS_RETURN)
                break;
        }
    }
}


void setup() {
    //setup
    init();
    infinite_try_auth();
}
void loop(){
    //loop
    if (auth != SUCCESS_RETURN)
        infinite_try_auth();
    uint8_t data[1000];
    int len = secure_receive(data);
    printf("Secure receive:\n");
    for(int i =0; i<len; i++){
        printf("%d\n", data[i]);
    }
}
