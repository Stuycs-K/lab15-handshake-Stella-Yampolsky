#include "pipe_networking.h"

int main() {
    int to_server;
    int from_server;

    client_handshake(&to_server);

    struct message msg;
    msg.type = MESSAGE;
    strcpy(msg.data, "Hello from client!");
    send_message(to_server, &msg);

    receive_message(to_server, &msg);
    printf("Received message: %s\n", msg.data);

    return 0;
}
