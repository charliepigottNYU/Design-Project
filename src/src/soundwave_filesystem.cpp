#include "soundwave_filesystem.h"

SoundwaveFilesystem* SoundwaveFilesystem::instance = nullptr;

using namespace std;

bool SoundwaveFilesystem::createSong(ofstream& ofs, const string& user, const string& songName) {
    string songPath = "../../data/" + user + "/" + songName;
    bool created = SoundwaveDatabase::createSong(user, songPath); // we should be generating the song path first? the database only stores song paths?
    if (!created) {
        return false;
    }
    ofs.open(songPath);
    if (!ofs) {
        return false;
    }
    size_t dotPos = songPath.find(".");
    string folder = songPath.substr(0, dotPos);
    cout << "path: " << songPath << ", folder: " << folder <<endl;
    struct stat st = {0};
    if (stat(folder.c_str(), &st) == -1) {
        mkdir(folder, 0777);
    }
    return true;
}

