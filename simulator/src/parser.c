#include "parser.h"
#include <string.h>
#include <arpa/inet.h>

size_t can_encoder_frame(const CANFrame* frame, uint8_t* buffer, size_t buffer_size) {
    if (!frame || !buffer || buffer_size < CAN_RAW_FRAME_SIZE) return 0;

    // ID in network byte order (big endian)
    uint32_t id_net = htonl(frame->id); 
    memcpy(buffer, &id_net, 4);
    buffer[4] = frame->dlc;

    // Always write 8 bytes of data (padded if dlc < 8)
    memset(buffer + 5, 0, 8);
    memcpy(buffer + 5, frame->data, frame->dlc);

    return CAN_RAW_FRAME_SIZE;
}

bool can_parse_raw_frame(const uint8_t* buffer, size_t buffer_size, CANFrame* frame) {
    if (!frame || !buffer || buffer_size < CAN_RAW_FRAME_SIZE) return false;

    uint32_t id_net = 0;
    memcpy(&id_net, buffer, 4);
    frame->id = ntohl(id_net);

    frame->dlc = buffer[4];
    
    memcpy(frame->data, buffer + 5, 8);
    frame->rtr = false; 

    return can_frame_validate(frame);
}