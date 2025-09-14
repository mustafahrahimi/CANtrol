#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "can_frame.h"
#include "can_bus.h"

typedef struct CANNode {
    uint32_t node_id;                     
    CANBus* bus;                          

    // Callback called by CANBus when a frame is received
    void (*on_receive)(struct CANNode* node, const CANFrame* frame);

    uint32_t frames_sent;
    uint32_t frames_received;
} CANNode;

void node_init(CANNode* node, uint32_t node_id, CANBus* bus);
bool node_send(CANNode* node, const CANFrame* frame);
