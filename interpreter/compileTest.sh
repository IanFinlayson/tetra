g++ -c -Wall -I../frontend diagnoseTree.cpp
#echo "compile successful"
g++ -Wall diagnoseTree.o ../frontend/libfrontend.a
rm diagnoseTree.o
