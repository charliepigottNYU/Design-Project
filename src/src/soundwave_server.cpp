#include "soundwave_server.h"


SoundwaveServer::SoundwaveServer(): bufferSize(1024) {
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htons(INADDR_ANY);
    saddr.sin_port = htons(5000);
    bind(serverSocket, (struct sockaddr*) &saddr, sizeof(saddr));
}

void SoundwaveServer::run() {
    listen(serverSocket, 10);
    char buffer[bufferSize];
    while(true) {
        int client = accept(serverSocket, NULL, NULL);
        int n;

        n = read(client, buffer, bufferSize);
        int fileSize = atoi(buffer);
        int remainingData = fileSize;
        memset(buffer, 0, bufferSize);

        n = read(client, buffer, bufferSize);
        std::string filename(buffer);
        memset(buffer, 0, bufferSize);

        std::ofstream fileStream;
        fileStream.open(filename);
        if (!fileStream) {  // NOTE: improve error handling
            std::cout << "File opening failed" << std::endl;
            std::exit(1);
        }

        while((n = read(client, buffer, bufferSize)) > 0 && (remainingData > 0)) {
            std::cout << buffer;  // NOTE: remove
            fileStream.write(buffer, n);
            remainingData -= n;
            memset(buffer, 0, bufferSize);
        }

        fileStream.close();
    }
}
