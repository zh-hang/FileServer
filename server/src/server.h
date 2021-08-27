#ifndef SERVER_H
#define SERVER_H

#include "../../lib/typedef.h"
#include "../../lib/connection.h"

struct ServerOpt {
    int op;
    void (*opfun)(int fd,UDPConnection &udp);
};

void sendFile(int fd,UDPConnection &file_connection);
void recvFile(int fd,UDPConnection &file_connection);
void delFile(int fd,UDPConnection &file_connection);

#endif