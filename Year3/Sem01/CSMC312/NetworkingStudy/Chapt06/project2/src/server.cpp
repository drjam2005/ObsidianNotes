#include "../include/server.h"

#include <cstdio>
#include <cstring>
#include <thread>

Server::Server(const char* host, int port)
    : hostname(host), port(port)
{
}

void Server::handleJoin()
{
    while (true) {
        Socket newClient = tcpServer.Accept();

        int id = static_cast<int>(clients.size());

        clients.push_back({
            .id = id,
            .client_fd = std::move(newClient)
        });

        printf("Client %d connected\n", id);
    }
}

void Server::run()
{
    if (!tcpServer.Listen(hostname, port)) {
        printf("Failed to listen to %s:%d\n",
               hostname, port);
        return;
    }

    printf("Now listening to %s:%d\n",
           hostname, port);

    Socket newClient = tcpServer.Accept();

    clients.push_back({
        .id = 0,
        .client_fd = std::move(newClient)
    });

    printf("Client 0 connected\n");

    while (true) {
        char buffer[256];

        ssize_t received =
            clients[0].client_fd.Recv(
                buffer,
                sizeof(buffer) - 1
            );

        if (received <= 0) {
            printf("Client 0 disconnected\n");
            break;
        }

        buffer[received] = '\0';

        printf("(0): %s", buffer);
    }
}
