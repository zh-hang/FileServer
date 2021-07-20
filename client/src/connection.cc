#include "connection.h"

Connection::Connection(int type, std::string server_addr)
{
	bzero(&this->addr, sizeof(this->addr));
	this->addr.sin_family = AF_INET;
	this->addr.sin_addr.s_addr = inet_addr(server_addr.c_str());
	this->addr.sin_port = htons(SERVER_PORT);
	if (type == UDP_TYPE)
	{
		this->fd = socket(AF_INET, SOCK_DGRAM, IPPROTO_UDP);
		if (this->fd < 0)
		{
			std::cerr << "create socket failed\n";
			exit(0);
		}
	}
}

void Connection::transFile()
{
	char buf[BUFF_SIZE] = "hello\n";
	std::cout << buf;
	sendto(this->fd, buf, BUFF_SIZE, 0, (struct sockaddr *)&this->addr, sizeof(this->addr));
	ssize_t s = recvfrom(fd, buf, sizeof(buf), 0, NULL, NULL);
	if (s > 0)
	{
		std::string filename(buf);
		std::cout << "filename: " << filename << std::endl;
		std::ofstream outfile(filename, std::ios::out);
		while (1)
		{
			bzero(buf,BUFF_SIZE);
			s = recvfrom(fd, buf, sizeof(buf), 0, NULL, NULL);
			if (s > 0 && buf[0] == 't')
				outfile << buf + 1;
			else
				break;
		}
		outfile.close();
	}
}
