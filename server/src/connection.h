#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include "log.h"

#ifndef CONNECTION_H
#define CONNECTION_H

#define BUFF_SIZE 4086
#define SERVER_PORT 12345
#define QUEQUE_SIZE 10

typedef int conn_type;
#define TCP_TYPE 0
#define UDP_TYPE 1

class Connection{
	public:
		int serverfd;
		struct sockaddr_in server_addr;
		Connection(int type){
			bzero(&this->server_addr,sizeof(this->server_addr));
			this->server_addr.sin_family=AF_INET;
			this->server_addr.sin_addr.s_addr=htonl(INADDR_ANY);
			this->server_addr.sin_port=htons(SERVER_PORT);
			if(type==UDP_TYPE){
				this->serverfd=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
				if(this->serverfd<0)
					writeLog("create socket failed\n");
				if(bind(this->serverfd,(struct sockaddr*)&this->server_addr,sizeof(this->server_addr))<0)
					writeLog("bind failed.\n");
			}
		}
		~Connection(){
			close(this->serverfd);
		}
		void listen(){
		}

};

#endif
