#include "can_node.h"
#include "logger.h"
#include <stdlib.h>
#include <string.h>

static void default_on_receive(CANNode* node, const CANFrame* frame) {
    if (!node || !frame) return;

    export_frame_json(&frame, node->node_id);

    logger_log(LOG_INFO, "Node %u received frame ID=0x%03X dlc=%u", 
                          node->node_id, frame->id, frame->dlc);
    node->frames_received++;
}

void node_init(CANNode* node, uint32_t node_id, CANBus* bus) {
    if (!node || !bus) return;
    memset(node, 0, sizeof(CANNode));
    node->node_id = node_id;
    node->bus = bus;
    node->frames_sent = 0;
    node->frames_received = 0;

    // Default callback: just log received frames
    node->on_receive = default_on_receive;
}

bool node_send(CANNode* node, const CANFrame* frame) {
    if (!node || !frame || !node->bus) return false;
    if (!can_frame_validate(frame)) return false;

    bool result = can_bus_send(node->bus, frame);
    if (result) node->frames_sent++;
    return result;
}

