#!/bin/bash

echo "Running test suite..."

pass=0
fail=0
for file in *.ttr
do 
  ../../bin/tetra $file > ./output/tmp.txt
  diffWords=`diff ./output/tmp.txt ./output/${file}.out | wc -w`
  if [ $diffWords -ne 0 ]
  then
    fail=$(($fail + 1))
    echo "********** $file failed **********"
  else
    pass=$(($pass + 1))
  fi
done

echo "${pass}/$(($pass + $fail)) tests were successful."
