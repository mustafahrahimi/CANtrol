#pragma once

#include "can_node.h"
#include "parser.h"

#define MAX_DRIVER_NODES 16
#define MAX_CAN_FRAME_BYTES 16

typedef struct {
    CANNode* nodes[MAX_DRIVER_NODES];
    int node_count;
} CANDriver;

CANDriver* can_driver_create(void);
void can_driver_destroy(CANDriver* driver);

int can_driver_add_node(CANDriver* driver, CANNode* node);

/**
 * Sends a raw byte buffer through the driver.
 * All nodes except the sender receive it.
 */
int can_driver_send_raw(CANDriver* driver,
                        CANNode* sender,
                        const uint8_t* data,
                        size_t len);

