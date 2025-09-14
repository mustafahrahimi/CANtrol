#pragma once
#include <stdint.h>
#include "can_frame.h"

void export_frame_json(const CANFrame* frame, uint32_t source_node);
