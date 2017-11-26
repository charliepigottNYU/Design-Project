package lib

// COMMANDS (frontend to backend server commands)
const (
    _ uint8 = iota
    CommandCreateSong
    CommandModifySong
    CommandVoteSong
    CommandDeleteSong
)

