//Author: Feng Wu

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

//TODO: Implement this!
std::string convertDNSFieldToHostName(unsigned char field){
    return "test";
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


    for(int index = 0; index < decodedHeader->ancount; index++){
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
        pointerOffSet += sizeof(short);
        *compressMsgPointer = ntohs(*compressMsgPointer);
        
        if(*compressMsgPointer >> 14 == 0x3){
            std::cout << "Message compression occurs!" << std::endl;
            unsigned short offSetValue;
            offSetValue = *compressMsgPointer;
            //removing the first two bits inside the pointer
            offSetValue = offSetValue << 2;
            offSetValue = offSetValue >> 2;
            *compressMsgPointer = offSetValue;
            pointerOffSet += 2;//note: assuming message compression offset is always 2 bytes
        }else{
            //Note: Not sure if msg compression does not occur, leaving this condition
            //      in just in case if it is needed
            std::cout << "Message compression does not occur!" << std::endl;
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
            // std::cout << buf[nameOffsetTracker] << std::endl;
            name[inner_index] = buf[nameOffsetTracker];
            nameOffsetTracker++;
        }

        //TODO: CHECK IF CONTENT IS ACUTALLY IN NAME ARRAY
        // std::cout << "name from msg compression: " << *name << std::endl;
        int testIndex = 0;
        while(name[testIndex] != '\0'){
            std::cout << "trace: " << name[testIndex] << std::endl;
            testIndex++;
        }
        //TODO: implement answer RR?
        TEMP_RESOURCE_RECORD *tempRR = new TEMP_RESOURCE_RECORD();
        memcpy(tempRR, buf+pointerOffSet, sizeof(TEMP_RESOURCE_RECORD));
        pointerOffSet += sizeof(TEMP_RESOURCE_RECORD);
        std::cout << "debug: " << ntohs(tempRR->ttl) << std::endl;
        //TODO: CHECK RDATA TO SEE HOW IT IS FORMATTED


    


        
        //TODO: FREE compressMsgpointer after getting the name and storing it somewhere
        delete compressMsgPointer;        
    }

};