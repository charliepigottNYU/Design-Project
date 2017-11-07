#include "soundwave_server.h"

int main() {
    SoundwaveServer* server = SoundwaveServer::getInstance();
    server->run();
    return 0;
}
