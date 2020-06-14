#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#define handle_error(msg) \
    do { perror(msg); exit(EXIT_FAILURE); } while (0)

typedef struct {
    union {
        uint8_t address_in_chunks[4],
        uint32_t address;
    };
} Address_t;

typedef struct {
    uint16_t portno;
} Port_t;

typedef * struct Message {
    void *content;
    uint32_t length;
} Message_t;

typedef enum {
    NOT_LISTENING,
    LISTENING,
} ListenStatus;

typedef struct {
    uint16_t file_descriptor;
    Address_t address;
    Port_t port;
    ListenStatus listening = NOT_LISTENING;
} Socket;

typedef * struct comm_channel {
    struct socket * server;
    struct socket * client;
    message_t * messages;
} CommChannel;

Message_t message_constructor (int32_t length, char * content) {
    Message_t msg = malloc(sizeof(struct message));
    msg->length = length;
    msg->content = calloc(length, sizeof(* content));
    strncopy((char *) msg->content, content, length);

    return msg;
}

void message_destructor (message_t msg) {
    free(msg->content);
    free(msg);
}

Socket socket (char * socket_path) {
    int fd = open(socket_path, O_RDWR);
    Socket socket = { .file_descriptor = fd };

    return socket;
}

void bind (socket sock, address_t addr) {
    sock.address = addr;
}

void listen (socket sock) {
    sock.listening = LISTENING;
}

CommChannel accept(Socket listening_socket, Address_t address) {
    CommChannel comm = malloc(sizeof(struct comm_channel));
    if (pending_connections_set != NULL) {
        // TODO: create_client_socket()
        Socket client_socket = create_client_socket();
        comm->server = listening_socket;
        comm->client = client_socket;
        comm->messages = NULL;
    }

    return comm;
}