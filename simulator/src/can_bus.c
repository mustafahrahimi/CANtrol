#include "logger.h"
#include "can_bus.h"
#include "can_node.h"
#include <string.h>

void can_bus_init(CANBus* bus) {
    memset(bus, 0, sizeof(CANBus));
    logger_log(LOG_INFO, "CAN bus initialized");
}

bool can_bus_add_node(CANBus* bus, CANNode* node) {
    if (bus->node_count >= CAN_BUS_MAX_NODES) return false;
    bus->nodes[bus->node_count++] = node;
    logger_log(LOG_INFO, "Node added to CAN bus (total: %u)", bus->node_count);
    return true;
}

bool can_bus_send(CANBus* bus, const CANFrame* frame) {
    uint32_t next_tail = (bus->q_tail + 1) % CAN_BUS_QUEUE_SIZE;
    if (next_tail == bus->q_head) {
        logger_log(LOG_WARN, "CAN bus queue full, dropping frame");
        return false;
    }

    bus->queue[bus->q_tail] = *frame;
    bus->q_tail = next_tail;
    logger_log(LOG_DEBUG, "Frame queued: ID=0x%03X", frame->id);
    return true;
}

void can_bus_process(CANBus* bus) {
    while (bus->q_head != bus->q_tail) {
        CANFrame frame = bus->queue[bus->q_head];
        bus->q_head = (bus->q_head + 1) % CAN_BUS_QUEUE_SIZE;

        for (uint32_t i = 0; i < bus->node_count; i++) {
            CANNode* node = bus->nodes[i];
            if (node && node->on_receive) {
                node->on_receive(node, &frame);
            }
        }
    }
}