#!/bin/bash

#A primitive shell script to run the interpreter with actual programs and check the results
#For a ttr file postfixed Test.ttr, simply add the prefix to the prefix array, and the expected return value to the results array

rm log.txt
echo "LOGFILE:" > log.txt
cd ..

retVal=-1
successFlag=1

prefixes[0]="j";	results[0]=120
prefixes[1]="k";	results[1]=3
prefixes[2]="l";	results[2]=2
prefixes[${#prefixes[*]}]="m";	results[${#results[*]}]=2

for index in $(seq 0 `expr ${#prefixes[*]} - 1`)
do
	echo
	echo Testing  ${prefixes[ ${index} ]}Test.ttr
	./tetra testProgs/${prefixes[ ${index} ]}Test.ttr >> log.txt
	retVal=$?
	if [ $retVal -eq ${results[${index}]} ]
	then
		echo 'Successful return of '${results[$index]}
	else
		echo 'Failed: Returned '$retVal' but expected '${results[$index]}
		successFlag=0
	fi
done

echo
echo 'Tests complete'
echo

if [ $successFlag -eq 1 ]
then
	echo 'Tests completed successfully'
else
	echo 'Not all tests were completed successfully'
fi

cd testProgs	
