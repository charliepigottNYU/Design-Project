#include "soundwave_database.h"

using namespace std;

bool SoundwaveDatabase::createSong(const string& user, const string& songName, const string& songPath) {
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

bool SoundwaveDatabase::createModification(const string& user, const string& songPath, const std::string& modPath) {
    pid_t pid;
    if ((pid = fork()) == 0) {
        LOG(INFO) << "soundwave_database.cpp:createModification " << "Execute shell script";
        execl("../../shell/add_modification.sh", "add_modification.sh", "-u", user.c_str(), 
            "-p", songPath.c_str(), "-m", modPath.c_str());
    }
    int status;
    wait(&status);
    LOG(INFO) << "soundwave_database.cpp:CreateModification: " << "Status: " << status;
    return status == 0;
}
