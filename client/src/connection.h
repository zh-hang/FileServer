#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <unistd.h>
#include <string>
#include <iostream>
#include <arpa/inet.h>

#ifndef CONNECTION_H
#define CONNECTION_H

#define SERVER_PORT 12345
#define BUFF_SIZE 4086
#define QUEQUE_SIZE 10

typedef int conn_type;
#define TCP_TYPE 0
#define UDP_TYPE 1

class Connection{
	public:
		int fd;
		struct sockaddr_in addr;
		Connection(int type,std::string server_addr);
		~Connection(){
			close(this->fd);
		}
		void send();
		
};

#endif
