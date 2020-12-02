//Author: Feng Wu

#include "server.hpp"
#include "root_servers.hpp"
#include <errno.h>
#include "utility.hpp"
#include <bitset>


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
    testHead.id = htons(1337);
    testHead.qr = 0x0;
    std::cout <<"testing testhead.qr: " <<  std::bitset<8>(testHead.qr)<< std::endl;
    // testHead.opcode = htons(0x0);
    testHead.tc = 0x0;
    //left out aa for now: teadHead.aa

    //TODO: NEED TO FIX HEADER AGAIN!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!
    testHead.rd = 0x1;

    testHead.qdcount = htons(1);
    testHead.ancount = htons(0);
    testHead.nscount = htons(0);
    testHead.arcount = htons(0);

    QUESTION testQuestion = QUESTION();

    testQuestion.qtype = htons(1);
    testQuestion.qclass = htons(1);

    unsigned char *testConversion = convertHostNameToDNSField("sfu.ca");
    std::string domainName = "sfu.ca";
    unsigned char *encodedInfo = encodeDNSQuery(domainName,testHead,testQuestion);
    int totalSize = sizeof(testHead) 
    + strlen((const char*)testConversion) + 1 + sizeof(testQuestion.qclass) + sizeof(testQuestion.qtype);



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

        int result = sendto(dnsListener, encodedInfo, totalSize, 
        0, (const struct sockaddr *) &dnsHint,  
            sizeof(dnsHint)); 
        if(result == -1){
            std::cout <<"error with sendto()" << std::endl;
        }

        unsigned char buffer[512]; 
        memset (&buffer, 0, 512);
        //todo: look up bind9 and wireshark
        std::cout << "send test data to dns server: " << result << std::endl; 
    
        
        socklen_t len = 0;
        int n = recvfrom(dnsListener, buffer, 512,  
                0, (struct sockaddr *) &dnsHint, 
                &len); 
        std::cout << "retrieve successful? " << std::endl; 

        decodeDNSRespond(buffer);
    }


};