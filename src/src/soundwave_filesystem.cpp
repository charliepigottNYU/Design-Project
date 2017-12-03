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
    string songPath = user + "/" + songName;
    bool created = SoundwaveDatabase::createSong(user, songName, songPath); // we should be generating the song path first? the database only stores song paths?
    if (!created) {
        LOG(INFO) << "soundwave_filesystem.cpp:createSong: " << "Song already exists";
        return false;
    }
    ofs.open("../../data/" + songPath);
    if (!ofs) {
        LOG(ERROR) << "soundwave_filesystem.cpp:createSong: " << "Unable to open song path" << "../../data/" + songPath << endl;
        return false;
    }
    size_t dotPos = songPath.rfind(".");
    string folder = "../../data/" + songPath.substr(0, dotPos);
    LOG(INFO) << "soundwave_filesystem.cpp:createSong: " << "Path: " << songPath << ", Folder: " << folder;
    struct stat st = {0};
    if (stat(folder.c_str(), &st) == -1) {
        mkdir(folder.c_str(), 0777);
    }
    LOG(INFO) << "soundwave_filesystem.cpp:createSong: " << "Song created";
    return true;
}

void SoundwaveFilesystem::createUserFolder(const string& user) {
    string path = "../../data/" + user;
    DIR* directory;
    if ((directory = opendir(path.c_str())) == NULL) {
        mkdir(path.c_str(), 0777);
    }
}

bool SoundwaveFilesystem::createModification(ofstream& ofs, const string& user, const string& modifier, const string& songName) {
    string songPath = user + "/" + songName;
    size_t dotPos = songPath.rfind(".");
    string modPath = songPath.substr(0, dotPos) + "/" + modifier + songPath.substr(dotPos);
    bool created = SoundwaveDatabase::createModification(modifier, songPath, modPath);
    if (!created) {
        LOG(INFO) << "soundwave_filesystem.cpp:createModification: " << "Modification already exists";
        return false;
    }
    ofs.open("../../data/" + modPath);
    if (!ofs) {
        LOG(ERROR) << "soundwave_filesystem.cpp:createModification: " << "Unable to open song path" << "../../data/" + modPath << endl;
        return false;
    }
    return true;
}
