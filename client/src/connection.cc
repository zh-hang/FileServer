#include "connection.h"

Connection::Connection(int type,std::string server_addr){
	bzero(&this->addr,sizeof(this->addr));
	this->addr.sin_family=AF_INET;
	this->addr.sinaddr.saddr=inet_addr(server_addr.c_str());
	this->addr.sin_port=htons(_PORT);
	if(type==UDP_TYPE){
		this->fd=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
		if(this->fd<0){
			std::cerr<<"create socket failed\n";
			exit(0);
		}
	}
}

void Connection::send(){
	char buf[BUFF_SIZE];
	std::cin>>buf;
	std::cout<<buf;
	sendto(fd,buf,BUFF_SIZE,0,(struct sockaddr*)&this->addr,sizeof(this->addr));
	ssize_t s=recvfrom(fd,buf,siezeof(buf),0,NULL,NULL);
	if(s>0)
		std::cout<<buf;
}