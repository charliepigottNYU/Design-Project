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

bool SoundwaveUser::createModification(ofstream& ofs, const SoundwaveUser* modifier, const string& songName) {
    userlock.lock();
    bool result = sfs->createModification(ofs, username, modifier->username, songName);
    userlock.unlock();
    return result;
}

