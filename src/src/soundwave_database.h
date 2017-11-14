#ifndef SOUNDWAVE_DATABASE
#define SOUNDWAVE_DATABASE

class SoundwaveDatabase {
public:
    static bool createSong(std::string user, std::string songPath);
    static bool deleteSong(std::string user, std::string songPath);

};



#endif
