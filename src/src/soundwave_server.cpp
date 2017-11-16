#include "soundwave_server.h"

using namespace std;

SoundwaveServer* SoundwaveServer::instance = nullptr;

SoundwaveServer::SoundwaveServer(): bufferSize(1024), users() {
    //create server file descriptor
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htons(INADDR_ANY);
    saddr.sin_port = htons(5000);
    //bind server fd to address information
    bind(serverSocket, (struct sockaddr*) &saddr, sizeof(saddr));
}

SoundwaveServer* SoundwaveServer::getInstance() {
    if (instance == nullptr) {
        instance = new SoundwaveServer();
    }
    return instance;
}

void SoundwaveServer::run() {
    //mark server socket for listening with a backlog of 10 tcp connections
    listen(serverSocket, 10);
    while(true) {  // change to threadpool to avoid spawning infinitely many threads
        int client = accept(serverSocket, NULL, NULL);
        thread runClient(&SoundwaveServer::handleClient, this, client);
        runClient.detach();
   }
}


void SoundwaveServer::handleClient(int client) {
    char* buffer = new char[bufferSize];
    memset(buffer, 0, bufferSize);
    //read in username size
    uint8_t userSize = 0;
    read(client, &userSize, sizeof(uint8_t));
    read(client, buffer, userSize);
    string username = buffer;
    memset(buffer, 0, bufferSize);

    SoundwaveUser* user = nullptr;
    maplock.lock();
    auto userItr = users.find(username);
    if (userItr == users.cend()) {
        users.emplace(make_pair(username, SoundwaveUser(username)));
        user = &users.at(username);
        user->initUnlock(maplock);
    } else {
        user = &(userItr->second);
        maplock.unlock();
    }
    
    int64_t songNameSize;
    read(client, &songNameSize, sizeof(int64_t));
    read(client, buffer, songNameSize);
    string songName = buffer;
    memset(buffer, 0, bufferSize);

    ofstream fileStream;
    uint8_t isValid = 1;
    if (!user->createSong(fileStream, songName)) {
        isValid = 0;
        write(client, &isValid, sizeof(uint8_t));
        close(client);
        return;
    }
    write(client, &isValid, sizeof(uint8_t));
    //read in file size
    int64_t songSize;
    read(client, &songSize, sizeof(int64_t));
    int64_t remainingData = songSize;
    memset(buffer, 0, bufferSize);

    int n;
    //read in song file
    while((n = read(client, buffer, bufferSize)) > 0 && (remainingData > 0)) {
        fileStream.write(buffer, n);
        remainingData -= n;
        memset(buffer, 0, bufferSize);
    }

    delete[] buffer;
    fileStream.close();
    close(client);
}
