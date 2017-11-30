#include "soundwave_database.h"

bool SoundwaveDatabase::createSong(const std::string& user, const std::string& songName, const std::string& songPath) {
    pid_t pid;
    if ((pid = fork()) == 0) {
        LOG(INFO) << "soundwave_database.cpp:createSong: " << "Execute shell script";
        execl ("../../shell/add_song.sh", "add_song.sh", "-u", user.c_str(), "-n", songName.c_str(), "-p", songPath.c_str(), (char*) NULL);
    }
    int status;
    wait(&status);
    LOG(INFO) << "soundwave_database.cpp:createSong: " << "Status: " << status;
    return status == 0;
}
