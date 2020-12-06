//Author: Feng Wu

#include "dns.hpp"


DECODED_RESPONSE *dnsSendQuery(std::string domainName, std::string destinationIPAddress){
    HEADER head = HEADER();
    head.id = htons(1337);
    head.qr = 0x0;
    head.tc = 0x0;
    head.rd = 0x0;
    head.qdcount = htons(1);
    head.ancount = htons(0);
    head.nscount = htons(0);
    head.arcount = htons(0);

    QUESTION question = QUESTION();
    question.qtype = htons(1);
    question.qclass = htons(1);

    unsigned char *convertedDomainName = convertHostNameToDNSField(domainName);
    unsigned char *encodedInfo = encodeDNSQuery(domainName, head, question);

    int totalSize = sizeof(head) 
    + strlen((const char*)convertedDomainName) + 1 + sizeof(question.qclass) + sizeof(question.qtype);

    int dnsPort = 53;
    int dnsListener = socket(AF_INET, SOCK_DGRAM, 0);
    sockaddr_in dnsHint;
    if(dnsListener == -1){
        std::cout << "unable to initialize dnsLisener" << std::endl;
        DECODED_RESPONSE *res = new DECODED_RESPONSE();
        return res;
    }else{
        dnsHint.sin_family = AF_INET;
        dnsHint.sin_port = htons(dnsPort);
        inet_pton(AF_INET, destinationIPAddress.c_str(), &dnsHint.sin_addr);
        
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

        DECODED_RESPONSE *res = decodeDNSRespond(buffer);
        // printDecodedResponse(*testRes);
        return res;
    }

};

void dnsLookUp(){

};

