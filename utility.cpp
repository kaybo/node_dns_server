//Author: Feng Wu

#include "utility.hpp"
#include "constants.hpp"


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

//TODO: Implement this!
std::string convertDNSFieldToHostName(unsigned char field){
    return "test";
};

//TODO: IMPLEMENT THIS!
void messageDecompression(unsigned char *buf, unsigned char *nameServerDomain, unsigned short length){
    //loop thru nameServerDomain
    //if not message compression
    //add to list
    //if is message compression
    //expand and add into list
    //after iterating, iterate list again and turn it back into unsigned char array then array
    //that array
    // std::cout << "=======EXECUTED===========: "<< std::endl;
    std::list<unsigned char> uncompressedList;
    unsigned short index = 0;
    while(index < length){
        unsigned char currentChar = nameServerDomain[index];
        std::cout << "=======currentchar===========: "<< currentChar<<" "<<std::bitset<8>(currentChar) <<std::endl;
        if(currentChar >> 6 == 0x3){
            unsigned short offSetValue;
            memcpy(&offSetValue, nameServerDomain+index, 2);
            offSetValue = ntohs(offSetValue);
            offSetValue = offSetValue << 2;
            offSetValue = offSetValue >> 2;
            std::cout << "=======messageDecompression===========: "<< offSetValue << " " << std::bitset<16>(offSetValue)<< std::endl;
            // unsigned short bufIndex = 0;
            while(buf[offSetValue] != '\0'){
                std::cout << "=======while loop:===========: "<< buf[offSetValue] << " " << std::bitset<8>(buf[offSetValue])<< std::endl;
                uncompressedList.push_back(buf[offSetValue]);
                offSetValue++;
            }
            index ++;
            index ++;
        }else{
            uncompressedList.push_back(currentChar);
            index++;
        }
    }
    for(unsigned char t:uncompressedList){
        std::cout << "============new lis===========: " << t << std::endl;
    }


};

//TODO: do htons instead of letting user doing it
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

void decodeDNSRespond(unsigned char *buf){
    std::cout << "Decoding function running" << std::endl;
    DECODED_RESPONSE *returnInformation = new DECODED_RESPONSE();

    int pointerOffSet = 0;

    HEADER *decodedHeader = new HEADER();
    memcpy(decodedHeader, buf, sizeof(*decodedHeader));
    pointerOffSet += sizeof(*decodedHeader);

    //converting edians to host format
    decodedHeader->id = ntohs(decodedHeader->id);
    decodedHeader->qdcount = ntohs(decodedHeader->qdcount);
    decodedHeader->ancount = ntohs(decodedHeader->ancount);
    decodedHeader->nscount = ntohs(decodedHeader->nscount);
    decodedHeader->arcount = ntohs(decodedHeader->arcount);

    //decoding questions section

    //getting qname
    int fieldLength = 0;
    int tempPointerOffSet = pointerOffSet; //used for temp tracing
    while(buf[tempPointerOffSet] != '\0'){
        fieldLength++;
        tempPointerOffSet++;
    }
    fieldLength++;

    unsigned char *qname = new unsigned char[fieldLength];
    for(int index = 0; index < fieldLength ; index++){
        std::cout << buf[pointerOffSet] << std::endl;
        qname[index] = buf[pointerOffSet];
        pointerOffSet++;
    }

    QUESTION *decodedQuestion = new QUESTION();
    memcpy(decodedQuestion, buf + pointerOffSet, sizeof(*decodedQuestion));

    decodedQuestion->qtype = ntohs(decodedQuestion->qtype);
    decodedQuestion->qclass = ntohs(decodedQuestion->qclass);
    pointerOffSet += sizeof(*decodedQuestion);

    //RES_QUESTION used for putting it back into local server
    RES_QUESTION *resDecodedQuestion = new RES_QUESTION();
    resDecodedQuestion->qname = qname;
    resDecodedQuestion->qclass = decodedQuestion->qclass;
    resDecodedQuestion->qtype = decodedQuestion->qtype;
    delete decodedQuestion;

    std::cout << "before forloop execution pointeroffset: " << pointerOffSet << std::endl;
    
    //appending head and question before decoding RRs
    returnInformation->head = *decodedHeader;
    returnInformation->question = *decodedQuestion;

    std::list<RESOURCE_RECORD> *decodedAnswerList = new std::list<RESOURCE_RECORD>();
    std::list<RESOURCE_RECORD> *decodedAuthNameServerList = new std::list<RESOURCE_RECORD>();
    std::list<RESOURCE_RECORD> *decodedAdditionalList = new std::list<RESOURCE_RECORD>();

    int totalRRCount = decodedHeader->ancount + decodedHeader->nscount + decodedHeader->arcount;

    int answerCount = returnInformation->head.ancount;
    int nsCount = returnInformation->head.nscount;
    int additionalCount = returnInformation->head.arcount;

    //debuging code here
    int debug = 2;
    //================

    for(int index = 0; index < totalRRCount; index++){
        //resource record name
        //TODO: IMPLEMENT THIS

        //NOTE: THIS ONLY WORKS IF MESSAGE COMPRESSION OCCURS, I'M NOT SURE IF THIS WORKS ON OTHER FORMATS
        //NEED TO CHECK IF NAME != QANME IN OTHER ANSWERS
        //1)Get two octet (2bytes) from the buffer
        //2)only 14bits(ignore the firs two bits) and store it in var offset
        //3)Use offset to get the name
        //4)Continue decoding and store all the data in the RR structure

        unsigned short *compressMsgPointer = new unsigned short; //technically a offset, not really a pointer
        memcpy(compressMsgPointer, buf+pointerOffSet, sizeof(short));
        pointerOffSet += sizeof(unsigned short);//===TODO===: might need to change this later due to msg compresssion feature
        *compressMsgPointer = ntohs(*compressMsgPointer);
        
        if(*compressMsgPointer >> 14 == 0x3){//This is incorrect I think?
            std::cout << "Message compression occurs!" << std::endl;
            unsigned short offSetValue;
            offSetValue = *compressMsgPointer;
            //removing the first two bits inside the pointer
            offSetValue = offSetValue << 2;
            offSetValue = offSetValue >> 2;
            *compressMsgPointer = offSetValue;
        }else{
            //Note: Not sure if msg compression does not occur, leaving this condition
            //      in just in case if it is needed
            std::cout << "Message compression does =========not======= occur!" << std::endl;
        }
        //retrieving name field with compressMsgPointer
        int tempLength = 0;
        int tempOffSetTracker = *compressMsgPointer; //used for temp tracing
        int nameOffsetTracker = *compressMsgPointer;//used for iterating the name
        while(buf[tempOffSetTracker] != '\0'){
            tempLength++;
            tempOffSetTracker++;
        }
        tempLength++;

        unsigned char *name = new unsigned char[tempLength];
        for(int inner_index = 0; inner_index < tempLength ; inner_index++){
            name[inner_index] = buf[nameOffsetTracker];
            nameOffsetTracker++;
        }

        //CHECKING IF CONTENT IS ACUTALLY IN NAME ARRAY
        int testIndex = 0;
        while(name[testIndex] != '\0'){
            std::cout << "trace: " << name[testIndex] << std::endl;
            testIndex++;
        }
        std::cout << "pointeroffset before TMEP_RESOURCE_RECORD: " << pointerOffSet << std::endl;
        //TODO: implement answer RR?
        TEMP_RESOURCE_RECORD *tempRR = new TEMP_RESOURCE_RECORD();
        memcpy(tempRR, buf+pointerOffSet, sizeof(TEMP_RESOURCE_RECORD));
        tempRR->rrClass = ntohs(tempRR->rrClass);
        tempRR->rrType = ntohs(tempRR->rrType);
        tempRR->ttl = ntohl(tempRR->ttl);
        tempRR->rdlength = ntohs(tempRR->rdlength);
 
        pointerOffSet += 10;//warning: could not use sizeof(TEMP_RESOURCE_RECORD) due to padding issues
        std::cout << "debug rrType: " << tempRR->rrType<< std::endl;
        std::cout << "debug rrClass: " << tempRR->rrClass  << std::endl;
        std::cout << "debug ttl: " << tempRR->ttl <<std::endl;
        std::cout << "debug rdlength: " << tempRR->rdlength << " "<<std::bitset<16>(tempRR->rdlength) << std::endl;


        RESOURCE_RECORD decodedRR;
        //TODO: add rrName later once message compression has all been figured out(Added)
        decodedRR.rrName = name; //NOTE: this might not work, need to test this first!!!

        decodedRR.rrType = tempRR->rrType;
        decodedRR.rrClass = tempRR->rrClass;
        decodedRR.ttl = tempRR->ttl;
        decodedRR.rdlength = tempRR->rdlength;

        delete tempRR;

        //TODO: check which type
        //Note: Ignores all IPV6 address because this is a IPV4 implementation
        std::cout <<"pointeroffset before checking type:  " << pointerOffSet << std::endl;
        if(decodedRR.rrType == A){
            std::cout << "RR type A" << std::endl;
            unsigned char *ipAddress = new unsigned char[4];
            memcpy(ipAddress, buf + pointerOffSet, decodedRR.rdlength);

            for(int test = 0 ; test < decodedRR.rdlength ; test++){
                std::cout << "debug NS: " << ipAddress[test] << " " <<std::bitset<8>(ipAddress[test]) << std::endl;
            }
            decodedRR.rdata = ipAddress;
        }else if(decodedRR.rrType == NS){
            std::cout << "RR type NS" << std::endl;
            unsigned char *nameServerDomain = new unsigned char [decodedRR.rdlength];
            memcpy(nameServerDomain, buf + pointerOffSet, decodedRR.rdlength);
            for(int test = 0 ; test < decodedRR.rdlength ; test++){
                std::cout << "debug NS: " << nameServerDomain[test] << " " <<std::bitset<8>(nameServerDomain[test]) << std::endl;
            }
            messageDecompression(buf, nameServerDomain, decodedRR.rdlength);
            //TODO: need to add message compression to find the actual NS domain before storing it
            //      into rdata
        }else if(decodedRR.rrType == CNAME){
            std::cout << "RR type CNAME" << std::endl;
        }else if(decodedRR.rrType == SOA){
            std::cout << "RR type SOA" << std::endl;
        }else if(decodedRR.rrType == PTR){
            std::cout << "RR type PTR" << std::endl;
        }else{
            std::cout << "none of the RR type has been matched" << std::endl;
            //TODO: decrement the specific count fields because of ignoring them
        }
        pointerOffSet += decodedRR.rdlength;


        if(answerCount > 0){
            decodedAnswerList->push_back(decodedRR);
            answerCount--;
        }else if(nsCount > 0){
            decodedAuthNameServerList->push_back(decodedRR);
            nsCount--;
        }else if(additionalCount > 0){
            decodedAdditionalList->push_back(decodedRR);
            additionalCount--;
        }
        delete compressMsgPointer;   
        std::cout << "end of debug for iteration" << std::endl;
  
        if(debug < index){
            break; 
        }
          
    }

            for (RESOURCE_RECORD n : *decodedAnswerList) {
                std::cout << n.ttl << '\n';
            }
    returnInformation->answer = *decodedAnswerList;
    returnInformation->authNameServer = *decodedAuthNameServerList;
    returnInformation->additional = *decodedAnswerList;
    // return returnInformation;

};

