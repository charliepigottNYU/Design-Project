#include "soundwave_user.h"

using namespace std;

SoundwaveUser::SoundwaveUser(const string& username): username(username) {
    sfs = SoundwaveFilesystem::getInstance();
    sfs->createUserFolder(username);
}

SoundwaveUser::SoundwaveUser(SoundwaveUser&& swUser): username(move(swUser.username)), sfs(swUser.sfs) {}

bool SoundwaveUser::createSong(ofstream& ofs, const string& songName) {
    userlock.lock();
    bool result = sfs->createSong(ofs, username, songName);
    userlock.unlock();
    return result;
}

bool SoundwaveUser::createModification(ofstream& ofs, const SoundwaveUser* modifier, const string& songName, const string& path) {
    userlock.lock();
    bool result = sfs->createModification(ofs, username, modifier->username, songName, path);
    userlock.unlock();
    return result;
}

void SoundwaveUser::updateSong(const string& songName, const string& modifier) {
    userlock.lock();
    sfs->updateSong(username, modifier, songName);
    userlock.unlock();
}
