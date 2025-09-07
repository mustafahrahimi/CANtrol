#include "parser.h"
#include <string.h>
#include <arpa/inet.h>

size_t encode_can_frame(const CANFrame* frame, uint8_t* buffer, size_t buffer_size) {
    if (!frame || !buffer || buffer_size < PARSER_BUFFER_SIZE) return 0;

    uint32_t id_net = htonl(frame->id); // convert to network byte order
    memcpy(buffer, &id_net, 4);
    buffer[4] = frame->dlc;
    memcpy(buffer + 5, frame->data, 8);
    
    return PARSER_BUFFER_SIZE;
}

bool decode_can_frame(const uint8_t* buffer, size_t buffer_size, CANFrame* frame) {
    if (!frame || !buffer || buffer_size < PARSER_BUFFER_SIZE) return false;

    uint32_t id_net = 0;
    memcpy(&id_net, buffer, 4);
    frame->id = ntohl(id_net);
    frame->dlc = buffer[4];
    memcpy(frame->data, buffer + 5, 8);
    frame->rtr = false; // currently not encoded

    return can_frame_validate(frame);
}