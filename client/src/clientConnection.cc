#include "connection.h"

ClientConnection::ClientConnection(int type, std::string server_addr,int port=TCP_PORT):Connection(type)
{
	bzero(this->server_addr,sizeof(this->server_addr));
	this->server_addr.sin_family=AF_INET;
	this->server_addr.sin_addr.s_addr=htonl(server_addr.c_str());
	this->server_addr.sin_port=htons(port);
	if(this->type==TCP_TYPE){
		if(connect(this->fd,(struct sockaddr*)&this->server_addr,sizeof(this->server_addr))<0){
			std::cout<<"connect failed.\n";
			exit(0)
		}
	}
}

