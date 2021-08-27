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
    int port;
public:
	
    struct sockaddr_in addr;

    Connection(){
    }
	~Connection(){
	}
    
    int getFd(){
        return this->fd;
    }

    int getPort(){
        return this->port;
    }

    void setAddr(sockaddr_in addr){
        this->addr=std::move(addr);
    }

	void closeSelf(){
        close(this->fd);
    }

    void bindServer(){
        if (bind(this->fd, (struct sockaddr *)&this->addr, sizeof(this->addr)) < 0) {
            writeLog("bind failed.\n");
            exit(0);
        }
        std::cout << "bind successfully.\n";
    }
	
	static int sendMsg(int connfd,const std::string &msg);
	static std::string recvMsg(int connfd);
	static int sendMsg(int connfd,const std::string &msg,const sockaddr*ra);
	static std::string recvMsg(int connfd,sockaddr*sa);
    static int trySelect(fd_set *readfds,fd_set *writefds,fd_set *exceptfds,timeval &timeout);
    static void errnoDeal();
};

class TCPConnection:public Connection{
    public:
    TCPConnection(){
        init();
        this->port=TCP_PORT;
		this->addr.sin_addr.s_addr = htonl(INADDR_ANY);
		this->addr.sin_port = htons(TCP_PORT);
    }
    TCPConnection(const char*addr){
        init();
        this->port=TCP_PORT;
		this->addr.sin_addr.s_addr = inet_addr(addr);
		this->addr.sin_port = htons(TCP_PORT);
    }
    TCPConnection(int port){
        init();
        this->port=port;
		this->addr.sin_addr.s_addr = htonl(INADDR_ANY);
		this->addr.sin_port = htons(port);
    }
    TCPConnection(const char*addr,int port){
        init();
        this->port=port;
		this->addr.sin_addr.s_addr = inet_addr(addr);
		this->addr.sin_port = htons(port);
    }

    void TCPListen();
    int TCPAccept();

    private:
    void init(){
        this->fd=socket(AF_INET, SOCK_STREAM,IPPROTO_TCP);
		if (this->fd < 0){
			writeLog("create socket failed\n");
			exit(0);
		}
        std::cout<<"create socket succussfully.\n";
		bzero(&this->addr, sizeof(this->addr));
		this->addr.sin_family = AF_INET;
    }
};

class UDPConnection:public Connection{
    public:
    UDPConnection(int port){
        init();
        this->port=port;
		this->addr.sin_addr.s_addr = htonl(INADDR_ANY);
		this->addr.sin_port = htons(port);
    }

    UDPConnection(const char*addr,int port){
        init();
        this->port=port;
		this->addr.sin_addr.s_addr = inet_addr(addr);
		this->addr.sin_port = htons(port);
    }

	void sendFile(std::string filename,sockaddr *ra);
	void recvFile(std::string filename);

    private:
    void init(){
        this->fd=socket(AF_INET, SOCK_DGRAM,IPPROTO_UDP);
		if (this->fd < 0){
			writeLog("create socket failed\n");
			exit(0);
		}
        std::cout<<"create socket succussfully.\n";
		bzero(&this->addr, sizeof(this->addr));
		this->addr.sin_family = AF_INET;
    }
};

#endif
