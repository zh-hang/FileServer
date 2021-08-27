
#include "src/client.h"

#define DEBUG_TIME

int main(int argc, char* argv[])
{
    std::string address;
    if(argc==2)
        address=argv[1];
    else{
        std::cout<<"please input with format: client host\nexample: client 127.0.0.1\n";
        return 0;
    }
    TCPConnection conn(address.c_str());
    std::string msg;
    std::vector<std::string>fileList;
    std::string user_name,user_password;
    std::cout<<"please input your user name and password seperated by space.\n";

#ifndef DEBUG_TIME
    std::cin>>user_name>>user_password;
#else
    user_name="admin";
    user_password="123456";
#endif

    msg="%"+user_name+"%"+user_password;
    Connection::sendMsg(conn.getFd(),msg);
    std::cout<<msg<<std::endl;
    msg=Connection::recvMsg(conn.getFd());

#ifdef DEBUG_TIME
    std::cout<<msg<<std::endl;
#endif

    if(msg!="correct"){
        std::cout<<"your user name and password is incorrect."<<std::endl;
        return 0;
    }
    while(1) {
        msg=Connection::recvMsg(conn.getFd());
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
            Connection::sendMsg(conn.getFd(),std::to_string(cmd));
            recvFile(conn.getFd(), address, fileList);
            break;
            case UPLOAD_FILE:
            Connection::sendMsg(conn.getFd(),std::to_string(cmd));
            sendFile(conn.getFd(), address, fileList);
            break;
            case DELETE_FILE:
            Connection::sendMsg(conn.getFd(),std::to_string(cmd));
            delFile(conn.getFd(),fileList);
            break;
            default:
            std::cout<<"unrecognized command.\n";
            break;
        }
    }while (cmd!=CLIENT_EOF);
    std::cout<<"exit.\n";
    conn.closeSelf();
    return 0;
}
