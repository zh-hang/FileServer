#include "connection.h"
#include <cstddef>
#include <netinet/in.h>

Connection::Connection(int type)
{
	this->type=type;
	if (this->type == UDP_TYPE)
	{
		this->fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (this->fd < 0){
			writeLog("create socket failed\n");
			exit(0);
		}
        std::cout<<"create socket succussfully.\n";
	}else if(this->type==TCP_TYPE){
		this->fd=socket(AF_INET, SOCK_STREAM,IPPROTO_TCP);
		if (this->fd < 0){
			writeLog("create socket failed\n");
			exit(0);
		}
        std::cout<<"create socket succussfully.\n";
		bzero(&this->addr, sizeof(this->addr));
		this->addr.sin_family = AF_INET;
		this->addr.sin_addr.s_addr = htonl(INADDR_ANY);
		this->addr.sin_port = htons(TCP_PORT);
	}else{
		writeLog("unknown connection type.\n");
		exit(0);
	}
}


void Connection::sendFileUDP(std::string filename,sockaddr *ra){
	std::ifstream infile(filename, std::ios::in);
	std::cout << "start transport file.\n";
	char buf[BUFF_SIZE];
	bzero(buf, BUFF_SIZE);
	buf[0] = 's';
	int res=sendto(this->fd, buf, BUFF_SIZE, 0, ra, sizeof(*ra));
	while (infile)
	{
		bzero(buf, BUFF_SIZE);
		buf[0] = 't';
		infile.read(buf + 1, BUFF_SIZE - 1);
		std::cout<<"send: "<<buf<<std::endl;
		sendto(this->fd, buf, BUFF_SIZE, 0, ra, sizeof(*ra));
	}
	bzero(buf, BUFF_SIZE);
	buf[0] = 'e';
	sendto(this->fd, buf, BUFF_SIZE, 0, ra, sizeof(*ra));
	infile.close();
    std::cout<<inet_ntoa(((struct sockaddr_in*)ra)->sin_addr)<<": finish transport file.\n";
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
    std::cout<<"finish receive file.\n";
}

void Connection::sendMsg(int connfd,const std::string &msg){
	int res=send(connfd,msg.c_str(),BUFF_SIZE,0);
	if(res<0){
		struct sockaddr_in ra;
		socklen_t len = sizeof(ra);
		getpeername(connfd, (struct sockaddr *)&ra, &len);
		std::string log("send msg failed.\n");
		log+=inet_ntoa(ra.sin_addr);
		writeLog(log);
	}
}

std::string Connection::recvMsg(int connfd){
	char buf[BUFF_SIZE];
	int res=recv(connfd,buf,BUFF_SIZE,0);
	if(res<0){
		struct sockaddr_in sa;
		socklen_t len = sizeof(sa);
		getpeername(connfd, (struct sockaddr *)&sa, &len);
		std::string log("receive msg failed.\n");
		log+=inet_ntoa(sa.sin_addr);
		writeLog(log);
	}
	return std::string(buf);
}


void Connection::sendMsg(int connfd,const std::string &msg,const sockaddr*ra){
    int res=sendto(connfd, msg.c_str(), BUFF_SIZE, 0, ra, sizeof(*ra));
	if(res<0){
		std::string log("send msg failed.\n");
		log+=inet_ntoa(((sockaddr_in*)ra)->sin_addr);
		writeLog(log);
	}
}

std::string Connection::recvMsg(int connfd,sockaddr*sa){
    char buf[BUFF_SIZE];
    socklen_t len=sizeof(*sa);
	int res=recvfrom(connfd,buf,BUFF_SIZE,0,sa,&len);
	if(res<0){
		std::string log("receive msg failed.\n");
		log+=inet_ntoa(((sockaddr_in*)sa)->sin_addr);
		writeLog(log);
	}
	return std::string(buf);
}
