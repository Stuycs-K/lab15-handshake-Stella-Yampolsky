#include "pipe_networking.h"

int send_message(int pipe_fd, struct message *msg) {
    if (write(pipe_fd, msg, sizeof(struct message)) == -1) {
        perror("Error sending message");
        exit(1);
    }
    return 0;
}

int receive_message(int pipe_fd, struct message *msg) {
    if (read(pipe_fd, msg, sizeof(struct message)) == -1) {
        perror("Error receiving message");
        exit(1);
    }
    return 0;
}

int server_setup() {
    if (access(WKP, F_OK) != -1) {
        remove(WKP);
    }
    if (mkfifo(WKP, 0666) == -1) {
        perror("Error creating WKP");
        exit(1);
    }
    int from_client = open(WKP, O_RDONLY);
    if (from_client == -1) {
        perror("Error opening WKP");
        exit(1);
    }
    return from_client;
}

int server_handshake(int *to_client) {
    int from_client = server_setup();
    *to_client = open(WKP, O_WRONLY);
    if (*to_client == -1) {
        perror("Error opening WKP for writing");
        exit(1);
    }

    struct message msg;
    receive_message(from_client, &msg);
    if (msg.type != SYN) {
        perror("Expected SYN message");
        exit(1);
    }

    msg.type = SYN_ACK;
    send_message(*to_client, &msg);

    receive_message(from_client, &msg);
    if (msg.type != ACK) {
        perror("Expected ACK message");
        exit(1);
    }

    return from_client;
}

int client_handshake(int *to_server) {
    *to_server = open(WKP, O_WRONLY);
    if (*to_server == -1) {
        perror("Error opening WKP for writing");
        exit(1);
    }

    struct message msg;
    msg.type = SYN;
    send_message(*to_server, &msg);

    receive_message(*to_server, &msg);
    if (msg.type != SYN_ACK) {
        perror("Expected SYN_ACK message");
        exit(1);
    }

    msg.type = ACK;
    send_message(*to_server, &msg);

    return 0;
}
