#include <stdint.h>
#include <string.h>
#include <stdio.h>

#include "boardlink_component.h"
#include "ectf_params.h"

int board_link_init(uint32_t component_id) {
    //TODO Add lora init code here
}

int send_packet_and_ack(buf_u8 packet) {
    uint32_t len = packet.size;
    if (len + 1 > MAX_I2C_MESSAGE_LEN)
        panic();
    //TODO Add insecure send here
    printf("%s","PDS:\n");
    printf("%d\n", packet.size);
    for(int i=0; i<packet.size; i++){
        printf("%d\n", packet.data[i]);
    }
    printf("%s", "Sent\n");
    return SUCCESS_RETURN;

}

int wait_and_receive_packet_with_poll_limit(buf_u8 packet) {
    return wait_and_receive_packet(packet);
    //TODO insecure receive

    //return (int)len;
    //else
    // return ERROR_RETURN;
}

int wait_and_receive_packet(buf_u8 packet) {
    //TODO insecure receive
    printf("%s","PDS receive:\n"); 
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
