#include "ResponseParser.h"
#include <iostream>
#include <sstream>
#include <unistd.h>
#include <cstdlib>
#include <sys/types.h>
#include <sys/socket.h>


static bool readChar(int sockfd, char &c){
    ssize_t r = recv(sockfd, &c, 1, 0);
    return(r == 1);

}

static std::string readLine(int sockfd){
    std::string line;
    char c;
    while(readChar(sockfd,c)){
        if(c =='\r'){
            //accept \n as next; read and break
            readChar(sockfd,c);
            break;
        }
        line.push_back(c);
    }
    return line;
}

std::string ResponseParser::parseResponse(int sockfd){
    char prefix;
    if(!readChar(sockfd, prefix)){
        return ("(Error) No response or connection closed");
    }
    switch(prefix){
        case '+': return parseSimpleString(sockfd);
        case '-': return parseSimpleError(sockfd);
        case ':': return parseInteger(sockfd);
        case '$': return parseBulkString(sockfd);
        case '*': return praseArray(sockfd);
        default: 
        return "(Error) Unknown reply type.\n";
    }
}

std::string ResponseParser::parseSimpleString(int sockfd){
    return readLine(sockfd);
}
std::string ResponseParser::parseSimpleError(int sockfd){
    return "(Error) " + readLine(sockfd);
}
std::string ResponseParser::parseInteger(int sockfd){
    return readLine(sockfd);
}
std::string ResponseParser::parseBulkString(int sockfd){
    std::string lenstr = readLine(sockfd);
    int length = std::stoi(lenstr);
    if(length == -1){
        return "(nil)";
    }
    std::string bulk;
    bulk.resize(length);
    int totalRead = 0;
    //loop to read bulk data from the socket
    while(totalRead < length){
        ssize_t r = recv(sockfd, &bulk[totalRead], length- totalRead,0);
        if(r <= 0){
            return "(Error) Incomplete bulk data";
        }
        totalRead += r;
    }
    //consume trailing CRLF
    char dummy;
    readChar(sockfd,dummy);// Read the first CR
    readChar(sockfd,dummy);//Read the LF
    return bulk;
}
std::string ResponseParser::praseArray(int sockfd){
    std::string counter = readLine(sockfd);
    int count = std::stoi(counter);
    if(count == -1){
        return "(nil)";
    }
    std::ostringstream oss;

    for(int i = 0; i < count; ++i){
        oss << parseResponse(sockfd);
        if(i != count -1){
            oss << "\n";
        }
    }
    return oss.str();
}