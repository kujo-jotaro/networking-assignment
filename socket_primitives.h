#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>

#define CHECK(X) ({int __val = (X); (__val ==-1 ? \
                ({fprintf(stderr,"ERROR (" __FILE__ ":%d) --\
                %s\n",__LINE__,strerror(errno)); \
                return;-1;}) : __val); })

// No IPv6. An address can be represented by its four part, 8-byte each
// representation; or by its full 32-bit representation. For commodity.
typedef struct {
    union {
        uint8_t address_in_chunks[4],
        uint32_t address;
    };
} Address_t;

// It could just be a typedef short, but since I still don't know if anything
// could be added, it's best to leave it as a struct.
typedef struct {
    uint16_t portno;
} Port_t;

// Messages are the stream of bytes the client and server send to each other
// via a communication channel
typedef struct Message {
    void *content;
    int length;
} * Message_t;

typedef enum {
    LS_NOT_LISTENING,
    LS_LISTENING,
    LS_COUNT,
} ListenStatus;

typedef enum {
    ST_SERVER,
    ST_CLIENT,
    ST_COUNT,
} SocketType;

typedef struct Socket {
    SocketType socket_type;
    int file_descriptor;
    Address_t address;
    Port_t port;
    ListenStatus listening = LS_NOT_LISTENING;
    uint16_t connections_permitted;
} * Socket;

typedef struct comm_channel {
    struct socket * server;
    struct socket * client;
    Message_t * messages;
} * CommChannel;

Message_t message_constructor(int length, const char * content) {
    Message_t msg = malloc(sizeof(struct Message));
    msg->length = length;
    msg->content = calloc(length, sizeof(* content));
    strncopy((char *) msg->content, content, length);

    return msg;
}

void message_destructor(Message_t msg) {
    free(msg->content);
    free(msg);
}

int message_send(int fd, const char * content) {
    static int res, attempts = 0;
    int length = strlen(content);
    Message_t msg = message_constructor(length, content);
    while ((res = write(fd, msg->content, msg->length)) == -1 && attempts < 5) {
        fprintf(stderr,"ERROR %s\n",strerror(errno));
        attempts++;
        wait(1);
    }
    if (res == -1)
        fprintf(stderr,"ERROR: CANNOT SEND TO OTHER HOST\n");
    message_destructor(msg);
    
    return res;
}

Socket socket(SocketType socket_type, char * sockets_dir) {
    Socket socket = malloc(sizeof(struct Socket));
    int fd = open(sockets_dir, O_TMPFILE | O_RDWR,
                                S_IRUSR | S_IWUSR);
    socket->socket_type = socket_type;
    socket->file_descriptor = fd;

    return socket;
}

void bind(Socket sock, Address_t addr, Port_t port) {
    sock->address = addr;
    sock->port = port;
}

void listen(Socket sock, uint16_t connections_permitted) {
    sock->listening = LISTENING;
    sock->connections_permitted = connections_permitted;
}

CommChannel accept(Socket listening_socket, Address_t address, char * clients_dir) {
    CommChannel comm = malloc(sizeof(struct comm_channel));
    if (pending_connections_set != NULL) {
        Socket client_socket = socket(ST_CLIENT, clients_dir)
        comm->server = listening_socket;
        comm->client = client_socket;
        comm->messages = NULL;
    }

    return comm;
}

Socket get_peer_by_address(Address_t address) {

}

// construir mensaje y enviar con write
void connect(Address_t server_address) {
    char * content = "connect"; // not quite sure, should send a signal instead
    const int length = strlen(content);
    Socket server = get_socket_by_address(server_address);
    write(server->file_descriptor, (void *) content, length)
}

// The client sends a signal (writes) through its channel telling it to close
// Frees up the port
// Destructs itself
void client_close_connection() {;}

// The server catches the signal (reads) through the channel
// Frees up the port
// Destructs the client socket
void server_close_connection() {;}

void close(Socket * socket) {
    if ((SocketType) socket == ST_SERVER) {
        server_close_connection();
    }
    else {
        client_close_connection();
    }
    int res = close(socket->fd);
}