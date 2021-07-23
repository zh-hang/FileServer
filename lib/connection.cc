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
	}else if(this->type==TCP_TYPE){
		this->id=socket(AF_INET, SOCK_DGRAM,IPPROTO_UDP);
	}else{
		writeLog("unknown connection type.\n";
		exit(0);
	}
	if (this->fd < 0){
		writeLog("create socket failed\n");
		exit(0);
	}
	if (bind(this->fd, (struct sockaddr *)&this->addr, sizeof(this->addr)) < 0){
		writeLog("bind failed.\n");
		exit(0);
	}
}


void Connection::sendFileUDP(std::string filename,sockaddr *ra){
	std::ifstream infile(filename, std::ios::in);
	std::cout << "start transport file.\n";
	char buf[BUFF_SIZE];
	bzero(buf, BUFF_SIZE);
	buf[0] = 's';
	sendto(this->fd, buf, strlen(buf), 0, ra, sizeof(*ra));
	while (infile)
	{
		bzero(buf, BUFF_SIZE);
		buf[0] = 't';
		infile.read(buf + 1, BUFF_SIZE - 1);
		std::cout<<"send: "<<buf;
		sendto(this->fd, buf, strlen(buf), 0, ra, sizeof(*ra));
	}
	bzero(buf, BUFF_SIZE);
	buf[0] = 'e';
	sendto(this->fd, buf, strlen(buf), 0, ra, sizeof(*ra));
	infile.close();
}


void Connection::recvFileUDP(std::string filename){
	char buf[BUFF_SIZE];
	std::ofstream outfile(filename, std::ios::out);
	bzero(buf,BUFF_SIZE);
	ssize_t s;
	
	do{
		s = recvfrom(fd, buf, sizeof(buf), 0, NULL, NULL);
	}while(buf[0]!='s');
	
	do{
		s = recvfrom(fd, buf, sizeof(buf), 0, NULL, NULL);
		if (s > 0 && buf[0] == 't')
			outfile.write(buf + 1,BUFF_SIZE-1);
		else
			break;
	}while(1);
	outfile.close();
}

static void Connection::sendMsg(int connfd,const std::string &msg){
	int res=send(connfd,msg.c_str(),msg.length(),0);
	if(res<0){
		struct sockaddr_in sa;
		int len = sizeof(sa);
		getpeername(m_sockfd, (struct sockaddr *)&sa, &len);
		std::string log("send msg failed.\n");
		log+=inet_ntoa(sa.sin_addr);
		writeLog(log);
	}
}

static std::string Connection::recvMsg(int connfd){
	char buf[BUFF_SIZE]
	int res=recv(connfd,buf,BUFF_SIZE,0);
	if(res<0){
		struct sockaddr_in ra;
		int len = sizeof(ra);
		getpeername(m_sockfd, (struct sockaddr *)&ra, &len);
		std::string log("receive msg failed.\n");
		log+=inet_ntoa(ra.sin_addr);
		writeLog(log);
	}
	return std::string(buf);
}

