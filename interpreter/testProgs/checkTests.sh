#!/bin/bash

#A primitive shell script to repeatedly run the interpreter with actual programs and check the results
#For a ttr file postfixed Test.ttr, simply add the prefix to the prefix array, and the expected return value to the results array
#Letting this script run is a primitive way to check for unforseen race conditions

rm checkTestLog.txt
echo "LOGFILE:" > testCheckLog.txt

retVal=-1
successFlag=1
tetraPath=..
reps=0

prefixes[${#prefixes[*]}]="bg";	results[${#results[*]}]=10
prefixes[${#prefixes[*]}]="lock";	results[${#results[*]}]=200
prefixes[${#prefixes[*]}]="par";	results[${#results[*]}]=3
prefixes[${#prefixes[*]}]="parRef";	results[${#results[*]}]=15
prefixes[${#prefixes[*]}]="parpar";	results[${#results[*]}]=50
prefixes[${#prefixes[*]}]="parFor";	results[${#results[*]}]=10
#prefixes[${#prefixes[*]}]="parFor2";	results[${#results[*]}]=225
prefixes[${#prefixes[*]}]="parVar";	results[${#results[*]}]=2
prefixes[${#prefixes[*]}]="parForSim";	results[${#results[*]}]=3

total=${#prefixes[*]}

while true
do

reps=`expr ${reps} + 1`
rm checkTestLog.txt

for index in $(seq 0 `expr ${#prefixes[*]} - 1`)
do


	${tetraPath}/tetra ${prefixes[ ${index} ]}Test.ttr >> checkTestLog.txt
	retVal=$?
	if [ $retVal -ne ${results[${index}]} ]
	then
		echo Program ${prefixes[ ${index} ]}Test.ttr has a race condition
		echo found after ${reps} repetitions
		exit 1
	fi

done
	if [ `expr ${reps} % 20` -eq 0 ]
	then
		echo At ${reps} repetitions
	fi
done
