g++ -c -I../frontend treeAnalyzer.cpp
g++ treeAnalyzer.o ../frontend/libfrontend.a
rm treeAnalyzer.o
