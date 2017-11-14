#ifndef SOUNDWAVE_USER
#define SOUNDWAVE_USER

#include <vector>
#include <mutex>
#include "soundwave_filesystem.h"

class SoundwaveUser {
public:

    SoundwaveUser();

    bool createSong(std::ofstream& ofs, const std::string& songName);

private:
    std::string username;
    std::vector<std::string> songs;
    SoundwaveFilesystem* sfs;
    std::mutex userlock;
}

#endif
