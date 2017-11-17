#!/bin/bash

export PGPASSWORD=CS4523
usage="Usage: -u <username> -p <path> -n <song name>"

while getopts ":u:p:n:" opt; do
    case $opt in
        u) username="$OPTARG"; 
        ;;
        p) song_path="$OPTARG";
        ;;
        n) song_name="$OPTARG";
        ;;
        \?) echo $usage;  exit 1;
        ;;
    esac
done

if [ -z $username ] || [ -z $song_path ] || [ -z $song_name ];
then
    echo $usage;
    exit 1
fi

mkdir -p ../../tmp

cat ../../sql/templates/insert_song.template | sed -e "s:<username>:$username:g" -e "s:<song_path>:$song_path:g" -e "s:<name>:$song_name:g" > ../../tmp/insert_song.sql

psql -U postgres -f ../../tmp/insert_song.sql -d postgres -v "ON_ERROR_STOP=1"

psql_error_code=$?

rm ../../tmp/insert_song.sql

if [ $psql_error_code -ne 0 ];
then
    echo $psql_error_code;
    exit $psql_error_code;
fi

echo $?

exit $?

