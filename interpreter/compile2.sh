g++ -Wall -c -I../frontend operationMap.cpp
g++ -Wall operationMap.o ../frontend/libfrontend.a
rm operationMap.o
