#include <cstddef>
#include <strings.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <string>
#include <vector>
#include <fstream>
#include <map>

#include "src/serverConnection.h"
#include "src/userManager.h"

#include "../lib/log.h"
#include "../lib/fileManager.h"
#include "../lib/threadpool.h"
#include "../lib/typedef.h"


static std::string _user_data_file="user.txt";


void sendFile(int fd,FileManager &fm,ServerConnection &file_connection){
    std::string send_filename(ServerConnection::recvMsg(fd));
    ServerConnection::sendMsg(fd,std::to_string(file_connection.port));
    sockaddr*ra=new sockaddr();
    ServerConnection::recvMsg(file_connection.fd,ra);
    file_connection.sendFileUDP(send_filename, ra);
    delete ra;
}

void recvFile(int fd,FileManager &fm,ServerConnection &file_connection){
    std::string recv_filename(ServerConnection::recvMsg(fd));
    ServerConnection::sendMsg(fd,std::to_string(file_connection.port));
    sockaddr*ra=new sockaddr();
    ServerConnection::recvMsg(file_connection.fd,ra);
    file_connection.recvFileUDP(recv_filename);
    delete ra;
}


void dealConnection(int fd,FileManager &fm,ServerConnection &file_connection){
    static UserManager user_manager(_user_data_file);
    auto user_data= ServerConnection::recvMsg(fd);
    size_t f_pos=user_data.find("%");
    size_t s_pos=user_data.find("%",f_pos+1);
    if(user_manager.login(user_data.substr(f_pos,s_pos-f_pos), user_data.substr(f_pos))){
        ServerConnection::sendMsg(fd,"correct");
        std::vector<std::string> filelist=fm.getFilesList();
        for(auto file:filelist){
            std::cout<<file<<std::endl;
            ServerConnection::sendMsg(fd,"file:"+file);
        }
        CLIENT_CMD cmd;
        ServerConnection::sendMsg(fd,"finish");
        do{
            cmd=std::stoi(ServerConnection::recvMsg(fd));
            switch (cmd) {
                case CLIENT_EOF:
                break;
                case DOWLOAD_FILE:
                sendFile(fd,fm,file_connection);
                break;
                case UPLOAD_FILE:
                recvFile(fd, fm, file_connection);
                break;
                case DELETE_FILE:
                break;
                default:
                cmd=CLIENT_EOF;
                break;
            }
        }while(cmd!=CLIENT_EOF);
    }else{
        ServerConnection::sendMsg(fd,"incorrect");
    }
    close(fd);
}


int main(){
    ThreadPool pool;
	FileManager fm;
    std::cout<<"user data init successfully.\n";
	ServerConnection main_connection(TCP_TYPE);
    std::vector<ServerConnection>file_connections;
    int base_port=10000;
    for(int i=0;i<MAX_NUM;i++){
        file_connections.push_back(ServerConnection(UDP_TYPE,base_port+i));
    }
    int i=0;
	while(true){
		int fd=main_connection.acceptTCP();
        pool.commit(dealConnection, fd,fm,file_connections[i]);
        i=(i+1)%MAX_NUM;
	}
	return 0;
}
