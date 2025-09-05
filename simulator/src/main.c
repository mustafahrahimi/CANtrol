#include "logger.h"
#include "can_frame.h"
#include "can_bus.h"
#include "can_node.h"

static void custom_receive(CANNode* node, const CANFrame* frame) {
    logger_log(LOG_INFO, 
        "[Custom Callback] Node %u got frame ID=0x%03X", 
        node->node_id, 
        frame->id);
    node->frames_received++;
}

int main() {
    logger_init(LOG_DEBUG);

    CANBus bus;
    can_bus_init(&bus);

    CANNode node1;
    CANNode node2;

    node_init(&node1, 1, &bus);
    node_init(&node2, 2, &bus);

    // Override node2 callback
    node2.on_receive = custom_receive;

    can_bus_add_node(&bus, &node1);
    can_bus_add_node(&bus, &node2);

    CANFrame frame = {
        .id = 0x100,
        .dlc = 3,
        .data = {0xAA, 0xBB, 0xCC},
        .rtr = false
    };

    node_send(&node1, &frame);

    can_bus_process(&bus);

    logger_log(LOG_INFO, "Node1 sent: %u", node1.frames_sent);
    logger_log(LOG_INFO, "Node1 received: %u", node1.frames_received);
    logger_log(LOG_INFO, "Node2 received: %u", node2.frames_received);

    return 0;
}
