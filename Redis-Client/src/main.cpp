/*

1. Command line argument parsing
    -h <host> default : 127.0.0.1 ;-p <port> default: 6379
    if not arguments launch interactive REPL mode
    
2. OOP
    RedisClient, CommandHandler, ResponseParser, CLI

3. Establish TCP connection to Redis (RedisClient)
    Berkeley sockets to open TCP connection
    IPv4 and IPv6 'getaddrinfo'

4. Parsing and command formatting (CommandHandler)
    split user input 
    Convert commands into RESP format:
        '''*3\r\n$3\r\nSET\r\n$3\r\nkey\r\n$5\r\nvalue\r\n'''

5. handling Redis Responses (ResponseParser)
    Read server Rresponses and parses RESP types
    +OK -Error, :100, $5\r\nhello -> bulk sting, *2\r\n$3\r\nfoo\r\n$3\r\nbar -> Array response

6. Implement interactive REPL (CLI)
    Run loop: User Input, valis redis commands, send command to server, display parsed response Support: help, quit

7. main.cpp: parse command-line args, instantiate CLI and Launch in CLI mode.

Socket Programming
Protocol Handling 
OOP principles
CLI development

*/

#include <iostream>
#include "CLI.h"

int main(int argc,  char* argv[]){

    std::string host = "127.0.0.1";
    int port = 6379;
    int i = 1;
    std::vector<std::string> commandArgs;

    while(i<argc){
        std::string arg = argv[i];
        if(arg == "-h" && i+1 < argc){
            host = arg[++i];
        }else if (arg == "-p" && i+1 <argc){
            port = std::stoi(argv[++i]);
        }else{
            //remaining arguments
            while(i < argc){
                commandArgs.push_back(argv[i]);
                i++;
            }

            break;
        }
        ++i;
    }

    //Handle REPL and one-shot command nodes

    CLI cli(host,port);

    cli.run(commandArgs);

    return 0;
}