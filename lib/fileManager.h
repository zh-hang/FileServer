#include <vector>
#include <string>
#include <iostream>
#include <unistd.h>
#include <dirent.h>
#include <mutex>
#include <memory>

#include "log.h"

#ifndef FILE_MANAGER_H
#define FILE_MANAGER_H

class FileManager{
public:
	using FileManagerPtr=std::shared_ptr<FileManager>;

    static FileManagerPtr getFileManager(){
        _lock.lock();
        if(_my_instance==nullptr)
            _my_instance=FileManagerPtr(new FileManager());
        _lock.unlock();
        return _my_instance;
    }

	static std::vector<std::string> getFilesList(){
		return _files_list;
	}
	
	static std::string getFileName(int pos){
        if(pos>_files_list.size())
            return NULL;
		return _files_list[pos];
	}

private:
	static std::vector<std::string> _files_list;
    static std::mutex _lock;
    static FileManagerPtr _my_instance;
	FileManager();
	~FileManager();
};
#endif