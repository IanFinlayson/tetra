g++ -Wall -c -I../frontend main.cpp
g++ -Wall main.o ../frontend/libfrontend.a
rm main.o
