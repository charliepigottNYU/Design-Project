CREATE TABLE user_info(
    username        TEXT PRIMARY KEY    NOT NULL,
    password        TEXT                NOT NULL

);

CREATE TABLE song_info(
    song_path       TEXT                NOT NULL,
    username        TEXT                NOT NULL,
    PRIMARY KEY (song_path, username),
    FOREIGN KEY (username) REFERENCES user_info (username)
);


CREATE TABLE modification_info(
    modification_path       TEXT PRIMARY KEY    NOT NULL,
    original_path           TEXT                NOT NULL,
    modifier                TEXT                NOT NULL,
    FOREIGN KEY(original_path) REFERENCES song_info(song_path)
);

CREATE TABLE song_contributers(
    song_path       TEXT        NOT NULL,
    contributer     TEXT        NOT NULL,
    PRIMARY KEY(song_path, contributer),
    FOREIGN KEY(song_path) REFERENCES song_info(song_path)
);
