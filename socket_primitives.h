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

typedef enum {
    LS_NOT_LISTENING,
    LS_LISTENING,
    LS_COUNT,
} ListenStatus;

typedef struct Socket {
    int file_descriptor;
    Address_t address;
    Port_t port;
    ListenStatus listening = LS_NOT_LISTENING;
    uint16_t connections_permitted;
} * Socket;

Socket get_peer_by_address(Address_t address);

Socket socket(SocketType socket_type, char * sockets_dir);

int bind(Socket sock, Address_t addr, Port_t port);

int listen(Socket sock, uint16_t connections_permitted);

int accept(Socket listening_socket, Address_t address, char * clients_dir);

int connect(Address_t server_address);

int close(Socket * socket);