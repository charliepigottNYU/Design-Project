DROP TABLE IF EXISTS modification_info;
DROP TABLE IF EXISTS song_contributers;
DROP TABLE IF EXISTS song_info;
DROP TABLE IF EXISTS user_info;


CREATE TABLE user_info(
    username    TEXT PRIMARY KEY    NOT NULL,
    password    TEXT                NOT NULL

);

CREATE TABLE song_info(
    song_path   TEXT    UNIQUE NOT NULL,
    creator     TEXT           NOT NULL,
    name        TEXT           NOT NULL,
    PRIMARY KEY (song_path, creator, name),
    FOREIGN KEY (creator) REFERENCES user_info (username)
);


CREATE TABLE modification_info(
    modification_path   TEXT                NOT NULL,
    original_path       TEXT                NOT NULL,
    modifier            TEXT                NOT NULL,
    PRIMARY KEY (modification_path, original_path, modifier),
    FOREIGN KEY(original_path) REFERENCES song_info(song_path)
);

CREATE TABLE song_contributers(
    song_path       TEXT    NOT NULL,
    contributer     TEXT    NOT NULL,
    PRIMARY KEY(song_path, contributer),
    FOREIGN KEY(song_path)   REFERENCES song_info(song_path),
    FOREIGN KEY(contributer) REFERENCES user_info(username)
);
