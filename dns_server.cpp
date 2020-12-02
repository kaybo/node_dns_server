//Author: Feng Wu

#include <iostream>
#include <string>
#include <sstream> 
#include "client_handler.hpp"
#include "server.hpp"



int main(int argc, char const *argv[]){
    // printWelcomeText();

    //user input

    std::string userInputPort;

    std::cout << "Enter the DNS server port number that you want to connect to:" << std::endl;
    std::cin >> userInputPort;

    //TODO: validate user input

    //convert user input into correct format
    //TODO: need to research on domain name before modifying. Only convert IP addr 
    //      and port for now
    int port = std::stoi(userInputPort);
    
    //initializing Server
    //std::string inputServerIpAddr, int inputPort, void* data
    network::Server clientTest = network::Server(port);



    

    return 0;
}