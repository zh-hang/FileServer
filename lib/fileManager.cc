#include "fileManager.h"

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
    auto dirp=readdir(dir);
	while(dirp != nullptr){
        if(dirp->d_type==DT_REG)
    		this->files_list.push_back(dirp->d_name);
        dirp=readdir(dir);
	}
	closedir(dir);
    std::cout<<std::endl<<"find file: \n";
    for(auto file:this->files_list){
        std::cout<<file<<" ";
    }
    std::cout<<std::endl;
}