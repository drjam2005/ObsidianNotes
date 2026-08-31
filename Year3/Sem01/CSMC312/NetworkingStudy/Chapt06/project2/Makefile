all: server

server: src/server_main.cpp server.o
	g++ src/server_main.cpp server.o socket.o -o server.out

server.o: src/server.cpp include/server.h socket.o
	g++ -c src/server.cpp -o server.o -c

socket.o: src/socket.cpp include/socket.h
	g++ -c src/socket.cpp -o socket.o -c
