#include "soundwave_filesystem.h"

SoundwaveFilesystem* SoundwaveFilesystem::instance = nullptr;

using namespace std;

SoundwaveFilesystem* SoundwaveFilesystem::getInstance() {
    if (instance == nullptr) {
        instance = new SoundwaveFilesystem();
    }
    return instance;
}

SoundwaveFilesystem::SoundwaveFilesystem(){}

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
    size_t dotPos = songPath.rfind(".");
    string folder = songPath.substr(0, dotPos);
    cout << "path: " << songPath << ", folder: " << folder <<endl;
    struct stat st = {0};
    if (stat(folder.c_str(), &st) == -1) {
        mkdir(folder.c_str(), 0777);
    }
    return true;
}

vector<string> SoundwaveFilesystem::getAllSongs(const string& user) {
    string path = "../../data/" + user;
    vector<string> songs;
    DIR* directory;
    if ((directory = opendir(path.c_str())) == NULL) {
        mkdir(path.c_str(), 0777);
        return songs;
    }
    struct dirent* entry;
    while ((entry = readdir(directory)) != NULL) {
        struct stat st = {0};
        string name = entry->d_name;
        string songPath = path + "/" + name;
        //skip unopenable files and directories
        if (stat(songPath.c_str(), &st) == -1 || S_ISDIR(st.st_mode)) {
            continue;
        }
        songs.push_back(name);
    }
    return songs;
}
