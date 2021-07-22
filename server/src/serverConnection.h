#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <arpa/inet.h>
#include <iostream>
#include <fstream>

#include "log.h"
#include "../../include/connection.h"

#ifndef SERVER_CONNECTION_H
#define SERVER_CONNECTION_H

#define BUFF_SIZE 4086
#define QUEQUE_SIZE 10

typedef int conn_type;
#define TCP_TYPE 0
#define UDP_TYPE 1

class ServerConnection{
public:
	ServerConnection(int type=TCP_TYPE);
	~ServerConnection(){
		close(this->fd);
	}
};

#endif
