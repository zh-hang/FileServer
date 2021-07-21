#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include "src/log.h"
#include "src/connection.h"

#define UDP_PORT 12345
#define TCP_PORT 23456

int main(){
	Connection server(TCP_TYPE,TCP_PORT);
	server.listen();
	return 0;
}
