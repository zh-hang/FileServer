#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include "src/clientConnection.h"
#include <vector>

std::string address="139.9.202.175";

int main(int argc, char* argv[])
{
    if(argc==2)
        address=argv[1];
    ClientConnection conn(TCP_TYPE,address);
    ClientConnection::sendMsg(conn.fd,"hello");
    std::string msg;
    std::vector<std::string>fileList;
    while(1) {
        msg=ClientConnection::recvMsg(conn.fd);
        std::cout<<msg<<std::endl;
        if(msg.substr(0,5)=="file:")
            fileList.push_back(msg.substr(5,msg.size()-5));
        else
            break;
    }
    int port=std::stoi(ClientConnection::recvMsg(conn.fd));
    ClientConnection fileTrans(UDP_TYPE,address,port);
    while(1){
        fileTrans.recvFileUDP(fileList[0]);
    }
    return 0;
}
