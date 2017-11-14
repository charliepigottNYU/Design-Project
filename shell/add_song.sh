#!/bin/bash

export PGPASSWORD=CS4523
usage="Usage: -u <username> -p <path>"

while getopts ":u:p:" opt; do
    case $opt in
        u) username="$OPTARG"
        ;;
        p) song_path="$OPTARG"
        ;;
        \?) echo $usage; exit 1;
        ;;
    esac
done

if [ -z $username ] || [ -z $song_path ];
then
    echo $usage;
    exit 1
fi

mkdir -p ../../tmp

cat ../../sql/templates/insert_song.template | sed -e "s/<username>/$username/g" -e "s/<song_path>/$song_path/g" > ../../tmp/insert_song.sql

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

