#ifndef RESPONSEPARSER_H
#define RESPONSEPARSER_H

#include <string>

class ResponseParser{
    public:
        // read from the given socket and return parsed response a string, return "" in faliure
        static std::string parseResponse(int sockfd);
    private:
        //redis serialisation protocol 2
        static std::string parseSimpleString(int sockfd);
        static std::string parseSimpleError(int sockfd);
        static std::string parseInteger(int sockfd);
        static std::string parseBulkString(int sockfd);
        static std::string praseArray(int sockfd);

};

#endif