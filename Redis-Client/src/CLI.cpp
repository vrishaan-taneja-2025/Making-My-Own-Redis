#include "CLI.h"
#include <vector>

// simple helper to trim whitespace

static std::string trim(const std::string &s){
    size_t start = s.find_first_not_of("\t\n\r\f\v");
    if(start == std::string::npos){
        return "";
    }
    size_t end = s.find_last_not_of("\t\n\r\f\v");
    return s.substr(start,end - start +1);
}

CLI::CLI(const std::string &host, int port) :  port(port), host(host),redisClient(host,port){

}

void CLI::run(const std::vector<std::string>& commandArgs){
    if(!redisClient.connectToServer()){
        return;
    }
    if(!commandArgs.empty()){
        executeCommand(commandArgs);
    }
    

    std::cout << "Connected Successfully at " << redisClient.getSocketFD() << "\n";

    while(true){
        std::cout << host << ":" << port <<">";
        std::cout.flush();
        std::string line;

        if(!std::getline(std::cin, line)){
            break;
        }

        line = trim(line);

        if(line.empty()){
            continue;

        }

        if(line == "quit" || line == "exit"){
            std::cout << "Goodbye\n";
            break;
        }

        if(line == "help"){
            std::cout << "Displaying help\n";
            continue;
        }

        //split command into tokens

        std::vector<std::string> args = CommandHandler::splitArgs(line);

        if(args.empty()){
            continue;
        }

        // for(const auto &arg : args){
        //     std::cout << arg << "\n";
        // }

        std::string command = CommandHandler::buildRESPcommand(args);
        if(!redisClient.sendCommand(command)){
            std::cerr << "(Error) Failed to send command \n";
            break;
        }

        // Parse and print response
        std::string response = ResponseParser::parseResponse(redisClient.getSocketFD());

        std::cout << response << "\n";
    }
}

void CLI::executeCommand(const std::vector<std::string>& commandArgs ){
    if(commandArgs.empty()){
        return;
    }

    std::string command = CommandHandler::buildRESPcommand(commandArgs);

    // for(const auto &arg : commandArgs){
    //     std::cout << arg << "\n";
    // }
        
    if(!redisClient.sendCommand(command)){
        std::cout << "(Error) Failed to send command.\n";
        return;
    }

    //parse and print response
    std::string response = ResponseParser::parseResponse(redisClient.getSocketFD());
    std::cout << response << "\n";


}