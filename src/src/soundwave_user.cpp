#include "soundwave_user.h"

using namespace std;

SoundwaveUser::SoundwaveUser(string& username): username(username) {
    sfs = SoundwaveFilesystem::getInstance();
}

bool SoundwaveUser::createSong(ofstream& ofs, const string& songName) {
    return true;
}
