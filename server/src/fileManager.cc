#include "fileManage.h"

FileManager::FileManager(){
	std::string file_path(getcwd(NULL,0));
	if(file_path.length()==0){
		writeLog("get file path failed.\n");
		exit(0);
	}
	DIR *dir=opendir(file_path.c_str());
	if(dir==nullptr){
		writeLog("open dir failed.\n");
		exit(0);
	}
	while((dirp = readdir(dp)) != nullptr){
		this->files_list.push_back(dirp->d_name);
	}
	closedir(dir);
}