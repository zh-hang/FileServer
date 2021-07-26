#include "clientConnection.h"

ClientConnection::ClientConnection(int type, std::string server_addr,int port):Connection{type}
{
	bzero(&this->addr,sizeof(this->addr));
	this->addr.sin_family=AF_INET;
	this->addr.sin_addr.s_addr=inet_addr(server_addr.c_str());
	this->addr.sin_port=htons(port);
	if(this->type==TCP_TYPE){
		if(connect(this->fd,(struct sockaddr*)&this->addr,sizeof(this->addr))<0){
			std::cout<<"connect failed.\n";
			exit(0);
		}
        std::cout<<"connect successfully.\n";
	}
}

