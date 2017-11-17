all: main.cpp soundwave_server.cpp  
	g++ main.cpp soundwave_server.cpp soundwave_database.cpp  soundwave_filesystem.cpp soundwave_user.cpp -o soundwave_server -pthread -std=c++11
clean:
	rm soundwave_server
