#ifndef __BOARDLINK_AP__
#define __BOARDLINK_AP__

#include "secure_buffer.h"

#define SUCCESS_RETURN 0
#define ERROR_RETURN -1


/******************************** FUNCTION PROTOTYPES ********************************/

void board_link_init(void);
int check_component_liveness(uint32_t component_id);
int send_packet_and_ack(uint32_t component_id, buf_u8 packet);
int poll_and_receive_packet(uint32_t component_id, buf_u8 packet);

#endif
