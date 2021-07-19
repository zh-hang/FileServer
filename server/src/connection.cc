#include "connection.h"

Connection::Connection(int type){
	bzero(&this->addr,sizeof(this->addr));
	this->addr.sin_family=AF_INET;
	this->addr.sin_addr.s_addr=htonl(INADDR_ANY);
	this->addr.sin_port=htons(SERVER_PORT);
	if(type==UDP_TYPE){
		this->fd=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
		if(this->fd<0)
			writeLog("create socket failed\n");
		if(bind(this->fd,(struct sockaddr*)&this->addr,sizeof(this->addr))<0)
			writeLog("bind failed.\n");
	}
}

void Connection::listen(){
	char buf[BUFF_SIZE];
	while(1){
		bzero(buf,BUFF_SIZE);
		struct sockaddr_in client;
		socklen_t len = sizeof(client);
		ssize_t s = recvfrom(this->fd, buf, BUFF_SIZE,0,(struct sockaddr*)&client,&len);
		if(s > 0){
			buf[s] = '\0';
			std::cout<<inet_ntoa(client.sin_addr)<<ntohs(client.sin_port)<<buf;
			sendto(this->fd,buf,strlen(buf),0,(struct  sockaddr*)&client,sizeof(client));
		}
	}
}
