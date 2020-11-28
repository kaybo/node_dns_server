#include "server.hpp"
#include "dns_struct.hpp"
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
    // testHead.arcount = htons(1337);
    testHead.id = htons(0x0);
    testHead.qr = 0x0;
    std::cout <<"testing testhead.qr: " <<  std::bitset<8>(testHead.qr)<< std::endl;
    // testHead.opcode = htons(0x0);
    testHead.tc = 0x0;
    //left out aa for now: teadHead.aa
    // testHead.rd = htons(0x1);
    testHead.ra = 0x0;
    testHead.ad = 0x0;

    testHead.qdcount = htons(1);
    testHead.ancount = htons(0);
    testHead.nscount = htons(0);
    testHead.arcount = htons(0);

    QUESTION testQuestion = QUESTION();

    testQuestion.qtype = htons(1);
    testQuestion.qclass = htons(1);

    unsigned char *testConversion = convertHostNameToDNSField("www.northeastern.edu");
    // unsigned char testConversion[] = {'3','w', 'w','w','6','g','o','o','g','l','e','3','c','o','m','\0'};

    // testQuestion.qname = testConversion;

    int totalSize = sizeof(testHead) 
    + strlen((const char*)testConversion) + 1 + sizeof(testQuestion.qclass) + sizeof(testQuestion.qtype);



    //TODO: Encode the data into bytes after initializing the data
    std::cout << totalSize << std::endl;

    unsigned char *encodedInfo = new unsigned char[totalSize];
    memset(encodedInfo, 0 ,totalSize);
    unsigned char *pointerEncodedInfo = reinterpret_cast<unsigned char*>(encodedInfo);
    int pointerOffSet = 0;


    std::cout<< "this is pointer offset: " << testConversion << std::endl;
    
    

    memcpy(pointerEncodedInfo, &testHead, sizeof(testHead));


    pointerEncodedInfo += sizeof(testHead);
    std::cout <<"this is size of testHEAD: " <<sizeof(testHead) <<std::endl; 
    pointerOffSet = sizeof(testHead);
    // memcpy(pointerEncodedInfo, &testConversion, strlen((const char*)testConversion) + 1);
    char testCharArr[21];
    // memset(testCharArr,0,21);
    // memcpy(testCharArr, &testConversion, strlen((const char*)testConversion) );

    int testIndex = 0;
    while(testConversion[testIndex] != '\0'){

        encodedInfo[pointerOffSet] = testConversion[testIndex];
        testIndex++;
        pointerOffSet++;
    }
    pointerOffSet++;//for the \0 character

    for(int i = 0 ; i < totalSize; i++){
        std::cout << "pointerEncodedInfo before : " << encodedInfo[i] << std::endl;
    }
    // pointerEncodedInfo = pointerEncodedInfo + strlen((const char*)testConversion) + 1;
    pointerEncodedInfo +=  pointerOffSet;
    memcpy(encodedInfo+pointerOffSet, &testQuestion, sizeof(testQuestion));

    for(int i = 0 ; i < totalSize; i++){
        std::cout << "pointerEncodedInfo after : " << encodedInfo[i] << std::endl;
    }

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
        std::string testDNS = "208.67.222.222";
        inet_pton(AF_INET, A_ROOT.c_str(), &dnsHint.sin_addr);
        

        //send data to dns server via udp send


        int result = sendto(dnsListener, encodedInfo, totalSize, 
        0, (const struct sockaddr *) &dnsHint,  
            sizeof(dnsHint)); 
        if(result == -1){
            std::cout <<"error with sendto()" << std::endl;
        }

        char buffer[512]; 
        memset (&buffer, 0, 512);
        //todo: look up bind9 and wireshark
        std::cout << "send test data to dns server: " << result << std::endl; 
    
        
        socklen_t len = 0;
        int n = recvfrom(dnsListener, (char *)buffer, 512,  
                0, (struct sockaddr *) &dnsHint, 
                &len); 
        std::cout << "buff data: %s"  << buffer << std::endl;
        std::cout << "retrieve successful? " << std::endl; 
    }


};