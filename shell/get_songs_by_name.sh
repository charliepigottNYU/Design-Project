#!/bin/bash

export PGPASSWORD=CS4523
usage="Usage: -s <song name>"
while getopts ":s:" opt; do
    case $opt in
        s) song="$OPTARG"
        ;;
        \?) echo $usage; exit 1;
        ;;
    esac
done

if [ -z $song ];
then
    echo $usage;
    exit 1
fi

mkdir -p ../../tmp

cat ../../sql/templates/get_songs_by_name.template | sed -e "s:<name>:$song:g" > ../../tmp/get_songs_by_name.sql

SONGS=$(psql -U postgres -f ../../tmp/get_songs_by_name.sql -d postgres -t -q -v "ON_ERROR_STOP=1")

psql_error_code=$?

rm ../../tmp/get_songs_by_name.sql

if [ $psql_error_code -ne 0 ];
then
    exit $psql_error_code;
fi

echo $SONGS

exit $?
