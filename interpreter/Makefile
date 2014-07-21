#There is a missing dependancy somewhere in this file.
#Make will (on rare occasion) complain about multiple definition or some other linker error, which can be fixed by recompiling the whole project.
#TODO: find the dependancy if it ever resurfaces

# -pg -g -O3
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
libbackend.a: main.o TSL.o functionTable.o progContext.o variableContext.o tDataSpecializations.o tArray.o threadEnvironment.o runtimeError.o systemError.o tetraEnvironment.o
	ar rcs libbackend.a main.o TSL.o functionTable.o progContext.o variableContext.o tDataSpecializations.o tArray.o threadEnvironment.o runtimeError.o systemError.o tetraEnvironment.o

run.o: run.cpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c run.cpp

main.o: main.cpp operationMap.cpp comparisonMap.cpp parallel.h backend.hpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c main.cpp

comparisonMap.o: comparisonMap.cpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c comparisonMap.cpp

functionTable.o: functionTable.cpp backend.hpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c functionTable.cpp

#nodeTable.o: nodeTable.cpp nodeTable.h
#	$(CC) $(FLAGS) -I$(SEARCHPATH) -c nodeTable.cpp

operationMap.o: operationMap.cpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c operationMap.cpp

variableContext.o: variableContext.cpp backend.hpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c variableContext.cpp

#tData.o: tData.h
#	$(CC) $(FLAGS) -I$(SEARCHPATH) -c tData.h

tDataSpecializations.o: tDataSpecializations.cpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c tDataSpecializations.cpp

progContext.o: backend.hpp progContext.cpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c progContext.cpp

tArray.o: tArray.cpp backend.hpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c tArray.cpp

TSL.o: TSL.cpp backend.hpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c TSL.cpp

threadEnvironment.o: threadEnvironment.cpp backend.hpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c threadEnvironment.cpp

runtimeError.o: runtimeError.cpp backend.hpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c runtimeError.cpp

systemError.o: systemError.cpp backend.hpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c systemError.cpp

tetraEnvironment.o: tetraEnvironment.cpp backend.hpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c tetraEnvironment.cpp

.PHONY clean:
	rm *.o
