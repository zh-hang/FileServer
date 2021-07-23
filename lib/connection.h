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

#ifndef CONNECTION_H
#define CONNECTION_H

#define BUFF_SIZE 4086
#define QUEQUE_SIZE 10
#define TCP_PORT 12345

typedef int conn_type;
#define TCP_TYPE 0
#define UDP_TYPE 1

class Connection{
public:
	int fd;
	conn_type type;
	
	Connection(int type=TCP_TYPE);
	~Connection(){
		close(this->fd);
	}
	void sendFileUDP();
	void recvFileUDP();
	void sendFileTCP();
	void recvFileTCP();
	static void sendMsg(int connfd,const std::string &msg);
	static std::string recvMsg(int connfd);

};

#endif
