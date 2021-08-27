#include "client.h"

void showCommand(){
    std::cout<<CLIENT_EOF<<":quit"<<std::endl;
    std::cout<<DOWLOAD_FILE<<":download"<<std::endl;
    std::cout<<UPLOAD_FILE<<":upload"<<std::endl;
    std::cout<<DELETE_FILE<<":upload"<<std::endl;
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
        Connection::sendMsg(fd,fileList[i]);
        int port =std::stoi(Connection::recvMsg(fd));
        UDPConnection fileTrans(address.c_str(),port);
        Connection::sendMsg(fileTrans.getFd(),"ready",(sockaddr*)&fileTrans.addr);
        std::cout<<"start recv file: "<<fileList[i]<<std::endl;
        fileTrans.recvFile(fileList[i]);
        fileTrans.closeSelf();
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
        break;
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
    Connection::sendMsg(fd,clear_filename);
    int port =std::stoi(Connection::recvMsg(fd));
    UDPConnection fileTrans(address.c_str(),port);
    Connection::sendMsg(fileTrans.getFd(),"ready",(sockaddr*)&fileTrans.addr);
    fileTrans.sendFile(filename,(sockaddr*)&fileTrans.addr);
    fileTrans.closeSelf();
}

void delFile(int fd,std::vector<std::string>&fileList){
    printFileList(fileList);
    std::cout<<"select witch file you want to delete.\n";
    int i;
    std::cin>>i;
    if(i<fileList.size()){
        Connection::sendMsg(fd,fileList[i]);
        std::cout<<Connection::recvMsg(fd);
    }else{
        std::cout<<"there is no such file.\n";
    }
}