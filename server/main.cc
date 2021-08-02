#include <cstddef>
#include <strings.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <string.h>
#include <string>
#include <thread>
#include <vector>
#include <fstream>
#include <map>
#include <sys/syscall.h> 
#include <cstdio>

#include "src/serverConnection.h"
#include "src/userManager.h"

#include "../lib/log.h"
#include "../lib/fileManager.h"
#include "../lib/threadpool.h"
#include "../lib/typedef.h"


static std::string _user_data_file="user.txt";
static UserManager user_manager(_user_data_file);

void debug_errno(){
    int id = syscall(SYS_gettid);
    std::cout<<id;
    switch (errno) {
        case EBADF:std::cout<<"sock不是有效的描述词\n";break;
        case ECONNREFUSED:std::cout<<"远程主机阻绝网络连接\n";break;
        case EFAULT:std::cout<<"内存空间访问出错\n";break;
        case EINTR:std::cout<<"操作被信号中断\n";break;
        case EINVAL:std::cout<<"参数无效\n";break;
        case ENOMEM:std::cout<<"内存不足\n";break;
        case ENOTCONN:std::cout<<"与面向连接关联的套接字尚未被连接上\n";break;
        case ENOTSOCK:std::cout<<"sock索引的不是套接字\n";break;
    }
}

void sendFile(int fd,ServerConnection &file_connection){
    std::string send_filename(ServerConnection::recvMsg(fd));
    ServerConnection::sendMsg(fd,std::to_string(file_connection.port));
    sockaddr*ra=new sockaddr();
    std::cout<<ServerConnection::recvMsg(file_connection.fd,ra);
    file_connection.sendFileUDP(send_filename, ra);
    delete ra;
}

void recvFile(int fd,ServerConnection &file_connection){
    std::string recv_filename(ServerConnection::recvMsg(fd));
    ServerConnection::sendMsg(fd,std::to_string(file_connection.port));
    sockaddr*ra=new sockaddr();
    ServerConnection::recvMsg(file_connection.fd,ra);
    file_connection.recvFileUDP(recv_filename);
    delete ra;
}

void delFile(int fd,ServerConnection &file_connection){
	std::string del_filename(ServerConnection:recvMsg(fd));
	int res=remove(del_filename.c_str());
	ServerConnection::sendMsg(fd,res?"success":"fail"));	
}

//这里不使用引用的原因时是线程池绑定函数使用的std::bind，默认拷贝，线程池代码还没改改了再换引用
void dealConnection(int fd,FileManager fm,ServerConnection &file_connection){
    auto user_data= ServerConnection::recvMsg(fd);
    size_t f_pos=user_data.find("%");
    size_t s_pos=user_data.find("%",f_pos+1);
    std::cout<<user_data.substr(f_pos+1,s_pos-f_pos-1)<<user_data.substr(s_pos+1)<<std::endl;
    if(user_manager.login(user_data.substr(f_pos+1,s_pos-f_pos-1), user_data.substr(s_pos+1))){
        ServerConnection::sendMsg(fd,"correct");
        std::vector<std::string> filelist=fm.getFilesList();
        for(auto file:filelist){
            ServerConnection::sendMsg(fd,"file:"+file);
        }
        CLIENT_CMD cmd;
        ServerConnection::sendMsg(fd,"finish");
        std::string msg;
        do{
            msg=ServerConnection::recvMsg(fd);
            std::cout<<msg<<std::endl;
            if(msg.empty())
                cmd=0;
            else
                cmd=std::stoi(msg);
            switch (cmd) {
                case CLIENT_EOF:
                break;
                case DOWLOAD_FILE:
                sendFile(fd,file_connection);
                break;
                case UPLOAD_FILE:
                recvFile(fd, file_connection);
                break;
                case DELETE_FILE:
                delFile(fd,file_connnection);
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
    ServerConnection *file_connections[MAX_NUM];
    int base_port=10000;
    for(int i=0;i<MAX_NUM;i++){
        file_connections[i]=new ServerConnection(UDP_TYPE,base_port+i);//不能用拷贝，因为临时对象在销毁的时候会调用析构函数，造成文件描述符的关闭
        //(后附：将析构函数中的close删除，可以使用拷贝)
    }
    int i=0;
    int fd;
    int fds[MAX_NUM];
    std::thread t1;
	while(true){
		fds[i]=main_connection.acceptTCP();
        pool.commit(dealConnection, fds[i],fm,*file_connections[i]);
        i=(i+1)%MAX_NUM;
	}
    main_connection.close_self();
    for(size_t i=0;i<MAX_NUM;i++){
        file_connections[i]->close_self();
        delete file_connection[i];
        file_connection[i]=nullptr;
    }
	return 0;
}
