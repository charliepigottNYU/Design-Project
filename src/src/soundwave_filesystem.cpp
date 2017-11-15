#include "soundwave_filesystem.h"

SoundwaveFilesystem* SoundwaveFilesystem::instance = nullptr;

using namespace std;

bool SoundwaveFilesystem::createSong(ofstream& ofs, const string& user, const string& songName) {
    bool created = SoundwaveDatabase::createSong(user, songName); // we should be generating the song path first? the database only stores song paths?
    if (!created) {
        return false;
    }
    ofs.open(songName);
    if (!ofs) {
        return false;
    }
    struct stat st = {0};
    if (stat(songName.c_str(), &st) == -1) {
        mkdir(songName, 0777);
    }
}

