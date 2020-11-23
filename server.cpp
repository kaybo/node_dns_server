#include "server.hpp"
#include "dns_struct.hpp"
#include "root_servers.hpp"
#include <errno.h>

network::Server::Server(int inputPort){
    performAction();//remove later
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
    //TODO: put this into dns.cpp later. Can implement a primitive version of dnslookup 
    //here for now

    //This is a simple test query to the root server
    HEADER testHead = HEADER();
    testHead.arcount = 1337;

    testHead.qr = 0x1;
    testHead.opcode = 0x0;
    testHead.tc = 0x0;
    //left out aa for now: teadHead.aa
    testHead.rd = 0x1;
    testHead.ra = 0x0;

    testHead.qdcount = 1;
    testHead.ancount = 0;
    testHead.nscount = 0;
    testHead.arcount = 0;

    QUESTION testQuestion = QUESTION();

    testQuestion.qtype = 0x1;
    testQuestion.qclass = 0x1;

    //TODO: Encode the data into bytes after initializing the data

    //TODO: setup UDP and DNS server information

    //
    int dnsPort = 53;
    int dnsListener = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in dnsHint;
    if(dnsListener == -1){
        std::cout << "unable to initialize dnsLisener" << std::endl;
    }else{
        dnsHint.sin_family = AF_INET;
        dnsHint.sin_port = htons(dnsPort);
        inet_pton(AF_INET, A_ROOT.c_str(), &dnsHint.sin_addr);

        //send data to dns server via udp send
        // char *hello = "Hello from client"; 
        char hello[512];
        hello[0] = '1';
        hello[1] = '2';
        // sendto(dnsListener, (const char *)hello, strlen(hello), sizeof(dnsHint));
        sendto(dnsListener, (const char *)hello, strlen(hello), 
        0, (const struct sockaddr *) &dnsHint,  
            sizeof(dnsHint)); 

        char buffer[512]; 
        std::cout << "send test data to dns server " << std::endl; 
        socklen_t len;
        int n = recvfrom(dnsListener, (char *)buffer, 512,  
                MSG_PEEK, (struct sockaddr *) &dnsHint, 
                &len); 
        std::cout << "buff data: %s"  << buffer << std::endl;
        std::cout << "retrieve successful? " << std::endl; 
    }


};