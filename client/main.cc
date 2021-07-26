#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include "src/clientConnection.h"
#include <vector>

#define PORT 41414

std::string address="139.9.202.175";

int main(int argc, char* argv[])
{
    if(argc==2)
        address=argv[1];
    // ClientConnection conn(TCP_TYPE,address);
    // ClientConnection::sendMsg(conn.fd,"hello");
    // std::string msg;
    // std::vector<std::string>fileList;
    // while(1) {
    //     msg=ClientConnection::recvMsg(conn.fd);
    //     if(msg.compare(0,5,"file:")==0){
    //         std::cout<<"receive: "<<msg.substr(5)<<std::endl;
    //         fileList.push_back(msg.substr(5));
    //     }
    //     else
    //         break;
    // }
    // ClientConnection::sendMsg(conn.fd,fileList[0]);
    ClientConnection fileTrans(UDP_TYPE,address,23456);
    ClientConnection::sendMsg(fileTrans.fd,"log.txt",(sockaddr*)&fileTrans.addr);
    std::cout<<"start recv file\n";
    while(1){
        fileTrans.recvFileUDP("log.txt");
    }
    return 0;
}
