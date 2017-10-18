#!/bin/bash

export PGPASSWORD=CS4523
usage="Usage: -u <username> -p <password>"

while getopts ":u:p:" opt; do
    case $opt in
        u) username="$OPTARG"
        ;;
        p) password="$OPTARG"
        ;;
        \?) echo $usage; exit 1;
        ;;
    esac
done

if [ -z $username ] || [ -z $password ]; 
then 
    echo $usage;
    exit 1
fi

cat ../sql/templates/insert_user | sed -e "s/<username>/$username/g" -e "s/<password>/$password/g" > ../tmp/insert_user.sql

psql -U postgres -f ../tmp/insert_user.sql postgres

rm ../tmp/insert_user.sql
