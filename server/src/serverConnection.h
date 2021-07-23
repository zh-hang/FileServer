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

#include "../../lib/log.h"
#include "../../lib/connection.h"

#ifndef SERVER_CONNECTION_H
#define SERVER_CONNECTION_H

class ServerConnection:public Connection{
public:
	ServerConnection(int type=TCP_TYPE);
	~ServerConnection(){
		close(this->fd);
	}
	int acceptTCP();
};

#endif
