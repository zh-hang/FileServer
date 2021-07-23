#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include "src/clientConnection.h"


std::string address="139.9.202.175";

int main(int argc, char* argv[])
{
    if(argc==2)
        address=argv[1];
    ClientConnection conn(TCP_TYPE,address);
    ClientConnection::sendMsg(conn.fd,"hello");
    std::cout<<ClientConnection::recvMsg(conn.fd);
    return 0;
}
