#There is a missing dependancy somewhere in this file.
#Make will (on rare occasion) complain about multiple definition or some other linker error, which can be fixed by recompiling the whole project.

# -pg -O3
CC = clang++
FLAGS = -Wall -g -pedantic -pthread -ftemplate-backtrace-limit=1
SEARCHPATH = ../frontend

#tetra: run.o main.o TSL.o functionTable.o nodeTable.o progContext.o variableContext.o tDataSpecializations.o tArray.o
#	$(CC) $(FLAGS) run.o main.o TSL.o functionTable.o nodeTable.o progContext.o variableContext.o tDataSpecializations.o tArray.o $(SEARCHPATH)/libfrontend.a -o tetra	
tetra: run.o libbackend.a
	$(CC) $(FLAGS) run.o libbackend.a $(SEARCHPATH)/libfrontend.a -o tetra
	@echo
	@echo

#create a linkable library letting other programs call interpret(Node* node)
libbackend.a: main.o TSL.o functionTable.o nodeTable.o progContext.o variableContext.o tDataSpecializations.o tArray.o threadEnvironment.o
	ar rcs libbackend.a main.o TSL.o functionTable.o nodeTable.o progContext.o variableContext.o tDataSpecializations.o tArray.o threadEnvironment.o

run.o: run.cpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c run.cpp

main.o: main.cpp functionTable.h nodeTable.h operationMap.cpp comparisonMap.cpp variableContext.h tData.h progContext.h TSL.h parallel.h threadEnvironment.h
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

TSL.o: TSL.cpp TSL.h
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c TSL.cpp

threadEnvironment.o: threadEnvironment.cpp threadEnvironment.h
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c threadEnvironment.cpp

.PHONY clean:
	rm *.o
