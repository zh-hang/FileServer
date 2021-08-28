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
#include <fcntl.h>

#include "src/server.h"
#include "src/userManager.h"

#include "../lib/log.h"
#include "../lib/fileManager.h"
#include "../lib/threadpool.h"
#include "../lib/typedef.h"


static std::string _user_data_file="user.txt";
static UserManager *user_manager=UserManager::getManager(_user_data_file);
extern ServerOpt server_opt[];

//这里不使用引用的原因时是线程池绑定函数使用的std::bind，默认拷贝，线程池代码还没改改了再换引用
void dealConnection(int fd,FileManager fm,UDPConnection &file_connection){
    auto user_data= Connection::recvMsg(fd);
    size_t f_pos=user_data.find("%");
    size_t s_pos=user_data.find("%",f_pos+1);
    // std::cout<<user_data.substr(f_pos+1,s_pos-f_pos-1)<<user_data.substr(s_pos+1)<<std::endl;
    if(user_manager->login(user_data.substr(f_pos+1,s_pos-f_pos-1), user_data.substr(s_pos+1))){
        Connection::sendMsg(fd,"correct");
        std::vector<std::string> filelist=fm.getFilesList();
        for(auto file:filelist){
            Connection::sendMsg(fd,"file:"+file);
        }
        CLIENT_CMD cmd;
        Connection::sendMsg(fd,"finish");
        std::string msg;
        do{
            msg=Connection::recvMsg(fd);
            std::cout<<msg<<std::endl;
            if(msg.empty())
                cmd=CLIENT_EOF;
            else
                cmd=std::stoi(msg);

            auto *op=server_opt;
            while(op->op){
                if(cmd==op->op){
                    op->opfun(fd,file_connection);
                    break;
                }
                op++;
            }
        }while(cmd!=CLIENT_EOF);
    }else{
        Connection::sendMsg(fd,"incorrect");
    }
    close(fd);
}


int main(){
    ThreadPool pool;
	FileManager fm;
    std::cout<<"user data init successfully.\n";
	TCPConnection main_connection;
    UDPConnection *file_connections[MAX_NUM];
    int base_port=10000;
    for(int i=0;i<MAX_NUM;i++){
        file_connections[i]=new UDPConnection(base_port+i);//不能用拷贝，因为临时对象在销毁的时候会调用析构函数，造成文件描述符的关闭
        //(后附：将析构函数中的close删除，可以使用拷贝)
    }
    int i=0;
    int fd;
    int curr_fd;
    std::thread t1;
	while(true){
		curr_fd=main_connection.TCPAccept();
        int flag=fcntl(curr_fd,F_GETFL, 0);
        if(fcntl(curr_fd, F_SETFL,flag|O_NONBLOCK)<0){ // 设为非阻塞避免客户端连接丢失造成的线程阻
            close(curr_fd);
            continue;
        }
        pool.commit(dealConnection, curr_fd,fm,*file_connections[i]);
	}
    main_connection.closeSelf();
    for(size_t i=0;i<MAX_NUM;i++){
        file_connections[i]->closeSelf();
    }
    delete [] *file_connections;
    *file_connections=nullptr;
	return 0;
}
