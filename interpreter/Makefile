#At present, because there is no split between the .h and .cpp files, much of this makefile is unnecessary at the moment


CC = g++
FLAGS = -Wall
SEARCHPATH = ../frontend

tetra: main.o functionTable.o nodeTable.o progContext.o variableContext.o tDataSpecializations.o
	$(CC) $(FLAGS) main.o functionTable.o nodeTable.o progContext.o variableContext.o tDataSpecializations.o $(SEARCHPATH)/libfrontend.a -o tetra
	#$(CC) $(FLAGS) voidlessMain.o $(SEARCHPATH)/libfrontend.a -o tetra
	rm *.o
	@echo
	@echo

main.o: main.cpp functionTable.h nodeTable.h operationMap.cpp comparisonMap.cpp variableContext.h tData.cpp progContext.h
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c main.cpp

comparisonMap.o: comparisonMap.cpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c comparisonMap.cpp

functionTable.o: functionTable.cpp functionTable.h
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c functionTable.cpp

nodeTable.o: nodeTable.cpp nodeTable.h
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c nodeTable.cpp

operationMap.o: operationMap.cpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c operationMap.cpp

variableContext.o: variableContext.cpp variableContext.h
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c variableContext.cpp

tData.o: tData.cpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c tData.cpp

tDataSpecializations.o: tDataSpecializations.cpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c tDataSpecializations.cpp

progContext.o: progContext.h progContext.cpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c progContext.cpp

.PHONY clean:
	rm *.o
