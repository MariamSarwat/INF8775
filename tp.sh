#!/usr/bin/env bash
# ./tp.sh -a {brute, recursif, seuil} -e CHEMIN_EXEMPLAIRE [-p] [-t]
# https://www.baeldung.com/linux/use-command-line-arguments-in-bash-script
# https://archive.is/TRzn4

while getopts a:e: flag
do
    case "${flag}" in
        a) algoType=${OPTARG};;
        e) filePath=${OPTARG};;
    esac
done

g++ Divide_And_Conquer.cpp -o main 
./main -a "$algoType" -e "$filePath"