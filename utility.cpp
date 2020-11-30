#include "utility.hpp"



unsigned char *convertHostNameToDNSField(std::string inputString){

    int newSize = inputString.length() + 2;
    unsigned char *conversionField = new unsigned char[newSize];

    int index = 0;
    int newIndex = 1;
    uint8_t charCount = 0; //counts the character before '.'
    int dotIndex = 0;//replaces '.' with the byte value
    while(index < inputString.length()){
        if(inputString[index] == '.'){
            conversionField[dotIndex] = charCount;
            dotIndex = newIndex;
            charCount = 0;
        }
        if(inputString[index] != '.'){
            conversionField[newIndex] = inputString[index];
            charCount++;
        }
        newIndex++;
        index++;
    }
    conversionField[dotIndex] = charCount;
    conversionField[newIndex] = '\0';

    return conversionField;
};

unsigned char *encodeDNSQuery(std::string domainName, HEADER dnsHeader, QUESTION dnsQuestion){

    unsigned char *convertedDomainName = convertHostNameToDNSField(domainName);

    int totalSize = sizeof(dnsHeader) 
    + strlen((const char*)convertedDomainName) + 1 + sizeof(dnsQuestion.qclass) + sizeof(dnsQuestion.qtype);

    //encoded array
    unsigned char *encodedInfo = new unsigned char[totalSize];
    memset(encodedInfo, 0 ,totalSize);

    int pointerOffSet = 0;

    //encoding header struct
    memcpy(encodedInfo, &dnsHeader, sizeof(dnsHeader));
    pointerOffSet += sizeof(dnsHeader);

    //encoding domain name
    int index = 0;
    while(convertedDomainName[index] != '\0'){
        encodedInfo[pointerOffSet] = convertedDomainName[index];
        index++;
        pointerOffSet++;
    }
    pointerOffSet++;//for the \0 character

    //encoding question struct
    memcpy(encodedInfo+pointerOffSet, &dnsQuestion, sizeof(dnsQuestion));
    
    delete convertedDomainName;

    return encodedInfo;
};