#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <arpa/inet.h>
#include <fstream>

#include "connection.h"

#ifndef CLIENT_CONNECTION_H
#define CLIENT_CONNECTION_H

class ClientConnection
{
public:
	ClientConnection(int type, std::string server_addr);
	~ClientConnection()
	{
		close(this->fd);
	}

private:
	struct sockaddr_in server_addr;
};

#endif
