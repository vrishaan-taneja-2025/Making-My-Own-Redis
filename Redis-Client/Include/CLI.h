#ifndef CLI_H
#define CLI_H


#include <string>
#include "RedisClient.h"
#include "CommandHandler.h"
#include "ResponseParser.h"

class CLI{
    public:
        CLI(const std::string &host, int port);
        void run(const std::vector<std::string>& commandArgs);
        void executeCommand(const std::vector<std::string>& commandArgs);
    
    private:
        int port;
        std::string host;
        RedisClient redisClient;

};

#endif