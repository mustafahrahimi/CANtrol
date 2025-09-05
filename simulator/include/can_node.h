#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "can_frame.h"
#include "can_bus.h"

struct CANBus;

typedef struct CANNode {
    uint32_t node_id;                     
    struct CANBus* bus;                   

    // Callback called by CANBus when a frame is received
    void (*on_receive)(struct CANNode* node, const CANFrame* frame);

    uint32_t frames_sent;
    uint32_t frames_received;
} CANNode;

static void default_on_receive(CANNode* node, const CANFrame* frame);   
void node_init(CANNode* node, uint32_t node_id, struct CANBus* bus);
bool node_send(CANNode* node, const CANFrame* frame);
