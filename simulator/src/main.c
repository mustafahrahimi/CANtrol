#include <stdio.h>
#include <unistd.h>
#include "can_bus.h"
#include "can_node.h"
#include "can_driver.h"
#include "can_frame.h"
#include "logger.h"
#include "tcp_client.h"

int main(void) {
    setvbuf(stdout, NULL, _IONBF, 0);
    logger_init(LOG_INFO);

    int tcp_fd = tcp_client_connect("bridge", 9000);
    if (tcp_fd < 0) {
        logger_log(LOG_ERROR, "Failed to connect to bridge");
        return -1;
    }
    logger_log(LOG_INFO, "Connected to bridge");

    CANBus bus;
    can_bus_init(&bus);

    CANDriver* driver = can_driver_create();
    if (!driver) return -1;

    CANNode nodeA, nodeB, nodeC, nodeD;
    node_init(&nodeA, 1, &bus);
    node_init(&nodeB, 2, &bus);
    node_init(&nodeC, 3, &bus);
    node_init(&nodeD, 4, &bus);

    can_bus_add_node(&bus, &nodeA);
    can_bus_add_node(&bus, &nodeB);
    can_bus_add_node(&bus, &nodeC);
    can_bus_add_node(&bus, &nodeD);

    can_driver_add_node(driver, &nodeA);
    can_driver_add_node(driver, &nodeB);
    can_driver_add_node(driver, &nodeC);
    can_driver_add_node(driver, &nodeD);

    uint8_t dataA[2] = {1, 2};
    uint8_t dataB[3] = {10, 20, 30};
    uint8_t dataC[4] = {100, 110, 120, 130};
    uint8_t dataD[1] = {255};

    while (1) {
        CANFrame frameA = can_frame_create(0x101, 2, dataA);
        CANFrame frameB = can_frame_create(0x202, 3, dataB);
        CANFrame frameC = can_frame_create(0x303, 4, dataC);
        CANFrame frameD = can_frame_create(0x404, 1, dataD);

        can_bus_send(&bus, &frameA);
        can_bus_send(&bus, &frameB);
        can_bus_send(&bus, &frameC);
        can_bus_send(&bus, &frameD);

        can_bus_process(&bus);

        tcp_client_send_frame(tcp_fd, &nodeA, &frameA);
        tcp_client_send_frame(tcp_fd, &nodeB, &frameB);
        tcp_client_send_frame(tcp_fd, &nodeC, &frameC);
        tcp_client_send_frame(tcp_fd, &nodeD, &frameD);

        sleep(1);
    }

    tcp_client_close(tcp_fd);
    can_driver_destroy(driver);

    return 0;
}
