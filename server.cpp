#include "server.hpp"


network::Server::Server(int inputPort){
    servPort = inputPort;
    createSocket();
    initializeListener();
};

void network::Server::createSocket(){
    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener == -1){
        std::cout << "unable to initialize listener socket" << std::endl;
    }else{
        hint.sin_family = AF_INET;
        hint.sin_port = htons(servPort);
        inet_pton(AF_INET, "127.0.0.1", &hint.sin_addr);

        //binding socket
        if(bind(listener, (sockaddr*)&hint, sizeof(hint)) == -1){
            std::cout << "Bind failed" << std::endl;
        }else{
            //creating socket listener
            if(listen(listener, SOMAXCONN) == -1){
                std::cout << "listen failed" << std::endl;
            }else{
                std::cout << "createSocket() succeeded" << std::endl;
            }
        }
    }
};

void network::Server::initializeListener(){

    socklen_t clientSize = sizeof(client);
    while(true){
        clientSock = accept(listener, (sockaddr*)&client, &clientSize);
        performAction();
        close(clientSock);
    }
    
};

void network::Server::performAction(){
    std::cout << "hello" << std::endl;
};