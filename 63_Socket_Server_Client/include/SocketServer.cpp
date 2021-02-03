#include "SocketServer.h"

/* Pending connections queue will hold */
#define BACKLOG 10

namespace socketServerClient {
    SocketServer::SocketServer(int port)
    {
        this->port = port;
        this->serverSocketfd = -1;
        this->clientSocketfd = -1;
        this->clientConnect = false;
    }

    int SocketServer::listenClient()
    {
        serverSocketfd = socket(AF_INET, SOCK_STREAM, 0);
        if (serverSocketfd < 0)
        {
            std::cout << "Socket Server: error opening socket" << std::endl;
            return 1;
        }

        serverAddr.sin_family = AF_INET;
        serverAddr.sin_addr.s_addr = INADDR_ANY;
        serverAddr.sin_port = htons(port);
        memset(&serverAddr.sin_zero, 0, sizeof serverAddr.sin_zero);
        
        if (bind(serverSocketfd, (struct sockaddr *)&serverAddr, sizeof serverAddr) < 0)
        {
            std::cout << "Socket Server: error on binding the socket" << std::endl;
            return 1;
        }

        if (listen(serverSocketfd, BACKLOG) < 0)
        {
            std::cout << "Socket Server: error on listen the socket" << std::endl;
            return 1;
        }

        socklen_t clientLength = sizeof clientAddr;
        clientSocketfd = accept(serverSocketfd, (struct sockaddr *)&clientAddr, &clientLength);
        if (clientSocketfd < 0)
        {
            std::cout << "Socket Server: Failed to bind the client socket properly" << std::endl;
            return 1;
        }

        clientConnect = true;
        return 0;
    }

    int SocketServer::send(std::string message)
    {
        const char *buff = message.data();
        int length = message.length();
        if (write(clientSocketfd, buff, length) < 0)
        {
            std::cout << "Socket Server: error writing to server socket" << std::endl;
            return 1;
        }
        return 0;
    }

    std::string SocketServer::receive(int size = 1024)
    {
        char readbuff[size];
        if (read(clientSocketfd, readbuff, sizeof(readbuff)) < 0)
        {
            std::cout << "Socket Server: error reading from server socket" << std::endl;
        }
        return std::string(readbuff);
    }

    SocketServer::~SocketServer()
    {   
        clientConnect = false;
        close(clientSocketfd);
        close(serverSocketfd);
    }

}   /* namespace socketServerClient */