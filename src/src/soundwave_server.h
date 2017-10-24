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
    SoundwaveServer();
    void run();

private:
    int serverSocket;
    int bufferSize;
    struct sockaddr_in saddr;
};

#endif
