//Author: Feng Wu

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
    

    int port = std::stoi(userInputPort);
    
    //initializing client
    network::Client clientTest = network::Client( userInputIpAddress,port, userInputDomainName);



    return 0;
}