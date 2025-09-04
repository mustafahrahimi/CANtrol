#include "can_frame.h"

bool can_frame_validate(const CANFrame* frame) {
	if (!frame) return false;
	if (frame->id > 0x7FF) return false; 
	if (frame->dlc > CAN_MAX_DATA_LEN) return false;
	return true;
}

