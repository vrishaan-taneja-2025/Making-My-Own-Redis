#ifndef REDISCLIENT_H
#define REDISCLIENT_H

#include <string>
#include <iostream>
#include <netdb.h>
#include <sys/socket.h>
#include <unistd.h>
#include <cstring>

class RedisClient{
    public:
        RedisClient(const std::string &host, int port);
        ~RedisClient();

        bool connectToServer();
        void disconnect();
        int getSocketFD() const;
        bool sendCommand(const std::string &command);

    private:
        std::string host;
        int port;
        int sockfd;

};

#endif