#include "log.h"

static std::string filepath="log.txt";

void writeLog(std::string msg){
	time_t now(time(0));
	std::string time(ctime(&now));
	std::ofstream f(filepath,std::ios::app);
	f<<time+msg;
	f.close();
}
