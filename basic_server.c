#include "pipe_networking.h"

int main() {
    int to_client;
    int from_client;

    from_client = server_handshake(&to_client);

    struct message msg;
    receive_message(from_client, &msg);
    printf("Received message: %s\n", msg.data);

    msg.type = MESSAGE;
    strcpy(msg.data, "Hello from server!");
    send_message(to_client, &msg);

    return 0;
}
