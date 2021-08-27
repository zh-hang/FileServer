#ifndef CLIENT_H
#define CLIENT_H
#include <asm-generic/errno-base.h>
#include <asm-generic/errno.h>
#include <cerrno>
#include <cstddef>
#include <fstream>
#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include <string>
#include <unistd.h>
#include <utility>
#include <vector>

#include "../../lib/typedef.h"
#include "../../lib/connection.h"

void showCommand();
void printFileList(std::vector<std::string>&fileList);
void recvFile(int fd,std::string &address,std::vector<std::string>&fileList);
void sendFile(int fd,std::string &address,std::vector<std::string>&fileList);
void delFile(int fd,std::vector<std::string>&fileList);

#endif