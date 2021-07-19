#include <sys/types/h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include "log.h"


#define SERVER_PORT 12345
#define BUF_SIZE 4096
#define QUEUE_SIZE 10

void haddelUDPMsg(int serverfd){
	char buf[BUF_SIZE];
	socket_t len;
	int count;
	struct sockadd_in client_addr;
	while(1){
		bzero(buf,BUF_SIZE);
		count=recvfrom(serverfd,buf,BUFF_LEN,0,(struct sockaddr*)&client_addr,&len);
		if(count==-1){
			writeLog("recieve data failed.\n");
			return ;
		}
		writeLog(buf);
		sendto(serverfd,buf,BUFF_LEN,0,(struct sockaddr*)&client_addr,len);
	}
}


int main(){
	int s,b,l,fd,sa,bytes,on(1);
	struct sockadd_in channel;
	
	bzero(&channel,sizeof(channel))
	channel.sin_faminly=AF_INET;
	channel.sin_addr.s_addr=htonl(INADDY_ANY);
	channel.sin_port=htons(SERVER_PORT);
	
	s=socket(AF_INET,SOCK_DGRAM,IPPROTO_UDP);
	if(s<0)
		logWrite("create socket failed\n");
	b=bind(s,(struct sockaddr*)&channel,sizeof(channel);
	if(b<0)
		logWrite("bind failed\n");
	handleUDPMsg(s);
	
	close(channel);
	return 0;
}