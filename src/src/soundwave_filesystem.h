#ifndef SOUNDWAVE_FILESYSTEM
#define SOUNDWAVE_FILESYSTEM

#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <unistd.h>
#include <fstream>
#include <vector>
#include <iostream>

#include "../../lib/log.h"
#include "soundwave_user.h"
#include "soundwave_database.h"


class SoundwaveFilesystem {
    static SoundwaveFilesystem* instance;

    SoundwaveFilesystem();

    static SoundwaveFilesystem* getInstance();

    friend class SoundwaveUser;

public:
    /*
      Checks if the song is a valid song name, creates and opens the file if it is valid
      modifies the database to include the new song.
      creates a folder for user songs if it does not exist yet
      creates an empty folder for future song modifications
      returns true if song file is created/opened
      does not fill the song file with any data
    */
    bool createSong(std::ofstream& ofs, const std::string& user, const std::string& songName);

    bool createModification(std::ofstream& ofs, const std::string& user, const std::string& modifier, const std::string& songName, const std::string& songPath);
    
    void updateSong(const std::string& user, const std::string& modifier, const std::string& songName, const std::string& path);

    void createUserFolder(const std::string& user);

    //void deleteSong(std::string user, std::string songName);
    
};

#endif
