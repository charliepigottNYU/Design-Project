#!/bin/bash

export PGPASSWORD=CS4523
usage="Usage: -u <username>"

while getopts ":u:" opt; do
    case $opt in
        u) username="$OPTARG"
        ;;
        \?) echo $usage; exit 1;
        ;;
    esac
done

if [ -z $username ];
then
    echo $usage;
    exit 1
fi

mkdir -p ../../tmp

cat ../../sql/templates/get_contributer_songs.template | sed -e "s:<username>:$username:g" > ../../tmp/get_contributer_songs.sql

SONGS=$(psql -U postgres -f ../../tmp/get_contributer_songs.sql -d postgres -t -q -v "ON_ERROR_STOP=1")

psql_error_code=$?

rm ../../tmp/get_contributer_songs.sql

if [ $psql_error_code -ne 0 ];
then
    exit $psql_error_code;
fi

echo $SONGS

exit $?
