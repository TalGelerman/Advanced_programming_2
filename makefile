ex2: main.o GenericVector.o Distances.o
	g++ -std=c++11 ./*.o -o ex2

main.o: main.cpp GenericVector.cpp GenericVector.h Distances.cpp Distances.h
	g++ -std=c++11 -c main.cpp GenericVector.cpp Distances.cpp

GenericVector.o: GenericVector.cpp GenericVector.h
	g++ -std=c++11 -c GenericVector.cpp

Distances.o: Distances.cpp Distances.h
	g++ -std=c++11 -c Distances.cpp
