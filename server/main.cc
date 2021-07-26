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


int main(){
	FileManager fm;
	// ServerConnection main_connection(TCP_TYPE);
    int UDP_Port=23456;
    ServerConnection send_file_connection(UDP_TYPE,UDP_Port);
    sockaddr*sa;
    std::string filename=ServerConnection::recvMsg(send_file_connection.fd,sa);
    std::cout<<filename;
    send_file_connection.sendFileUDP(filename, sa);
	// while(true){
	// 	int fd=main_connection.acceptTCP();
    //     std::cout<<ServerConnection::recvMsg(fd);
    //     std::vector<std::string> filelist=fm.getFilesList();
	// 	for(auto file:filelist){
    //         ServerConnection::sendMsg(fd,"file:"+file);
    //     }
    //     ServerConnection::sendMsg(fd,"finish");
    //     std::string send_filename(ServerConnection::recvMsg(fd));
    //     sockaddr*ra;
    //     socklen_t len;
    //     getpeername(fd, ra, &len);
    //     send_file_connection.sendFileUDP(send_filename, ra);
	// 	close(fd);
	// }
	return 0;
}
