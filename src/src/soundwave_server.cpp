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
        LOG(ERROR) << "soundwave_server.cpp:SoundwaverServer: " << "Bind error";
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
        LOG(ERROR) << "soundwave_server.cpp:run: " << "Listen error";
    }
    while(true) {  // change to threadpool to avoid spawning infinitely many threads
        int client = accept(serverSocket, NULL, NULL);
        if (client == -1) {
            LOG(ERROR) << "sounwave_server.cpp:run: " << "Accept error";
        }
        Command command;
        read(client, &command, sizeof(Command));
        LOG(INFO) << "soundwave_server.cpp:run: " << "Executing command " << unsigned(command);
        switch (command) {
            case Command::CreateSong:{
                LOG(INFO) << "soundwave_server.cpp:run: " << "Command::CreateSong";
                thread runClient(&SoundwaveServer::createSong, this, client);
                runClient.detach();
                break;}
            case Command::ModifySong:{
                LOG(INFO) << "soundwave_server.cpp:run: " << "Command::ModifySong";
                thread runClient(&SoundwaveServer::createModification, this, client);
                runClient.detach();
                break;}
            case Command::ReplaceSong:{
                LOG(INFO) << "soundwave_server.cpp:run: " << "Command::ReplaceSong";
                thread runClient(&SoundwaveServer::updateSong, this, client);
                runClient.detach();
                break;}
            case Command::DeleteSong:{
                LOG(INFO) << "soundwave_server.cpp:run: " << "Command::DeleteSong";
                break;}
        }
   }
}


void SoundwaveServer::createSong(int client) {
    char* buffer = new char[bufferSize];
    memset(buffer, 0, bufferSize);
    
    string username = readStringFromNetwork(buffer, client);

    SoundwaveUser* user = findUser(username, true);
    
    string songName = readStringFromNetwork(buffer, client);

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

    loadSongFromNetwork(buffer, client, fileStream);

    delete[] buffer;
    fileStream.close();
    close(client);
}

void SoundwaveServer::createModification(int client) {
    char* buffer = new char[bufferSize];
    memset(buffer, 0, bufferSize);
    
    string username = readStringFromNetwork(buffer, client); // read size/creator from connection
    
    SoundwaveUser* user = findUser(username, true);
    uint8_t isValid = 1;
    if (user == nullptr) {
        LOG(WARNING) << "soundwave_server.cpp:createModification: " << "Song Creator not found";
        isValid = 0;
        write(client, &isValid, sizeof(uint8_t));  // write failed response
        close(client);
        return;
    }
    write(client, &isValid, sizeof(uint8_t));  // write successful response

    username = readStringFromNetwork(buffer, client);  // read size/modifier from connection

    SoundwaveUser* modifier = findUser(username, true);
    if (modifier == nullptr) {
        LOG(WARNING) << "soundwave_server.cpp:createModification: " << "Song Modifier not found";
        isValid = 0;
        write(client, &isValid, sizeof(uint8_t));  // write failed response
        close(client);
        return;
    }
    write(client, &isValid, sizeof(uint8_t));  // write successful response

    string songName = readStringFromNetwork(buffer, client);  // read size/songname from connection

    ofstream fileStream;
    for (size_t i = 0; i < songName.size(); ++i) {
        if (songName[i] == ' ')
            songName[i] = '-';
    }
    if (!user->createModification(fileStream, modifier, songName)) {
        isValid = 0;
        write(client, &isValid, sizeof(uint8_t));
        close(client);
        return;
    }
    write(client, &isValid, sizeof(uint8_t));

    loadSongFromNetwork(buffer, client, fileStream);

    delete[] buffer;
    fileStream.close();
    close(client);
}

void SoundwaveServer::updateSong(int client) {
    char* buffer = new char[bufferSize];
    memset(buffer, 0, bufferSize);
    
    string username = readStringFromNetwork(buffer, client); // read size/creator from connection 
    string songName = readStringFromNetwork(buffer, client);
    string modified = readStringFromNetwork(buffer, client);
    SoundwaveUser* user = findUser(username, true);
    user->updateSong(songName, modified);
    
    uint8_t isValid = 1;
    write(client, &isValid, sizeof(uint8_t));

    delete[] buffer;
    close(client);
}

SoundwaveUser* SoundwaveServer::findUser(const string& username, bool create) {
    SoundwaveUser* user = nullptr;
    maplock.lock();
    auto userItr = users.find(username);
    if (userItr != users.cend()) {
        user = &(userItr->second);
    }
    else if (create) {
        users.emplace(make_pair(username, SoundwaveUser(username)));
        user = &users.at(username);
    }
    maplock.unlock();
    return user;
}

void SoundwaveServer::loadSongFromNetwork(char* buffer, int client, ofstream& fileStream) {
//read in file size
    int64_t songSize;
    read(client, &songSize, sizeof(int64_t));
    int64_t remainingData = songSize;
    memset(buffer, 0, bufferSize);
    LOG(INFO) << "soundwave_server.cpp:loadSongFromNetwork: " << "attempting to read song of size " << songSize << " over connection " << client;

    int n;
    //read in song file
    while((n = read(client, buffer, bufferSize)) > 0 && (remainingData > 0)) {
        fileStream.write(buffer, n);
        remainingData -= n;
        memset(buffer, 0, bufferSize);
    }
}

string SoundwaveServer::readStringFromNetwork(char* buffer, int client) {
    //read in string size
    uint8_t size = 0;
    read(client, &size, sizeof(uint8_t));
    read(client, buffer, size);
    string result = buffer;
    memset(buffer, 0, bufferSize);
    LOG(INFO) << "soundwave_server.cpp:readStringFromNetwork: " << "String read: " << result;
    return result;
}
