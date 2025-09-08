#include <stdlib.h>
#include <string.h>
#include "can_driver.h"
#include "logger.h"

CANDriver* can_driver_create(void) {
    CANDriver* drv = calloc(1, sizeof(CANDriver));
    if (!drv) return NULL;
    logger_log(LOG_INFO, "CAN driver created");
    return drv;
}

void can_driver_destroy(CANDriver* driver) {
    if (!driver) return;
    logger_log(LOG_INFO, "CAN driver destroyed");
    free(driver);
}

int can_driver_add_node(CANDriver* driver, CANNode* node) {
    if (!driver || !node) return -1;
    if (driver->node_count >= MAX_DRIVER_NODES) return -2;

    driver->nodes[driver->node_count++] = node;
    logger_log(LOG_INFO, "Node %u added to driver", node->node_id);
    return 0;
}

int can_driver_send_raw(CANDriver* driver, CANNode* sender, const uint8_t* data, size_t len) {

    if (!driver || !data) 
        return -1;

    if (len != CAN_RAW_FRAME_SIZE) {
        logger_log(LOG_ERROR, "Driver: Invalid frame length %zu (expected %d bytes)", len, CAN_RAW_FRAME_SIZE);
        return -2;
    }

    CANFrame frame;
    if (!can_parse_raw_frame(data, len, &frame)) {
        logger_log(LOG_ERROR, "Driver: Failed to parse raw CAN frame");
        return -3;
    }

    logger_log(LOG_INFO, "Driver broadcasting frame ID=0x%03X", frame.id);

    // Deliver to every node except sender
    for (int i = 0; i < driver->node_count; i++) {
        CANNode* node = driver->nodes[i];

        if (node == sender)
            continue;

        if (node->on_receive)
            node->on_receive(node, &frame);
    }

    return 0;
}

