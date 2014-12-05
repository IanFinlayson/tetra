#There is a missing dependancy somewhere in this file.
#Make will (on rare occasion) complain about multiple definition or some other linker error, which can be fixed by recompiling the whole project.
#TODO: find the dependancy if it ever resurfaces

# -pg -O3
CC = clang++
FLAGS = -Wall -pedantic -pthread -ftemplate-backtrace-limit=1 -pg
SEARCHPATH = ../frontend
DEBUG_MODE = -D USE_OBSERVER
OPTIMIZE_MODE = -O3
done: tetra
	@echo
	@echo Build Completed

#tetra: run.o main.o TSL.o functionTable.o nodeTable.o progContext.o variableContext.o tDataSpecializations.o tArray.o
#	$(CC) $(FLAGS) run.o main.o TSL.o functionTable.o nodeTable.o progContext.o variableContext.o tDataSpecializations.o tArray.o $(SEARCHPATH)/libfrontend.a -o tetra	
tetra: run.o libbackend.a
	$(CC) $(FLAGS) run.o libbackend.a $(SEARCHPATH)/libfrontend.a -o tetra

#create a linkable library letting other programs call interpret(Node* node)
libbackend.a: main.o TSL.o functionTable.o progContext.o variableContext.o tDataSpecializations.o tArray.o threadEnvironment.o runtimeError.o systemError.o commandConsole.o tetraEnvironment.o commandObserver.o virtualObserver.o
	ar rcs libbackend.a main.o TSL.o functionTable.o progContext.o variableContext.o tDataSpecializations.o tArray.o threadEnvironment.o runtimeError.o systemError.o commandConsole.o tetraEnvironment.o commandObserver.o virtualObserver.o
	@echo

run.o: run.cpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c run.cpp
	@echo

main.o: main.cpp operationMap.cpp comparisonMap.cpp parallel.h backend.hpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c main.cpp
	@echo

comparisonMap.o: comparisonMap.cpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c comparisonMap.cpp
	@echo

functionTable.o: functionTable.cpp backend.hpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c functionTable.cpp
	@echo

#nodeTable.o: nodeTable.cpp nodeTable.h
#	$(CC) $(FLAGS) -I$(SEARCHPATH) -c nodeTable.cpp

operationMap.o: operationMap.cpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c operationMap.cpp
	@echo

variableContext.o: variableContext.cpp backend.hpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c variableContext.cpp
	@echo

#tData.o: tData.h
#	$(CC) $(FLAGS) -I$(SEARCHPATH) -c tData.h

tDataSpecializations.o: tDataSpecializations.cpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c tDataSpecializations.cpp
	@echo

progContext.o: backend.hpp progContext.cpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c progContext.cpp
	@echo

tArray.o: tArray.cpp backend.hpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c tArray.cpp
	@echo

TSL.o: TSL.cpp backend.hpp commandConsole.h
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c TSL.cpp
	@echo

threadEnvironment.o: threadEnvironment.cpp backend.hpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c threadEnvironment.cpp
	@echo

runtimeError.o: runtimeError.cpp backend.hpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c runtimeError.cpp
	@echo

systemError.o: systemError.cpp backend.hpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c systemError.cpp
	@echo

tetraEnvironment.o: tetraEnvironment.cpp backend.hpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c tetraEnvironment.cpp
	@echo

commandConsole.o: commandConsole.cpp backend.hpp commandConsole.h
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c commandConsole.cpp
	@echo

commandObserver.o: commandObserver.h commandObserver.cpp backend.hpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c commandObserver.cpp
	@echo

virtualObserver.o: virtualObserver.cpp backend.hpp
	$(CC) $(FLAGS) -I$(SEARCHPATH) -c virtualObserver.cpp
	@echo

DEBUG:
	touch main.cpp
	@echo $(DEBUG_MODE)
	$(eval FLAGS += $(DEBUG_MODE))
	@echo $(FLAGS)
	$(eval export FLAGS)
#	make FLAGS="$(FLAGS)"
	make -e

OPTIMIZED:
	rm *.o
	@echo $(OPTIMIZE_MODE)
	$(eval FLAGS += $(OPTIMIZE_MODE))
	@echo $(FLAGS)
	$(eval export FLAGS)
#	make FLAGS="$(FLAGS)"
	make -e

N_DEBUG:
	touch main.cpp
	make


.PHONY clean:
	rm *.o
