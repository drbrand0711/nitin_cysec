#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "boardlink_ap.h"
#include "ectf_params.h"

void board_link_init(void) {
    //TODO lora init 
}


int send_packet_and_ack(uint32_t component_id, buf_u8 packet) {
    //TODO insecure send
    uint32_t len = packet.size;
    if (len + 1 > MAX_I2C_MESSAGE_LEN)
        panic();
    //TODO Add insecure send here
    printf("%s","Remote:\n");
    printf("%d\n", packet.size);
    for(int i=0; i<packet.size; i++){
        printf("%d\n", packet.data[i]);
    }
    printf("%s", "Sent\n");
    return SUCCESS_RETURN;
    //else
    // return ERROR_RETURN;
}

int poll_and_receive_packet(uint32_t component_id, buf_u8 packet) {
    //TODO insecure receive
    printf("%s","Remote receive:\n"); 
    int len;
    scanf("%d", &len);
    INIT_BUF_U8(temp, len);
    // assert_min_size_bu8(packet, len);
    uint8_t var;
    for(int i=0; i<len; i++){
        scanf("%d", &var);
        temp.data[i] = var;
    }
    cpy_bu8(packet, temp, len);
    printf("%s", "Received\n");
    return (int)len;
    //else
    // return ERROR_RETURN;
}
