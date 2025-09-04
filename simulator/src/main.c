#include <stdio.h>
#include "logger.h"
#include "can_frame.h"

int main() {
    logger_init(LOG_DEBUG);

    logger_log(LOG_INFO, "Starting CANtrol basic test...");
    logger_log(LOG_DEBUG, "Logger is working fine");

    CANFrame frame = {
        .id = 0x123,
        .dlc = 4,
        .data = {0x10, 0x20, 0x30, 0x40},
        .rtr = false
    };

    if (can_frame_validate(&frame)) {
        logger_log(LOG_INFO, "Frame valid! ID=0x%03X DLC=%d", frame.id, frame.dlc);
    } else {
        logger_log(LOG_ERROR, "Frame failed validation");
    }

    CANFrame bad_frame = {
        .id = 0xFFFE,    // Invalid: > 11-bit
        .dlc = 10        // Invalid DLC
    };

    if (!can_frame_validate(&bad_frame)) {
        logger_log(LOG_WARN, "Invalid frame correctly rejected");
    } else {
        logger_log(LOG_ERROR, "Invalid frame should NOT pass validation!");
    }

    return 0;
}