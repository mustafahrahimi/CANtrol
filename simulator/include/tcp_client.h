#pragma once
#include <stdint.h>
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include "can_bus.h"
#include "can_node.h"
#include "can_frame.h"

int tcp_client_connect(const char* host, int port);
void tcp_client_send_frame(int tcp_fd, CANNode* node, const CANFrame* frame);
void tcp_client_close(int tcp_fd);
