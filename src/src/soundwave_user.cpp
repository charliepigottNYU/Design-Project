#include "soundwave_user.h"

using namespace std;

SoundwaveUser::SoundwaveUser(string& username): username(username) {
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

