#ifndef SOUNDWAVE_DATABASE
#define SOUNDWAVE_DATABASE

#include <unistd.h>

class SoundwaveDatabase {
public:
    static bool createSong(const std::string& user, const std::string& songPath);
    //static bool deleteSong(std::string user, std::string songPath);

};



#endif
