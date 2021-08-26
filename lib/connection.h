#include <bits/types/struct_timeval.h>
#include <sys/select.h>
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
#include <sys/select.h>
#include <sys/time.h>

#include "log.h"

#ifndef CONNECTION_H
#define CONNECTION_H

#define BUFF_SIZE 1024
#define QUEQUE_SIZE 10
#define TCP_PORT 12345

typedef int conn_type;
#define TCP_TYPE 0
#define UDP_TYPE 1



class Connection{
protected:

	int fd;
	conn_type type;
    struct sockaddr_in addr;
public:
	
    Connection(){
    }
	~Connection(){
	}
    
    int getFd(){
        return this->fd;
    }

    struct sockaddr_in getAddr(){
        return this->addr;
    }

	void close_self(){
        close(this->fd);
    }
	
	static void sendMsg(int connfd,const std::string &msg);
	static std::string recvMsg(int connfd);
	static void sendMsg(int connfd,const std::string &msg,const sockaddr*ra);
	static std::string recvMsg(int connfd,sockaddr*sa);

    
	private:
    static int trySelect(fd_set &readfds,fd_set &writefds,fd_set &exceptfds,timeval &timeout);
    static void errnoDeal();
};

class TCPConnection:public Connection{
    public:
    TCPConnection(){
        this->fd=socket(AF_INET, SOCK_STREAM,IPPROTO_TCP);
		if (this->fd < 0){
			writeLog("create socket failed\n");
			exit(0);
		}
        std::cout<<"create socket succussfully.\n";
		bzero(&this->addr, sizeof(this->addr));
		this->addr.sin_family = AF_INET;
		this->addr.sin_addr.s_addr = htonl(INADDR_ANY);
		this->addr.sin_port = htons(TCP_PORT);
    }
};

class UDPConnection:public Connection{
    public:
    UDPConnection(){
        this->fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (this->fd < 0){
			writeLog("create socket failed\n");
			exit(0);
		}
        std::cout<<"create socket succussfully.\n";
    }

	void sendFile(std::string filename,sockaddr *ra);
	void recvFile(std::string filename);

};

#endif
