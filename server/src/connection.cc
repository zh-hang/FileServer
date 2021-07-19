#include "connection.h"

Connection::Connection(int type){
	bzero(&this->addr,sizeof(this->addr));
	this->addr.sin_family=AF_INET;
	this->addr.sinaddr.saddr=htonl(INADDR_ANY);
	this->addr.sin_port=htons(_PORT);
	if(type==UDP_TYPE){
		this->fd=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
		if(this->fd<0)
			writeLog("create socket failed\n");
		if(bind(this->fd,(struct sockaddr*)&this->addr,sizeof(this->addr))<0)
			writeLog("bind failed.\n");
	}
}

void Connection::listen(){
	while(1){
		struct sockaddr_in client;
		socklen_t len = sizeof(client);
		ssize_t s = recvfrom(sock, buf, sizeof(buf)-1,0,(struct sockaddr*)&client,&len);
		if(s > 0){
			buf[s] = '\0';
			std::cout<<inet_ntoa(client.sinaddr)<<ntohs(client.sin_port)<<buf;
			sendto(sock,buf,strlen(buf),0,(struct  sockaddr*)&client,sizeof(client));
		}
	}
}