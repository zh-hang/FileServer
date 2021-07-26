#include <vector>
#include <string>
#include <iostream>
#include <unistd.h>
#include <dirent.h>
#include <dirent.h>

#include "log.h"

#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

class FileManager{
private:
	std::vector<std::string> files_list;
public:
	FileManager();
	~FileManager(){}
	
	std::vector<std::string> getFilesList(){
		return this->files_list;
	}
	
	std::string getFileName(int pos){
		return this->files_list[pos];
	}
};
#endif