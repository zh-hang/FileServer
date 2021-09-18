#include <cstddef>
#include <strings.h>
#include <sys/types.h>
#include <sys/fcntl.h>
#include <sys/socket.h>
#include <sys/epoll.h>
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

static std::string _user_data_file = "user.txt";
static UserManager::UserManagerPtr user_manager = UserManager::getManager(_user_data_file);

extern ServerOpt server_opt[];
FileManager::FileManagerPtr fm = FileManager::getFileManager();

void login(EpollControllor &controllor, int fd)
{
    auto user_data = Connection::recvMsg(fd);
    size_t f_pos = user_data.find("%");
    size_t s_pos = user_data.find("%", f_pos + 1);
    // std::cout<<user_data.substr(f_pos+1,s_pos-f_pos-1)<<user_data.substr(s_pos+1)<<std::endl;
    if (user_manager->login(user_data.substr(f_pos + 1, s_pos - f_pos - 1), user_data.substr(s_pos + 1)))
    {
        Connection::sendMsg(fd, "correct");
        std::vector<std::string> filelist = fm->getFilesList();
        for (auto file : filelist)
        {
            Connection::sendMsg(fd, "file:" + file);
        }
        Connection::sendMsg(fd, "finish");
        controllor.addFd(fd);
    }
    else
    {
        Connection::sendMsg(fd, "incorrect");
        close(fd);
    }
}

void distributor(EpollControllor &controllor, ThreadPool &pool)
{
    std::vector<UDPConnection> file_connections;
    int base_port = 10000;
    for (int i = 0; i < MAX_NUM; i++)
    {
        file_connections.emplace_back(UDPConnection(base_port + i)); //不能用拷贝，因为临时对象在销毁的时候会调用析构函数，造成文件描述符的关闭
        //(后附：将析构函数中的close删除，可以使用拷贝)
    }
    epoll_event *events;
    int i = 0;
    while (epoll_wait(controllor.fd_sets, events, 1, -1))
    {
        CLIENT_CMD cmd;
        std::string msg;
        msg = Connection::recvMsg(events->events);
        std::cout << msg << std::endl;
        if (msg.empty())
            cmd = CLIENT_EOF;
        else
            cmd = std::stoi(msg);
        auto *op = server_opt;
        while (op->op)
        {
            if (cmd == op->op)
            {
                if (op->opfun == nullptr)
                    controllor.delFd(events->events);
                else
                {
                    pool.commit(op->opfun, events->events, file_connections[i]);
                    i = (i + 1) % MAX_NUM;
                }
                break;
            }
            op++;
        }
    }
}

int main()
{
    ThreadPool pool;
    std::cout << "user data init successfully.\n";
    TCPConnection main_connection;
    int i = 0;
    int fd;
    int curr_fd;
    EpollControllor controllor;
    std::thread distributor_thread(distributor, controllor, pool);
    while (true)
    {
        curr_fd = main_connection.TCPAccept();
        int flag = fcntl(curr_fd, F_GETFL, 0);
        if (fcntl(curr_fd, F_SETFL, flag | O_NONBLOCK) < 0)
        { // 设为非阻塞避免客户端连接丢失造成的线程阻塞
            close(curr_fd);
            continue;
        }
        pool.commit(login, controllor, fd);
    }
    main_connection.closeSelf();
    distributor_thread.join();
    return 0;
}
