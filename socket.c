#include "socket_primitives.h"

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