#include "soundwave_filesystem.h"

SoundwaveFilesystem* SoundwaveFilesystem::instance = nullptr;

using namespace std;

bool SoundwaveFilesystem::createSong(ofstream& ofs, const string& user, const string& songPath) {
    bool created = SoundwaveDatabase::createSong(user, songPath);
    if (!created) {
        return false;
    }
    ofs.open(songPath);
    if (!ofs) {
        return false;
    }
    struct stat st = {0};
    if (stat(songPath.c_str(), &st) == -1) {
        mkdir(songPath, 0777);
    }
}
