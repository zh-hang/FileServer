#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include "src/log.h"
#include "src/connection.h"

int main(){
	Connection server(UDP_TYPE);
	server.listen();
	return 0;
}
