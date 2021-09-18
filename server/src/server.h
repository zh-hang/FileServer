#ifndef SERVER_H
#define SERVER_H

#include <sys/epoll.h>
#include "../../lib/typedef.h"
#include "../../lib/connection.h"
#include "../../lib/spinLock.h"
#include "../../lib/threadpool.h"

#define MAX_CONNECTION 10000


struct ServerOpt {
    int op;
    void (*opfun)(int fd,UDPConnection &udp);
};

void sendFile(int fd,UDPConnection &file_connection);
void recvFile(int fd,UDPConnection &file_connection);
void delFile(int fd,UDPConnection &file_connection);

class EpollControllor{
    SpinLock lock;
    public:
    int fd_sets;
    EpollControllor(){
        fd_sets=epoll_create(MAX_CONNECTION);
    }
    ~EpollControllor(){
        close(fd_sets);
    }
    void addFd(int fd){
        this->lock.lock();
        epoll_ctl(this->fd_sets,EPOLL_CTL_ADD,fd,nullptr);
        this->lock.unlock();
    }
    void delFd(int fd){
        this->lock.lock();
        epoll_ctl(this->fd_sets,EPOLL_CTL_DEL,fd,nullptr);
        this->lock.unlock();
    }
};

#endif