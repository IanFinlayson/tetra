CC = g++
FLAGS = -Wall
PATH = ../frontend

tetra: main.o 
	$(CC) $(FLAGS) main.o $(PATH)libfrontend.a -o tetra
	
main.o: main.cpp
	$(CC) $(FLAGS) -I$(PATH) -c main.cpp
