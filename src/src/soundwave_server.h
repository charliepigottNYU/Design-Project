#ifndef SOUNDWAVE_SERVER
#define SOUNDWAVE_SERVER

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>
#include <iostream>
#include <fstream>
#include <thread>

#include "soundwave_filesystem.h"
#include "soundwave_user.h"
#include "soundwave_database.h"

class SoundwaveServer {
    static SoundwaveServer* instance;
    //contsructor for soundwave server. Initialized serversocket, bufferSize and saddr
    //to correct values for running a server over tcp.
    SoundwaveServer();
    
public:
    static SoundwaveServer* getInstance();
    void run();
    
    void handleClient(int client);
private:
    int serverSocket;
    int bufferSize;
    struct sockaddr_in saddr;
};


#endif
