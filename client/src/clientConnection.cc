#include "clientConnection.h"

ClientConnection::ClientConnection(int type, std::string server_addr,int port):Connection{type}
{
	bzero(&this->server_addr,sizeof(this->server_addr));
	this->server_addr.sin_family=AF_INET;
	this->server_addr.sin_addr.s_addr=htonl(inet_addr(server_addr.c_str()));
	this->server_addr.sin_port=htons(port);
	if(this->type==TCP_TYPE){
		if(connect(this->fd,(struct sockaddr*)&this->server_addr,sizeof(this->server_addr))<0){
			std::cout<<"connect failed.\n";
			exit(0);
		}
	}
}

