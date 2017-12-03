#ifndef SOUNDWAVE_USER
#define SOUNDWAVE_USER

#include <vector>
#include <mutex>
#include <string>
#include <fstream>

#include "../../lib/log.h"
#include "soundwave_filesystem.h"

class SoundwaveFilesystem;

class SoundwaveUser {
public:

    SoundwaveUser(const std::string& username);

    SoundwaveUser(SoundwaveUser&& swUser);

    bool createSong(std::ofstream& ofs, const std::string& songName);

    bool createModification(std::ofstream& ofs, const SoundwaveUser* modifier, const std::string& songName);

private:
    std::string username;
    SoundwaveFilesystem* sfs;
    std::mutex userlock;
};

#endif
