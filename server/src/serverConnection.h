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
#include <sys/syscall.h> 

#include "../../lib/log.h"
#include "../../lib/connection.h"

#ifndef SERVER_CONNECTION_H
#define SERVER_CONNECTION_H

class ServerConnection:public Connection{
public:
    int port;
    ServerConnection():Connection(){
    }
	ServerConnection(int type,int port=TCP_PORT);
	~ServerConnection(){
	}
	int acceptTCP();
};

#endif
