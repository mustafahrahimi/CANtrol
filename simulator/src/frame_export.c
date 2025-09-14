#include "frame_export.h"
#include <stdio.h>
#include "can_frame.h"

void export_frame_json(const CANFrame* frame, uint32_t source_node) {
    if (!frame) return;

    printf(
        "{\"id\":%u,\"dlc\":%u,\"data\":[%u,%u,%u,%u,%u,%u,%u,%u],\"node\":%u}\n",
        frame->id,
        frame->dlc,
        frame->data[0], frame->data[1], frame->data[2], frame->data[3],
        frame->data[4], frame->data[5], frame->data[6], frame->data[7],
        source_node
    );

    fflush(stdout);
}
