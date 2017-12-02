#!/bin/bash

export PGPASSWORD=CS4523
usage="Usage: -v <votes> -p <path>"
while getopts ":v:p:" opt; do
    case $opt in
        v) vote="$OPTARG"
        ;;
        p) path="$OPTARG"
        ;;
        \?) echo $usage; exit 1;
        ;;
    esac
done

if [ -z "$vote" ] || [ -z "$path" ];
then
    echo $usage;
    exit 1
fi

mkdir -p ../../tmp

cat ../../sql/templates/modify_vote.template | sed -e "s:<vote_value>:$vote:g" -e "s:<modification_path>:$path:g" > ../../tmp/modify_vote.sql

VOTES=$(psql -U postgres -f ../../tmp/modify_vote.sql -d postgres -t -q -v "ON_ERROR_STOP=1")

psql_error_code=$?

rm ../../tmp/modify_vote.sql

if [ $psql_error_code -ne 0 ];
then
    exit $psql_error_code;
fi

echo $VOTES

exit $?