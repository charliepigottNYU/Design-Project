#ifndef SOUNDWAVE_DATABASE
#define SOUNDWAVE_DATABASE

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
#include <iostream>

class SoundwaveDatabase {
public:
    static bool createSong(const std::string& user, const std::string& songName, const std::string& songPath);
    //static bool deleteSong(std::string user, std::string songPath);

};



#endif
