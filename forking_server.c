#include "pipe_networking.h"
#include <sys/wait.h>

int main() {
    int to_client;
    int from_client;

    while (1) {
        from_client = server_handshake(&to_client);

        pid_t pid = fork();
        if (pid == 0) {
            struct message msg;
            receive_message(from_client, &msg);
            printf("Received message: %s\n", msg.data);

            msg.type = MESSAGE;
            strcpy(msg.data, "Hello from forking server!");
            send_message(to_client, &msg);

            close(from_client);
            close(to_client);
            exit(0);
        } else {
            wait(NULL);
        }
    }

    return 0;
}
