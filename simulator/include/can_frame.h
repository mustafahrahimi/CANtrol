#pragma once

#include <stdint.h>
#include <string.h>
#include <stdbool.h>

#define CAN_MAX_DATA_LEN 8

typedef struct {
	uint32_t id; // 11-bit identifier
	uint8_t dlc; // data length code (0-8)
	uint8_t data[CAN_MAX_DATA_LEN]; // payload
	bool rtr; // remote transmission request
} CANFrame;

CANFrame can_frame_create(uint32_t id, uint8_t dlc, const uint8_t* data);
bool can_frame_validate(const CANFrame* frame);

