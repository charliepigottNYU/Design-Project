#!/bin/bash

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
