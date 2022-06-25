#!/bin/bash


window_sizes="3 4 5 6 7 8 9 10"

filter_freqs="0 1 2 3"
progName=$(basename "$BASH_SOURCE")


function usage {
  echo
  echo "Usage: $progName <Path to the WSI files(input)> <PATH TO RESULTS DIRECTORY(output)> "
  echo
  echo " This script execute the emrgence program several times for different values of parameters"
  echo
  echo "  Options:"
  echo "    -h this help"
  echo
}



OPTIND=1
while getopts 'h' option ; do 
    case $option in
	"h" ) usage
 	      exit 0;;
#	"f" ) force=1;;
#	"l" ) trainFile="$OPTARG";;
 	"?" ) 
	    echo "Error, unknow option." 1>&2
            printHelp=1;;
    esac
done
shift $(($OPTIND - 1))
if [ $# -ne 2 ]; then
    echo "Error: expecting 2 args." 1>&2
    printHelp=1
fi
if [ ! -z "$printHelp" ]; then
    usage 1>&2
    exit 1
fi

input_dir=$1
output_dir=$2

for windowsize in $window_sizes; do

    for filterfreq in $filter_freqs; do

	./EMERGENCE_test "$input_dir" "$output_dir.$windowsize.$filterfreq" "$filterfreq" "$windowsize"
    done
done



