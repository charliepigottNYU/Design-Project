#include "soundwave_server.h"


SoundwaveServer::SoundwaveServer(){
    serverSocket = socket(AF_INET, SOCK_STREAM, 0);
    memset(&saddr, 0, sizeof(saddr));
    saddr.sin_family = AF_INET;
    saddr.sin_addr.s_addr = htons(INADDR_ANY);
    saddr.sin_port = htons(5000);
    bind(serverSocket, (struct sockaddr*) &saddr, sizeof(saddr));
}

void SoundwaveServer::run(){
    listen(serverSocket, 10);
    char buffer[1024];
    while(true){
        int client = accept(serverSocket, NULL, NULL);
        int n;
        while((n = read(client, buffer, sizeof(buffer))) > 0){
            std::cout<<buffer;
            memset(buffer, 0, sizeof(buffer));
        }
    }
}
