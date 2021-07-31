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
    struct sockaddr_in addr;
	
    Connection(){
    }
	Connection(int type);
	~Connection(){
	}
    
    void close_self(){
        close(this->fd);
    }
	void sendFileUDP(std::string filename,sockaddr *ra);
	void recvFileUDP(std::string filename);
	void sendFileTCP();
	void recvFileTCP();
	
	static void sendMsg(int connfd,const std::string &msg);
	static std::string recvMsg(int connfd);
	static void sendMsg(int connfd,const std::string &msg,const sockaddr*ra);
	static std::string recvMsg(int connfd,sockaddr*sa);

	void close_self(){
        close(this->fd);
    }
	
};

#endif
