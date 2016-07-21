#!/bin/bash

if [ $# -eq 0 ]
then
  echo "Usage: setExpectedOutput.sh [filename(s)]"
else
  for file in $*
    do
      ../../interpreter/tetra $file > ./output/${file}.out
    done

    echo "Done setting expected test program output."
fi
