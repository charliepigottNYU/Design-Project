#include "soundwave_database.h"

bool SoundwaveDatabase::createSong(std::string user, std::string songPath) {
    pid_t pid;
    if ((pid = fork()) > 0)
        execl("../../shell/add_song.sh", "-p", user.c_str(), "-u", songPath.c_str(), (char*) NULL);
    int status;
    wait(&status);
    return status == 0;
}
