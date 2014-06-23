#At present, because there is no split between the .h and .cpp files, much of this makefile is unnecessary at the moment


CC = clang++
FLAGS = -Wall -g
SEARCHPATH = ../frontend

tetra: main.o functionTable.o nodeTable.o progContext.o variableContext.o tDataSpecializations.o tArray.o
	$(CC) $(FLAGS) main.o functionTable.o nodeTable.o progContext.o variableContext.o tDataSpecializations.o tArray.o $(SEARCHPATH)/libfrontend.a -o tetra
	#$(CC) $(FLAGS) voidlessMain.o $(SEARCHPATH)/libfrontend.a -o tetra
	rm *.o
	@echo
	@echo

main.o: main.cpp functionTable.h nodeTable.h operationMap.cpp comparisonMap.cpp variableContext.h tData.h progContext.h
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

tData.o: tData.h
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c tData.h

tDataSpecializations.o: tDataSpecializations.cpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c tDataSpecializations.cpp

progContext.o: progContext.h progContext.cpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c progContext.cpp

tArray.o: tArray.cpp tArray.h
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c tArray.cpp

.PHONY clean:
	rm *.o
