#include "connection.h"
#include <bits/types/struct_timeval.h>
#include <cstddef>
#include <netinet/in.h>
#include <strings.h>
#include <sys/select.h>

int Connection::sendMsg(int connfd, const std::string &msg) {
    fd_set fds;
    FD_SET(connfd, &fds);
    timeval timeout{6, 0};
    if (trySelect(nullptr, &fds, nullptr, timeout) &&
        send(connfd, msg.c_str(), BUFF_SIZE, 0))
        return 0;
    struct sockaddr_in ra;
    socklen_t len = sizeof(ra);
    getpeername(connfd, (struct sockaddr *)&ra, &len);
    std::string log("send msg failed.\n");
    log += inet_ntoa(ra.sin_addr);
    writeLog(log + "\n");
    return -1;
}

std::string Connection::recvMsg(int connfd) {
    char buf[BUFF_SIZE];
    bzero(buf, BUFF_SIZE);
    fd_set fds;
    FD_SET(connfd, &fds);
    timeval timeout{6, 0};
    if (trySelect(&fds, nullptr, nullptr, timeout) &&
        recv(connfd, buf, BUFF_SIZE, 0))
        return std::string(buf);
    struct sockaddr_in sa;
    socklen_t len = sizeof(sa);
    getpeername(connfd, (struct sockaddr *)&sa, &len);
    std::string log("receive msg failed.\n");
    log += inet_ntoa(sa.sin_addr);
    writeLog(log + "\n");
    return NULL;
}

int Connection::sendMsg(int connfd, const std::string &msg,
                         const sockaddr *ra) {
    int res = sendto(connfd, msg.c_str(), BUFF_SIZE, 0, ra, sizeof(*ra));
    if (res < 0) {
        std::string log("send msg failed.\n");
        log += inet_ntoa(((sockaddr_in *)ra)->sin_addr);
        writeLog(log + "\n");
        return -1;
    }
    return 0;
}

std::string Connection::recvMsg(int connfd, sockaddr *sa) {
    char buf[BUFF_SIZE];
    socklen_t len = sizeof(*sa);
    int res = recvfrom(connfd, buf, BUFF_SIZE, 0, sa, &len);
    if (res < 0) {
        std::string log("receive msg failed.\n");
        log += inet_ntoa(((sockaddr_in *)sa)->sin_addr);
        writeLog(log + "\n");
    }
    return std::string(buf);
}

int Connection::trySelect(fd_set *readfds, fd_set *writefds, fd_set *exceptfds,
                          timeval &timeout) {
    int res = select(FD_SETSIZE, readfds, writefds, exceptfds, &timeout);
    switch (res) {
    case -1:
        errnoDeal();
    case 0:
        return 0;
    default:
        return res;
    }
}

void Connection::errnoDeal() {
    switch (errno) {
    case EBADF:
        writeLog("sock不是有效的描述词\n");
        break;
    case ECONNREFUSED:
        writeLog("远程主机阻绝网络连接\n");
        break;
    case EFAULT:
        writeLog("内存空间访问出错\n");
        break;
    case EINTR:
        writeLog("操作被信号中断\n");
        break;
    case EINVAL:
        writeLog("参数无效\n");
        break;
    case ENOMEM:
        writeLog("内存不足\n");
        break;
    case ENOTCONN:
        writeLog("与面向连接关联的套接字尚未被连接上\n");
        break;
    case ENOTSOCK:
        writeLog("sock索引的不是套接字\n");
        break;
    }
}

void TCPConnection::TCPListen() {
    if (listen(this->fd, QUEQUE_SIZE) < 0) {
        writeLog("listen failed.\n");
        exit(0);
    }
    std::cout << "listen successfully.\n";
}

int TCPConnection::TCPAccept(){
    std::cout<<"start accept.\n";
	int connfd=accept(this->fd,0,0);
	if(connfd<0)
		writeLog("accept failed.\n");
	return connfd;
}

void UDPConnection::sendFile(std::string filename, sockaddr *ra) {
    std::ifstream infile(filename, std::ios::in);
    std::cout << "start transport file： " << filename << std::endl;
    char buf[BUFF_SIZE];
    fd_set readfds;
    fd_set writefds;
    bzero(buf, BUFF_SIZE);
    buf[0] = 's';
    sendto(this->fd, buf, BUFF_SIZE, 0, ra, sizeof(*ra));
    while (infile) {
        bzero(buf, BUFF_SIZE);
        buf[0] = 't';
        infile.read(buf + 1, BUFF_SIZE - 1);
        sendto(this->fd, buf, BUFF_SIZE, 0, ra, sizeof(*ra));
    }
    bzero(buf, BUFF_SIZE);
    buf[0] = 'e';
    sendto(this->fd, buf, BUFF_SIZE, 0, ra, sizeof(*ra));
    infile.close();
    std::cout << inet_ntoa(((struct sockaddr_in *)ra)->sin_addr)
                << ": finish transport file.\n";
}

void UDPConnection::recvFile(std::string filename) {
    char buf[BUFF_SIZE];
    std::ofstream outfile(filename, std::ios::out);
    bzero(buf, BUFF_SIZE);
    ssize_t s;

    do {
        s = recvfrom(fd, buf, sizeof(buf), 0, NULL, NULL);
    } while (buf[0] != 's');

    do {
        s = recvfrom(fd, buf, sizeof(buf), 0, NULL, NULL);
        if (s > 0 && buf[0] == 't')
        outfile.write(buf + 1, BUFF_SIZE - 1);
        else
        break;
    } while (1);
    outfile.close();
    std::cout << "finish receive file.\n";
}
