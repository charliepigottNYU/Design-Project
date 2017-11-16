#include "soundwave_user.h"

using namespace std;

SoundwaveUser::SoundwaveUser(string& username): username(username) {
    sfs = SoundwaveFilesystem::getInstance();
}

SoundwaveUser::SoundwaveUser(SoundwaveUser&& swUser): username(move(swUser.username)),
   songs(move(swUser.songs)), sfs(swUser.sfs) {}

void SoundwaveUser::initUnlock(mutex& toUnlock) {
    userlock.lock();
    toUnlock.unlock();
    songs = move(sfs->getAllSongs(username));
    userlock.unlock();
}

bool SoundwaveUser::createSong(ofstream& ofs, const string& songName) {
    return true;
}

