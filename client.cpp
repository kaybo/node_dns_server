#include "client.hpp"

network::Client::Client(std::string inputServerIpAddr, int inputPort){
    serverIpAddr = inputServerIpAddr;
    port = inputPort;
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
    }
    
};

void network::Client::shutDown(){
    close(sock);
    std::cout << "shutDown() succeeded" << std::endl;
};

