#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include "src/log.h"
#include "src/connection.h"
#include "src/fileManager.h"

#define UDP_PORT 12345
#define TCP_PORT 23456

int main(){
	FileManager fm();
	ServerConnection main_connection();
	while(true){
		int fd=main_connection.accept();
		Connection::sendMsg(fd,"have received.\n");
		close(fd);
	}
	return 0;
}
