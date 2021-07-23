#include "serverConnection.h"

ServerConnection::ServerConnection(int type):Connection{type}
{
	if (bind(this->fd, (struct sockaddr *)&this->addr, sizeof(this->addr)) < 0){
		writeLog("bind failed.\n");
		exit(0);
	}
    std::cout<<"bind successfully.\n";
	if(listen(this->fd,QUEQUE_SIZE)<0)
		writeLog("listen failed.\n");
    std::cout<<"listen successfully.\n";
}

int ServerConnection::acceptTCP(){
    std::cout<<"start accept.\n";
	int connfd=accept(this->fd,0,0);
	if(connfd<0)
		writeLog("accept failed.\n");
	return connfd;
}