all: client.out server.out

client.out: Client.o GenericVector.o Distances.o
	g++ -std=c++11 ./*.o -o Client.out

server.out: Server.o GenericVector.o Distances.o
	g++ -std=c++11 ./*.o -o Server.out

GenericVector.o: GenericVector.cpp GenericVector.h
	g++ -std=c++11 -c GenericVector.cpp

Distances.o: Distances.cpp Distances.h
	g++ -std=c++11 -c Distances.cpp

Server.o: Server.cpp Server.h
	g++ -std=c++11 -c Server.cpp

Client.o: Client.cpp Client.h
	g++ -std=c++11 -c Client.cpp

