#include "connection.h"

Connection::Connection(int type=TCP_TYPE,int port)
{
	this->type=type;
	bzero(&this->addr, sizeof(this->addr));
	this->addr.sin_family = AF_INET;
	this->addr.sin_addr.s_addr = htonl(INADDR_ANY);
	this->addr.sin_port = htons(port);
	if (this->type == UDP_TYPE)
	{
		this->fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (this->fd < 0)
			writeLog("create socket failed\n");
		if (bind(this->fd, (struct sockaddr *)&this->addr, sizeof(this->addr)) < 0)
			writeLog("bind failed.\n");
	}else if(this->type==TCP_TYPE){
		this->id=socket(AF_INET, SOCK_DGRAM,IPPROTO_UDP);
		if (this->fd < 0)
			writeLog("create socket failed\n");
		if (bind(this->fd, (struct sockaddr *)&this->addr, sizeof(this->addr)) < 0)
			writeLog("bind failed.\n");
		if(listen(this->fd,QUEQUE_SIZE)<0)
			writeLog("listen failed.\n");
	}else{
		writeLog("unknown connection type.\n";
		exit(0);
	}
}

int Connection::accept(){
	struct sockaddr_in client;
	int len = sizeof(client);
	int connfd=accept(this->fd,(struct sockaddr *)&client,&len);
	if(conn<0)
		writeLog("accept connection failed.\n");	
	std::cout <<"connect: "<< inet_ntoa(client.sin_addr)<<":" << ntohs(client.sin_port);
	return connfd;
}

static void Connection::sendMsg(int connfd,const std::string &msg){
	int res=send(connfd,msg.c_str(),msg.length(),0);
	if(res<0){
		struct sockaddr_in sa;
		int len = sizeof(sa);
		getpeername(m_sockfd, (struct sockaddr *)&sa, &len);
		std::string log("send msg failed.\n");
		log+=inet_ntoa(client.sin_addr);
		writeLog(log);
	}
}

static std::string Connection::recvMsg(int connfd){
	char buf[BUFF_SIZE]
	int res=recv(connfd,buf,BUFF_SIZE,0);
	if(res<0){
		struct sockaddr_in sa;
		int len = sizeof(sa);
		getpeername(m_sockfd, (struct sockaddr *)&sa, &len);
		std::string log("receive msg failed.\n");
		log+=inet_ntoa(client.sin_addr);
		writeLog(log);
	}
	return std::string(buf);
}

void Connection::listen()
{
	char buf[BUFF_SIZE];
	while (1)
	{
		bzero(buf, BUFF_SIZE);
		struct sockaddr_in client;
		socklen_t len = sizeof(client);
		int conn=accept(this->fd,(struct sockaddr *)&client,len);
		if(s<0){
			writeLog("accept connection failed.\n");
			exit(0);
		}
		s = recvfrom(this->fd, buf, BUFF_SIZE, 0, (struct sockaddr *)&client, len);
		if (s > 0)
		{
			buf[s] = '\0';
			std::cout << inet_ntoa(client.sin_addr) << ntohs(client.sin_port) << buf;
			this->transFile((struct sockaddr *)&client);
		}
	}
}

void Connection::transFile(struct sockaddr *client)
{
	std::string filename = "server";
	std::ifstream infile(filename, std::ios::in);
	sendto(this->fd, filename.c_str(), filename.length(), 0, client, sizeof(*client));
	std::cout << "start transport file.\n";
	char buf[BUFF_SIZE];
	while (infile)
	{
		bzero(buf, BUFF_SIZE);
		buf[0] = 't';
		infile.read(buf + 1, BUFF_SIZE - 1);
		std::cout<<"send: "<<buf;
		sendto(this->fd, buf, strlen(buf), 0, client, sizeof(*client));
	}
	bzero(buf, BUFF_SIZE);
	buf[0] = 'e';
	sendto(this->fd, buf, strlen(buf), 0, client, sizeof(*client));
	infile.close();
}