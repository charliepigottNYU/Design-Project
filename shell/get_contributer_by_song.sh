#!/bin/bash

export PGPASSWORD=CS4523
usage="Usage: -s <song name> -u <username>"
while getopts ":s:u:" opt; do
    case $opt in
        s) song="$OPTARG"
        ;;
        u) username="$OPTARG"
        ;;
        \?) echo $usage; exit1;
        ;;
    esac
done

if [ -z "$song" ] || [ -z "$username" ];
then
    echo $usage;
    exit 1
fi

mkdir -p ../../tmp

cat ../../sql/templates/get_contributer_by_song.template | sed -e "s:<creator>:$username:g" -e "s:<name>:$song:g" > ../../tmp/get_contributer_by_song.sql

CONTRIBUTERS=$(psql -U postgres -f ../../tmp/get_contributer_by_song.sql -d postgres -t -q -v "ON_ERROR_STOP=1")

psql_error_code=$?

rm ../../tmp/get_contributer_by_song.sql

if [ $psql_error_code -ne 0 ];
then
    exit $psql_error_code;
fi

echo $CONTRIBUTERS

exit $?
