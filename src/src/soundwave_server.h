#ifndef SOUNDWAVE_SERVER
#define SOUNDWAVE_SERVER

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <cstring>
#include <iostream>
#include <fstream>


class SoundwaveServer {
public:
    //contsructor for soundwave server. Initialized serversocket, bufferSize and saddr
    //to correct values for running a server over tcp.
    SoundwaveServer();
    //run is a loop that continuously accepts connections and then recieves song information over tcp
    //it creates song files in the filesystem based on the user info sent over tcp
    void run();

private:
    int serverSocket;
    int bufferSize;
    struct sockaddr_in saddr;
};

#endif
