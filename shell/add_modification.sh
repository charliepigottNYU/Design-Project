#!/bin/bash

export PGPASSWORD=CS4523
usage="Usage: -u <modifier username> -p <path> -m <mod path>"

while getopts ":u:p:m:" opt; do
    case $opt in
        u) username="$OPTARG"; 
        ;;
        p) song_path="$OPTARG";
        ;;
        m) mod_path="$OPTARG";
        ;;
        \?) echo $usage;  exit 1;
        ;;
    esac
done

if [ -z "$username" ] || [ -z "$song_path" ] || [ -z "$mod_path" ];
then
    echo $usage;
    exit 1
fi

mkdir -p ../../tmp

cat ../../sql/templates/insert_modification.template | sed -e "s:<modifier>:$username:g" -e "s:<song_path>:$song_path:g" -e "s:<mod_path>:$mod_path:g" > ../../tmp/insert_modification.sql

psql -U postgres -f ../../tmp/insert_modification.sql -d postgres -v "ON_ERROR_STOP=1"

psql_error_code=$?

rm ../../tmp/insert_modification.sql

if [ $psql_error_code -ne 0 ];
then
    echo $psql_error_code;
    exit $psql_error_code;
fi

echo $?

exit $?

