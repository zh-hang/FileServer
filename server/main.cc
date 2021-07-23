#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>

#include "src/serverConnection.h"

#include "../lib/log.h"
#include "../lib/fileManager.h"


int main(){
	FileManager fm;
	ServerConnection main_connection(TCP_TYPE);
	while(true){
		int fd=main_connection.acceptTCP();
        std::cout<<Connection::recvMsg(fd);
		Connection::sendMsg(fd,"have received.\n");
		close(fd);
	}
	return 0;
}
