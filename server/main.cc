#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <string>
#include <vector>

#include "src/serverConnection.h"

#include "../lib/log.h"
#include "../lib/fileManager.h"
#include "../lib/threadpool.h"

#define MAX_UDP 16

void dealConnection(int fd,FileManager &fm,ServerConnection &file_connection){
    std::cout<<ServerConnection::recvMsg(fd);
    std::vector<std::string> filelist=fm.getFilesList();
    for(auto file:filelist){
        ServerConnection::sendMsg(fd,"file:"+file);
    }
    ServerConnection::sendMsg(fd,"finish");
    std::string send_filename(ServerConnection::recvMsg(fd));
    ServerConnection::sendMsg(fd,std::to_string(file_connection.port));
    sockaddr*ra=new sockaddr();
    ServerConnection::recvMsg(file_connection.fd,ra);
    file_connection.sendFileUDP(send_filename, ra);
    delete ra;
    close(fd);
}

int main(){
    ThreadPool pool;
	FileManager fm;
	ServerConnection main_connection(TCP_TYPE);
    std::vector<ServerConnection>file_connections;
    int base_port=10000;
    for(int i=0;i<MAX_NUM;i++){
        file_connections.push_back(ServerConnection(UDP_TYPE,base_port+i));
    }
    int i=0;
	while(true){
		int fd=main_connection.acceptTCP();
        pool.commit(dealConnection, fd,fm,file_connections[i]);
        i=(i+1)%MAX_NUM;
	}
	return 0;
}
