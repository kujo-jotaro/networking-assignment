#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include <errno.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>


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

Message_t message_constructor(int length, const char * content);

void message_destructor(Message_t msg);

int message_send(int fd, const char * content);

Socket socket(SocketType socket_type, char * sockets_dir);

void bind(Socket sock, Address_t addr, Port_t port);

void listen(Socket sock, uint16_t connections_permitted);

CommChannel accept(Socket listening_socket, Address_t address, char * clients_dir);

Socket get_peer_by_address(Address_t address);

// construir mensaje y enviar con write
void connect(Address_t server_address);

// The client sends a signal (writes) through its channel telling it to close
// Frees up the port
// Destructs itself
void client_close_connection();

// The server catches the signal (reads) through the channel
// Frees up the port
// Destructs the client socket
void server_close_connection();

void close(Socket * socket);