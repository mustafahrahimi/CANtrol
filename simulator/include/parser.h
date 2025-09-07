#pragma once

#include <stdint.h>
#include <stdbool.h>
#include <stddef.h>
#include "can_frame.h"

#define PARSER_BUFFER_SIZE 13 // 4 bytes ID + 1 byte DLC + 8 bytes data

// Encode a CANFrame into a byte array
// Returns number of bytes written, 0 on error
size_t encode_can_frame(const CANFrame* frame, uint8_t* buffer, size_t buffer_size);
    
// Decode a byte array into a CANFrame
// Returns true on success, false on error
bool decode_can_frame(const uint8_t* buffer, size_t buffer_size, CANFrame* frame);      