/*

Establish TCP connection to Redis (RedisClient)
    Berkeley sockets to open TCP connection
    IPv4 and IPv6 resolution using getaddrinfo

    Implement:
        connectToServer()   -> Establishes the connection.
        sendCommand()       -> Sends a command over the socket.
        disconnect()        -> Closses the socket when finished.

*/


#include "RedisClient.h"

RedisClient::RedisClient(const std::string &host, int port):host(host),port(port), sockfd(-1){

}

RedisClient::~RedisClient(){
    disconnect();

}

bool RedisClient :: connectToServer(){
    struct addrinfo hints , *res = nullptr;
    std::memset(&hints,0,sizeof(hints));     //clear hints structure
    hints.ai_family = AF_UNSPEC;             // Allow IPv4 or IPv6
    hints.ai_socktype = SOCK_STREAM;         // Use TCP

    std::string postStr = std::to_string(port); //convert port number to string
    int err = getaddrinfo(host.c_str(), postStr.c_str(), &hints, &res); //resolve address
    if (err != 0){
        std::cerr << "getaddrinfo: " << gai_strerror(err) << "\n";
        return false; //return false of faliure
    }

    //iterate through the resolved address
    for(auto p = res; p != nullptr; p = p->ai_next){
        sockfd = socket(p->ai_family, p->ai_socktype, p->ai_protocol); //create the socket
        if(sockfd == -1){
            continue;       //skip if the socket connection failed
        }
        if(connect(sockfd, p->ai_addr, p->ai_addrlen) == 0){ //attempt to connect
            break; //break on successful connecttion
        }
        close(sockfd); //close socket if connection failed
        sockfd = -1; //Reset the socket file description
    }

    freeaddrinfo(res); //free address information

    if(sockfd == -1){
        std::cerr << "Could not connect to " << host << ":" << port << "\n";
        return false; //return false if connection not made and print error message

    }

    return true; //return true on successfull connection
}

void RedisClient::disconnect(){
    if(sockfd == -1){
        close(sockfd); //close socket if connection failed
        sockfd = -1; //Reset socket file descriptor
    }
}

int RedisClient::getSocketFD() const{
    return sockfd;
}

bool RedisClient::sendCommand(const std::string &command){
    if(sockfd == -1){
        return false;
    }
    ssize_t sent = send(sockfd,command.c_str(),command.size(),0);

    return(sent == (ssize_t)command.size());
}