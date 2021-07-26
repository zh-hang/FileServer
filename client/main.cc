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
    std::string msg;
    std::vector<std::string>fileList;
    while(1) {
        msg=ClientConnection::recvMsg(conn.fd);
        if(msg.compare(0,5,"file:")==0){
            std::cout<<"receive: "<<msg.substr(5)<<std::endl;
            fileList.push_back(msg.substr(5));
        }
        else
            break;
    }
    ClientConnection::sendMsg(conn.fd,fileList[0]);
    int port =std::stoi(ClientConnection::recvMsg(conn.fd));
    ClientConnection fileTrans(UDP_TYPE,address,port);    
    std::cout<<"start recv file\n";
    ClientConnection::sendMsg(fileTrans.fd,"ready",(sockaddr*)&fileTrans.addr);
    fileTrans.recvFileUDP(fileList[0]);
    
    return 0;
}
