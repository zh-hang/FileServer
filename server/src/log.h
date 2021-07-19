#include <fstream>
#include <string>
#include <ctime>
filepath="log.txt"

void writeLog(std::string msg){
	time_t now(time(0));
	std::string time(ctime(&now));
	ofstream f=open(filepath,std::ios::app);
	f.write(time+msg);
	f.close();
}