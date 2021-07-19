#include <iostream>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <string.h>
#include "src/connection.h"

int main(int argc, char* argv[])
{
    Connection conn(UDP_TYPE,"127.0.0.1");
    conn.send();
    return 0;
}
