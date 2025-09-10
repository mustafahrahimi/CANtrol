#include "can_frame.h"

CANFrame can_frame_create(uint32_t id, uint8_t dlc, const uint8_t* data) {
    CANFrame frame;
    memset(&frame, 0, sizeof(CANFrame));
    frame.id = id;
    frame.dlc = dlc;
    if (data && dlc > 0) {
        memcpy(frame.data, data, dlc);
    }
    frame.rtr = 0;
    return frame;
}

bool can_frame_validate(const CANFrame* frame) {
	if (!frame) return false;
	if (frame->id > 0x7FF) return false; 
	if (frame->dlc > CAN_MAX_DATA_LEN) return false;
	return true;
}

