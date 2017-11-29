#include "soundwave_server.h"

using namespace std;

SoundwaveServer* SoundwaveServer::instance = nullptr;

SoundwaveServer::SoundwaveServer(): bufferSize(1024), users() {
    FILE* logFile = fopen("../../log/soundwave_server.log", "a");
    Output2FILE::Stream() = logFile;
    //create server file descriptor
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htonl(INADDR_ANY);
    saddr.sin_port = htons(5000);
    //bind server fd to address information
    if (::bind(serverSocket, (struct sockaddr*) &saddr, sizeof(saddr)) != 0) {
        LOG(ERROR) << "soundwave_server.cpp:SoundwaverServer: " << "Bind error" << endl;
    }
}

SoundwaveServer* SoundwaveServer::getInstance() {
    if (instance == nullptr) {
        instance = new SoundwaveServer();
    }
    return instance;
}

void SoundwaveServer::run() {
    //mark server socket for listening with a backlog of 10 tcp connections
    if (listen(serverSocket, 10) != 0) {
        LOG(ERROR) << "soundwave_server.cpp:run: " << "Listen error" << endl;
    }
    while(true) {  // change to threadpool to avoid spawning infinitely many threads
        int client = accept(serverSocket, NULL, NULL);
        if (client == -1) {
            LOG(ERROR) << "sounwave_server.cpp:run: " << "Accept error" << endl;
        }
        Command command;
        read(client, &command, sizeof(Command));
        switch (command) {
            case Command::CreateSong:{
                thread runClient(&SoundwaveServer::CreateSong, this, client);
                runClient.detach();
                break;}
            case Command::ModifySong:{
                break;}
            case Command::VoteSong:{
                break;}
            case Command::DeleteSong:{
                break;}
        }
   }
}


void SoundwaveServer::CreateSong(int client) {
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
    } else {
        user = &(userItr->second);
    }
    maplock.unlock();
    
    int64_t songNameSize;
    read(client, &songNameSize, sizeof(int64_t));
    read(client, buffer, songNameSize);
    string songName = buffer;
    memset(buffer, 0, bufferSize);

    ofstream fileStream;
    uint8_t isValid = 1;
    for (size_t i = 0; i < songName.size(); ++i) {
        if (songName[i] == ' ')
            songName[i] = '-';
    }
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
