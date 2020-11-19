#include <iostream>
#include <string>
#include <sstream> 
#include "client_handler.hpp"
#include "client.hpp"



int main(int argc, char const *argv[]){
    printWelcomeText();

    //user input
    std::string userInputIpAddress;
    std::string userInputPort;
    std::string userInputDomainName;
    std::cout << "Enter the DNS server IP address that you want to connect to:" << std::endl;
    std::cin >> userInputIpAddress;
    std::cout << "Enter the DNS server port number that you want to connect to:" << std::endl;
    std::cin >> userInputPort;
    std::cout << "Enter the domain name that you want to resolve: " << std::endl;
    std::cin >> userInputDomainName;
    
    //TODO: validate user input

    //convert user input into correct format
    //TODO: need to research on domain name before modifying. Only convert IP addr 
    //      and port for now
    int port = std::stoi(userInputPort);
    
    //initializing client
    //std::string inputServerIpAddr, int inputPort, void* data
    network::Client clientTest = network::Client( userInputIpAddress,port);

    // testing:
    std::cout << userInputIpAddress << std::endl;
    std::cout << userInputPort << std::endl;
    std::cout << userInputDomainName << std::endl;

    

    return 0;
}