#include "socket_primitives.h"

Socket socket(char * sockets_dir) {
    Socket socket = malloc(sizeof(struct Socket));
    int fd = open(sockets_dir, O_TMPFILE | O_RDWR,
                                S_IRUSR | S_IWUSR);
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

int accept(Socket listening_socket, Address_t address, char * clients_dir) {
}

Socket get_peer_by_address(Address_t address) {

}

// construir mensaje y enviar con write
int connect(Address_t server_address) {
    char * content = "connect"; // not quite sure, should send a signal instead
    const int length = strlen(content);
    Socket server = get_socket_by_address(server_address);
    write(server->file_descriptor, (void *) content, length)
}

void close(Socket * socket) {
    int res = close(socket->fd);
}