files=../lib/log.cc ../lib/fileManager.cc ../lib/connection.cc src/client.cc main.cc
debug/client:
	if ! (test -d debug);then  \
		mkdir debug; \
	fi
	g++ -pthread $(files)  -o debug/client
clean:
	rm debug/client