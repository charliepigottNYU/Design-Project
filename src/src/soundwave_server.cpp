#include "soundwave_server.h"

SoundwaveServer* SoundwaveServer::instance = nullptr;

SoundwaveServer::SoundwaveServer(): bufferSize(1024) {
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
    while(true) {
        int client = accept(serverSocket, NULL, NULL);
        std::thread runClient(&SoundwaveServer::handleClient, this, client);
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
    //read in file size
    int64_t songSize;
    int n = read(client, &songSize, sizeof(int64_t));
    int64_t remainingData = songSize;
    memset(buffer, 0, bufferSize);

    //currently sound.mp3 for testing purposes
    std::string filename = "sound.mp3";

    std::ofstream fileStream;
    fileStream.open(filename);
    if (!fileStream) {  // NOTE: improve error handling
        std::cout << "File opening failed" << std::endl;
        std::exit(1);
    }
    //read in song file
    while((n = read(client, buffer, bufferSize)) > 0 && (remainingData > 0)) {
        fileStream.write(buffer, n);
        remainingData -= n;
        memset(buffer, 0, bufferSize);
    }

    delete buffer;
    fileStream.close();
    close(client);
}
