#pragma once

#include <stdint.h>
#include <stdbool.h>
#include "can_frame.h"

#define CAN_BUS_MAX_NODES 32
#define CAN_BUS_QUEUE_SIZE 128

typedef struct CANNode CANNode;

typedef struct {
    CANNode* nodes[CAN_BUS_MAX_NODES];
    uint32_t node_count;
    CANFrame queue[CAN_BUS_QUEUE_SIZE];
    uint32_t q_head;
    uint32_t q_tail;
} CANBus;

void can_bus_init(CANBus* bus);
bool can_bus_add_node(CANBus* bus, CANNode* node);
bool can_bus_send(CANBus* bus, const CANFrame* frame);
void can_bus_process(CANBus* bus);
