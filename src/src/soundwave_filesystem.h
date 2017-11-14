#ifndef SOUNDWAVE_FILESYSTEM
#define SOUNDWAVE_FILESYSTEM

class SoundwaveFilesystem {
    static SoundwaveFilesystem* instance;

    SoundwaveFilesystem();

public:
    static SoundwaveFilesystem* getInstance();

    /*
      Checks if the song is a valid song name, creates and opens the file if it is valid
      modifies the database to include the new song.
      creates a folder for user songs if it does not exist yet
      creates an empty folder for future song modifications
      returns true if song file is created/opened
      does not fill the song file with any data
    */
    bool createSong(std::ofstream& ofs, std::string user, std::string songPath);

    void deleteSong(std::string user, std::string songPath);
    
    void voteSong(std::string user, std::string songPath);
};

#endif
