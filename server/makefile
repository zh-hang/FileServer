files=../lib/threadpool.cc ../lib/log.cc ../lib/fileManager.cc ../lib/connection.cc src/server.cc src/userManager.cc main.cc
debug/server:
	if !(test -d debug);then \
		mkdir debug; \
	fi
	g++ -pthread $(files) -o debug/server
	
clean:
	rm debug/server