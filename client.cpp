//Author: Feng Wu

#include "client.hpp"

network::Client::Client(std::string inputServerIpAddr, int inputPort, std::string userInputDomainName){
    serverIpAddr = inputServerIpAddr;
    port = inputPort;
    domainName = userInputDomainName;
    createSocket();
    connectToServer();
};

void network::Client::createSocket(){
    sock = socket(AF_INET, SOCK_STREAM, 0);
    if(sock == -1){
        std::cout << "unable to initialize socket" << std::endl;
    }else{
        hint.sin_family = AF_INET;
        hint.sin_port = htons(port);
        inet_pton(AF_INET, serverIpAddr.c_str(), &hint.sin_addr);
        std::cout << "createSocket() succeeded" << std::endl;
    }
};

void network::Client::connectToServer(){
    int con = connect(sock, (sockaddr*)&hint, sizeof(hint));
    if(con == -1){
        std::cout << "unable to connect to the server" << std::endl;
    }else{
        std::cout << "connectToServer() succeeded" << std::endl;
        std::cout << domainName << std::endl;
        unsigned char encodedData[512];
        unsigned char buf[512];
        strncpy((char*)encodedData, domainName.c_str(), sizeof(encodedData));
        int index = 0;
        while(encodedData[index] != '\0'){
            std::cout << "debug: " << encodedData[index] << std::endl;
            index++;
        }
        send(sock, encodedData, 512,0);
        int readingFromServer = read(sock, buf, 512);
        // index = 0;
        // while(buf[index] != '\0'){
        //     std::cout << "client recieve! debug: " << buf[index] <<std::endl;
        //     index++;
        // }
        int firstByte;
        unsigned char ipAddrValue[4];
        memcpy(&firstByte, buf, 1);
        std::cout << "firsbyte: " << firstByte << std::endl;
        memcpy(ipAddrValue, buf + 1, 4);
        std::string convertedIpAdress = convertUnsignedCharToIPAdress(ipAddrValue);
        std::cout << "ip address: " <<  convertedIpAdress << std::endl;
    }
    
};

void network::Client::shutDown(){
    close(sock);
    std::cout << "shutDown() succeeded" << std::endl;
};

