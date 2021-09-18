#include "server.h"


static ServerOpt server_opt[] = {
    {DOWLOAD_FILE,sendFile},
    {UPLOAD_FILE,recvFile},
    {DELETE_FILE,delFile},
    {CLIENT_EOF, nullptr},
};

void sendFile(int fd,UDPConnection &file_connection){
    std::string send_filename(Connection::recvMsg(fd));
    Connection::sendMsg(fd,std::to_string(file_connection.getPort()));
    sockaddr*ra=new sockaddr();
    std::cout<<Connection::recvMsg(file_connection.getFd(),ra);
    file_connection.sendFile(send_filename, ra);
    delete ra;
}

void recvFile(int fd,UDPConnection &file_connection){
    std::string recv_filename(Connection::recvMsg(fd));
    Connection::sendMsg(fd,std::to_string(file_connection.getPort()));
    sockaddr*ra=new sockaddr();
    Connection::recvMsg(file_connection.getFd(),ra);
    file_connection.recvFile(recv_filename);
    delete ra;
}

void delFile(int fd,UDPConnection &file_connection){
	std::string del_filename(Connection::recvMsg(fd));
	int res=remove(del_filename.c_str());
	Connection::sendMsg(fd,res?"success":"fail");	
}

