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

typedef int conn_type;
#define TCP_TYPE 0
#define UDP_TYPE 1

class Connection{
public:
	Connection(int type=TCP_TYPE);
	~Connection(){
		close(this->fd);
	}
	void listen();
	int accept();
	static void sendMsg(int connfd,const std::string &msg);
	static std::string recvMsg(int connfd);
private:

	int fd;
	conn_type type;
	struct sockaddr_in addr;
	void transFile(struct sockaddr*client);
};

#endif
