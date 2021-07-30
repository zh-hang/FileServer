#include <cstddef>
#include <fstream>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <vector>

#include "src/clientConnection.h"

#include "../lib/typedef.h"

void showCommand(){
    std::cout<<CLIENT_EOF<<":quit"<<std::endl;
    std::cout<<DOWLOAD_FILE<<":download"<<std::endl;
    std::cout<<UPLOAD_FILE<<":upload"<<std::endl;
}

void printFileList(std::vector<std::string>&fileList){
    for(size_t i =0;i<fileList.size();i++){
        std::cout<<i<<":"<<fileList[i]<<std::endl;
    }
}

void recvFile(int fd,std::string &address,std::vector<std::string>&fileList){
    printFileList(fileList);
    std::cout<<"select witch file you want to download.\n";
    int i;
    std::cin>>i;
    if(i<fileList.size()){
        ClientConnection::sendMsg(fd,fileList[i]);
        int port =std::stoi(ClientConnection::recvMsg(fd));
        ClientConnection fileTrans(UDP_TYPE,address,port);
        std::cout<<"start recv file: "<<fileList[i]<<std::endl;
        ClientConnection::sendMsg(fileTrans.fd,"ready",(sockaddr*)&fileTrans.addr);
        fileTrans.recvFileUDP(fileList[i]);
    }else{
        std::cout<<"there is no such file.\n";
    }
}

void sendFile(int fd,std::string &address,std::vector<std::string>&fileList){
    std::string filename;
    std::cin>>filename;
    std::string clear_filename;
    for(size_t c=0;c<filename.length();c++){
        if(filename[c]=='.'||filename[c]=='/')
            continue;
        clear_filename=filename.substr(c);
    }
    for(auto file:fileList){
        if (clear_filename==file) {
            std::cout<<"file exist in server.\n";
            return ;
        }
    }
    std::ifstream infile(filename);
    if(!infile){
        std::cout<<"file not exitst.\n";
        infile.close();
        return ;
    }
    infile.close();
    ClientConnection::sendMsg(fd,clear_filename);
    int port =std::stoi(ClientConnection::recvMsg(fd));
    ClientConnection fileTrans(UDP_TYPE,address,port);
    ClientConnection::sendMsg(fileTrans.fd,"ready",(sockaddr*)&fileTrans.addr);
    fileTrans.sendFileUDP(filename,(sockaddr*)&fileTrans.addr);
}

int main(int argc, char* argv[])
{
    std::string address;
    if(argc==2)
        address=argv[1];
    else{
        std::cout<<"please input with format: client host\nexample: client 127.0.0.1\n";
        return 0;
    }
    ClientConnection conn(TCP_TYPE,address);
    std::string msg;
    ClientConnection::sendMsg(conn.fd,msg);
    std::vector<std::string>fileList;
    std::string user_name,user_password;
    std::cout<<"please input your user name and password seperated by space.\n";
    std::cin>>user_name>>user_password;
    ClientConnection::sendMsg(conn.fd,"%"+user_name+"%"+user_password);
    if(ClientConnection::recvMsg(conn.fd)!="correct"){
        std::cout<<"your user name and password is incorrect."<<std::endl;
        return 0;
    }
    while(1) {
        msg=ClientConnection::recvMsg(conn.fd);
        if(msg.compare(0,5,"file:")==0)
            fileList.push_back(msg.substr(5));
        else
            break;
    }
    int cmd;
    do {
        showCommand();
        std::cin>>cmd;
        switch (cmd) {
            case CLIENT_EOF:
            break;
            case DOWLOAD_FILE:
            recvFile(conn.fd, address, fileList);
            break;
            case UPLOAD_FILE:
            sendFile(conn.fd, address, fileList);
            break;
            case DELETE_FILE:
            break;
            default:
            std::cout<<"unrecognized command.\n";
            break;
        }
    }while (cmd!=CLIENT_EOF);
    std::cout<<"exit.\n";
    
    return 0;
}
