#ifndef SOUNDWAVE_SERVER
#define SOUNDWAVE_SERVER

#include <sys/socket.h>
#include <sys/types.h>
#include <unistd.h>
#include <netinet/in.h>
#include <iostream>
#include <cstring>

class SoundwaveServer{
public:
    SoundwaveServer();
    void run();
private:
    int serverSocket;
    struct sockaddr_in saddr;
};

#endif
