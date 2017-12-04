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
#include <map>
#include <mutex>
#include <utility>
#include <string>

#include "../../lib/log.h"
#include "../../lib/status_codes.h"
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
    
    void createSong(int client);

    void createModification(int client);

    void updateSong(int client);
    
    SoundwaveUser* findUser(const std::string& username, bool create = false);

    void loadSongFromNetwork(char* buffer, int client, std::ofstream& fileStream);

    std::string readStringFromNetwork(char* buffer, int client);
private:
    std::map<std::string, SoundwaveUser> users;
    std::mutex maplock;
    int serverSocket;
    int bufferSize;
    struct sockaddr_in saddr;
};


#endif
