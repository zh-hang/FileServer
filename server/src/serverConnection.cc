#include "serverConnection.h"
#include <cstdlib>

ServerConnection::ServerConnection(int type,int port):Connection{type}
{
    if(this->type==UDP_TYPE){
		bzero(&this->addr, sizeof(this->addr));
		this->addr.sin_family = AF_INET;
		this->addr.sin_addr.s_addr = htonl(INADDR_ANY);
		this->addr.sin_port = htons(port);
    }
	if (bind(this->fd, (struct sockaddr *)&this->addr, sizeof(this->addr)) < 0){
		writeLog("bind failed.\n");
		exit(0);
	}
    std::cout<<"bind successfully.\n";
    if(this->type==TCP_TYPE){
        if(listen(this->fd,QUEQUE_SIZE)<0){
            writeLog("listen failed.\n");
            exit(0);
        }
        std::cout<<"listen successfully.\n";
    }
}

int ServerConnection::acceptTCP(){
    std::cout<<"start accept.\n";
	int connfd=accept(this->fd,0,0);
	if(connfd<0)
		writeLog("accept failed.\n");
	return connfd;
}