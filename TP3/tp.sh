#!/bin/bash

#   DOC :
#     Arguments non positionnels :
#       -e CHEMIN_EXEMPLAIRE -> INST="CHEMIN_EXEMPLAIRE"
#

INST=""

while getopts e: flag
do
    case "${flag}" in
        e)
            INST=$OPTARG
            ;;
    esac
done

g++ gold_main.cpp -o exec
./exec -e "$INST"
#python3 main.py $INST $ALG $TIME $PRINT
#java main.java $INST $ALG $TIME $PRINT
#...