g++ -Wall -c -I../frontend main.cpp
g++ -Wall -c -I../frontend functionTable.cpp
g++ -Wall main.o ../frontend/libfrontend.a functionTable.o
rm main.o
