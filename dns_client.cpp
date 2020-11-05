#include <iostream>
#include <string>
#include "client_handler.hpp"

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


    //testing:
    // std::cout << userInputIpAddress << std::endl;
    // std::cout << userInputPort << std::endl;
    // std::cout << userInputDomainName << std::endl;

    return 0;
}