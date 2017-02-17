#!/bin/bash

echo "Running test suite..."

pass=0
fail=0
for file in *.ttr
do 
    ../../bin/tetra $file > ./output/tmp.txt
    if [ ! -e "./output/${file}.out" ]
    then
        echo "~~~~~~~ ./output/${file}.out not found ~~~~~~~~~"
        diffWords="100"
    else
        diffWords=`diff ./output/tmp.txt ./output/${file}.out | wc -w`
    fi
    if [ $diffWords -ne 0 ]
    then
        fail=$(($fail + 1))
        echo "********** $file failed **********"
    else
        pass=$(($pass + 1))
    fi
done

echo "${pass}/$(($pass + $fail)) tests were successful."
