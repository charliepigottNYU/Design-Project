#ifndef SOUNDWAVE_DATABASE
#define SOUNDWAVE_DATABASE

#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <string>
#include <iostream>

#include "../../lib/log.h"

class SoundwaveDatabase {
public:
    static bool createSong(const std::string& user, const std::string& songName, const std::string& songPath);
    
    static bool createModification(const std::string& user, const std::string& songPath, const std::string& modPath);
    //static bool deleteSong(std::string user, std::string songPath);

};



#endif
