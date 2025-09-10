#include <stdio.h>
#include <unistd.h>  
#include "can_bus.h"
#include "can_node.h"
#include "can_driver.h"
#include "can_frame.h"
#include "logger.h"

int main(void) {
    logger_init(LOG_INFO);

    CANBus bus;
    can_bus_init(&bus);

    CANDriver* driver = can_driver_create();
    if (!driver) return -1;

    CANNode nodeA, nodeB;
    node_init(&nodeA, 1, &bus);
    node_init(&nodeB, 2, &bus);

    can_bus_add_node(&bus, &nodeA);
    can_bus_add_node(&bus, &nodeB);
    can_driver_add_node(driver, &nodeA);
    can_driver_add_node(driver, &nodeB);

    uint8_t dataA[2] = {1, 2};
    uint8_t dataB[3] = {10, 20, 30};

    // Simulate sending frames periodically
    for (int i = 0; i < 5; i++) { // 5 cycles
        CANFrame frameA = can_frame_create(0x101 + i, 2, dataA);
        CANFrame frameB = can_frame_create(0x202 + i, 3, dataB);

        can_bus_send(&bus, &frameA);
        can_bus_send(&bus, &frameB);

        can_bus_process(&bus);

        logger_log(LOG_INFO, "Cycle %d: Node A frames_received=%u, Node B frames_received=%u",
                   i+1, nodeA.frames_received, nodeB.frames_received);

        sleep(1);
    }

    can_driver_destroy(driver);

    return 0;
}
