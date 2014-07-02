#!/bin/bash

#A primitive shell script to run the interpreter with actual programs and check the results
#For a ttr file postfixed Test.ttr, simply add the prefix to the prefix array, and the expected return value to the results array

rm log.txt
echo "LOGFILE:" > log.txt

retVal=-1
successFlag=1
tetraPath=..

prefixes[0]="j";	results[0]=120
prefixes[1]="k";	results[1]=3
#prefixes[2]="l";	results[2]=2 removed temporarily
prefixes[${#prefixes[*]}]="m";	results[${#results[*]}]=2
prefixes[${#prefixes[*]}]="n";	results[${#results[*]}]=10
prefixes[${#prefixes[*]}]="o";	results[${#results[*]}]=2
prefixes[${#prefixes[*]}]="p";	results[${#results[*]}]=40
prefixes[${#prefixes[*]}]="array";	results[${#results[*]}]=10
prefixes[${#prefixes[*]}]="a";	results[${#results[*]}]=40
prefixes[${#prefixes[*]}]="aFor";	results[${#results[*]}]=30
prefixes[${#prefixes[*]}]="funArray";	results[${#results[*]}]=60
prefixes[${#prefixes[*]}]="litArray";	results[${#results[*]}]=12
prefixes[${#prefixes[*]}]="unary";	results[${#results[*]}]=10
prefixes[${#prefixes[*]}]="2D";	results[${#results[*]}]=2
prefixes[${#prefixes[*]}]="2DFun";	results[${#results[*]}]=4
prefixes[${#prefixes[*]}]="ultA";	results[${#results[*]}]=22
prefixes[${#prefixes[*]}]="forLit";	results[${#results[*]}]=55
prefixes[${#prefixes[*]}]="ref";	results[${#results[*]}]=200
prefixes[${#prefixes[*]}]="refref";	results[${#results[*]}]=222
prefixes[${#prefixes[*]}]="dA";	results[${#results[*]}]=10
prefixes[${#prefixes[*]}]="retA";	results[${#results[*]}]=44
prefixes[${#prefixes[*]}]="shortCircuit";	results[${#results[*]}]=26

successes=0
total=${#prefixes[*]}

for index in $(seq 0 `expr ${#prefixes[*]} - 1`)
do
	echo
	echo Testing  ${prefixes[ ${index} ]}Test.ttr
	${tetraPath}/tetra ${prefixes[ ${index} ]}Test.ttr >> log.txt
	retVal=$?
	if [ $retVal -eq ${results[${index}]} ]
	then
		echo 'Successful return of '${results[$index]}
		successes=`expr ${successes} + 1`
	else
		echo 'Failed: Returned '$retVal' but expected '${results[$index]}
		successFlag=0
	fi
done

echo
echo 'Tests complete'
echo 'Successful in '$successes'/'$total
echo

if [ $successFlag -eq 1 ]
then
	echo 'Tests completed successfully'
else
	echo 'Not all tests were completed successfully'
fi	
echo
echo
