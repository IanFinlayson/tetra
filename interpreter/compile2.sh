g++ -Wall -c -I../frontend variableContext.cpp
g++ -Wall variableContext.o ../frontend/libfrontend.a
rm variableContext.o
