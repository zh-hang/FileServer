#include "connection.h"

Connection::Connection(int type=TCP_TYPE,int port)
{
	if(listen(this->fd,QUEQUE_SIZE)<0)
		writeLog("listen failed.\n");0);
}

void Connectoin::accept(){
	
}