#At present, because there is no split between the .h and .cpp files, much of this makefile is unnecessary at the moment


CC = g++
FLAGS = -Wall
SEARCHPATH = ../../frontend

voidlessTetra: voidlessMain.o functionTable.o nodeTable.o voidlessOperationMap.o voidlessComparisonMap.o
	#$(CC) $(FLAGS) main.o functionTable.o nodeTable.o operationMap.o comparisonMap.o $(SEARCHPATH)/libfrontend.a -o tetra
	$(CC) $(FLAGS) voidlessMain.o $(SEARCHPATH)/libfrontend.a -o voidlessTetra
	rm *.o
	@echo
	@echo

voidlessMain.o: voidlessMain.cpp functionTable.cpp nodeTable.cpp voidlessOperationMap.cpp voidlessComparisonMap.cpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c voidlessMain.cpp

voidlessComparisonMap.o: voidlessComparisonMap.cpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c voidlessComparisonMap.cpp

functionTable.o: functionTable.cpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c functionTable.cpp

nodeTable.o: nodeTable.cpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c nodeTable.cpp

voidlessOperationMap.o: voidlessOperationMap.cpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c voidlessOperationMap.cpp

.PHONY clean:
	rm *.o
