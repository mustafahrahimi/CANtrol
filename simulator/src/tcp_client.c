#define _POSIX_C_SOURCE 200112L
#include "tcp_client.h"
#include <stdio.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <netdb.h>
#include <errno.h>


int tcp_client_connect(const char* host, int port) {
    char port_str[16];
    snprintf(port_str, sizeof(port_str), "%d", port);

    struct addrinfo hints;
    memset(&hints, 0, sizeof(hints));
    hints.ai_socktype = SOCK_STREAM;
    hints.ai_family = AF_UNSPEC;

    struct addrinfo* res = NULL;
    int gai_err = getaddrinfo(host, port_str, &hints, &res);
    if (gai_err != 0) {
        return -1;
    }

    int sock = -1;
    for (struct addrinfo* p = res; p != NULL; p = p->ai_next) {
        sock = socket(p->ai_family, p->ai_socktype, p->ai_protocol);
        if (sock < 0) {
            continue;
        }
        if (connect(sock, p->ai_addr, p->ai_addrlen) == 0) {
            break; // connected
        }
        close(sock);
        sock = -1;
    }

    freeaddrinfo(res);
    return sock; // -1 if none succeeded
}

void tcp_client_send_frame(int tcp_fd, CANNode* node, const CANFrame* frame) {
    if (tcp_fd < 0 || !node || !frame) return;

    char buf[256];
    int len = snprintf(buf, sizeof(buf),
        "{\"id\":%u,\"dlc\":%u,\"data\":[%d,%d,%d,%d,%d,%d,%d,%d],\"node\":%u}\n",
        frame->id, frame->dlc,
        frame->data[0], frame->data[1], frame->data[2], frame->data[3],
        frame->data[4], frame->data[5], frame->data[6], frame->data[7],
        node->node_id
    );
    send(tcp_fd, buf, len, 0);
}

void tcp_client_close(int tcp_fd) {
    if (tcp_fd >= 0) close(tcp_fd);
}
