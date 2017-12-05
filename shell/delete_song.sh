#!/bin/bash

export PGPASSWORD=CS4523
usage="Usage: -p <path> -u <username>"

while getopts ":u:p:" opt; do
    case $opt in
        p) song_path="$OPTARG";
        ;;
        u) username="$OPTARG";
        ;;
        \?) echo $usage;  exit 1;
        ;;
    esac
done

if [ -z "$song_path" ] || [ -z "$username" ];
then
    echo $usage;
    exit 1
fi

mkdir -p ../../tmp

cat ../../sql/templates/delete_song.template | sed -e "s:<song>:$song_path:g" -e "s:<name>:$username:g" > ../../tmp/delete_song.sql

psql -U postgres -f ../../tmp/delete_song.sql -d postgres -v "ON_ERROR_STOP=1"

psql_error_code=$?

rm ../../tmp/delete_song.sql

if [ $psql_error_code -ne 0 ];
then
    echo $psql_error_code;
    exit $psql_error_code;
fi

echo $?

exit $?

