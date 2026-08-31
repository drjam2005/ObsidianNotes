all: server client

server: src/server.cpp include/server.h include/packets.h
	g++ src/server.cpp -o server

client: src/client.cpp include/client.h include/packets.h
	g++ src/client.cpp -o client -Lvendor/lib -l:libraylib.a -lX11
