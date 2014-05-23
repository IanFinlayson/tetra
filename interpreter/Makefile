#At present, because there is no split between the .h and .cpp files, much of this makefile is unnecessary at the moment


CC = g++
FLAGS = -Wall
SEARCHPATH = ../frontend

tetra: main.o functionTable.o nodeTable.o operationMap.o 
	#$(CC) $(FLAGS) main.o functionTable.o nodeTable.o operationMap.o $(SEARCHPATH)/libfrontend.a -o tetra
	$(CC) $(FLAGS) main.o $(SEARCHPATH)/libfrontend.a -o tetra


main.o: main.cpp functionTable.cpp nodeTable.cpp operationMap.cpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c main.cpp

functionTable.o: functionTable.cpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c functionTable.cpp

nodeTable.o: nodeTable.cpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c nodeTable.cpp

operationMap.o: operationMap.cpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c operationMap.cpp

clean:
	rm *.o
