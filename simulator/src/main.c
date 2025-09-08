#include <stdio.h>
#include "can_driver.h"
#include "can_node.h"
#include "can_frame.h"
#include "can_bus.h"
#include "parser.h"
#include "logger.h"

// Callback function for nodes
void node_receive_callback(CANNode* n, const CANFrame* f) {
    if (!n || !f) return;
    n->frames_received++;
    logger_log(LOG_INFO, "Node %u received frame ID=0x%03X dlc=%u",
               n->node_id, f->id, f->dlc);
}

int main(void) {
    logger_init(LOG_INFO);

    CANDriver* driver = can_driver_create();
    if (!driver) return -1;

    // Dummy bus
    CANBus bus;

    CANNode nodeA, nodeB;
    node_init(&nodeA, 1, &bus);
    node_init(&nodeB, 2, &bus);

    nodeA.on_receive = node_receive_callback;
    nodeB.on_receive = node_receive_callback;

    can_driver_add_node(driver, &nodeA);
    can_driver_add_node(driver, &nodeB);

    CANFrame frameA = {0};
    frameA.id = 0x123;
    frameA.dlc = 3;
    frameA.data[0] = 10;
    frameA.data[1] = 20;
    frameA.data[2] = 30;

    uint8_t buf[CAN_RAW_FRAME_SIZE];
    size_t len = can_encoder_frame(&frameA, buf, sizeof(buf));
    can_driver_send_raw(driver, &nodeA, buf, len);

    logger_log(LOG_INFO, "Node A frames_received: %u", nodeA.frames_received);
    logger_log(LOG_INFO, "Node B frames_received: %u", nodeB.frames_received);

    can_driver_destroy(driver);
    return 0;
}
