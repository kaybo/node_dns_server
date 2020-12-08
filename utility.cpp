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

std::string convertUnsignedCharToIPAdress(unsigned char *data){
    //always 4 octet, working with IPv4 protocol
    std::string returnValue = "";
    for(int index = 0 ; index < 4 ; index++){
        returnValue += std::to_string(data[index]);
        if(index != 3){
            returnValue += ".";
        }
    }
    return returnValue;
};


std::string convertSequenceLabelToHostName(unsigned char *field){
    unsigned short index = 0;
    unsigned short dotIndex = 0;
    std::string returnString = "";
    while(field[index] != '\0'){
        if(index == 0){
            dotIndex = field[index] + 1;
        }else{
            if(dotIndex == index){
                returnString += ".";
                dotIndex = field[index] + index + 1;
            }else{
                returnString += field[index];
            }
        }
        index++;
    }
    return returnString;
};

void printDecodedResponse(DECODED_RESPONSE res){
    std::cout << std::endl << "=========Print Decoded Response=========" << std::endl;
    std::cout << "*******Header Section********" << std::endl;
    printHeader(res.head);
    std::cout << "****************************" << std::endl;
    std::cout << "******Question Section*******" << std::endl;
    printResQuestion(res.question);
    std::cout << "****************************" << std::endl;
    std::cout << "*******Answer Section*******" << std::endl;
    for(RESOURCE_RECORD rr: res.answer){
        printResourceRecord(rr);
    }
    std::cout << "*********************************" << std::endl;
    std::cout << "********Authority Section*******" << std::endl;
    for(RESOURCE_RECORD rr: res.authNameServer){
        printResourceRecord(rr);
    }
    std::cout << "***********************************" << std::endl;
    std::cout << "*******Additional Section*********" << std::endl;
    for(RESOURCE_RECORD rr: res.additional){
        printResourceRecord(rr);
    }
    std::cout << "**********************************" << std::endl;
    std::cout << "=========================================" << std::endl << std::endl;
};

void printResourceRecord(RESOURCE_RECORD rr){
    std::cout << ".......Resource Record......." << std::endl;
    std::string convertedRRName = convertSequenceLabelToHostName(rr.rrName);
    std::cout << "Name: " << convertedRRName << std::endl;
    std::cout << "Type: " << rr.rrType << std::endl; 
    std::cout << "Class: " << rr.rrClass << std::endl;
    std::cout << "Time to live: " << rr.ttl << " (seconds)" << std::endl;
    std::cout << "Data Length: " << rr.rdlength <<std::endl;

    if(rr.rrType == A){
        std::string ipAdress = convertUnsignedCharToIPAdress(rr.rdata);
        std::cout << "IP Adress: " << ipAdress << std::endl;
    }else if(rr.rrType == NS){
        std::string convertedData = convertSequenceLabelToHostName(rr.rdata);
        std::cout << "Name Server: " << convertedData << std::endl;
    }else if(rr.rrType == CNAME){
        std::string convertedData = convertSequenceLabelToHostName(rr.rdata);
        std::cout << "Canonical Name: " << convertedData << std::endl;     
    }else{
        std::cout << "unformatted data" << std::endl;
    }
    std::cout << ".............................." << std::endl;

};

void printResQuestion(RES_QUESTION resQ){
    std::string convertedQName = convertSequenceLabelToHostName(resQ.qname);
    std::cout << "Question Name: " << convertedQName << std::endl;
    std::cout << "Query Type: " << resQ.qtype << std::endl;
    std::cout << "Query Class: " << resQ.qclass << std::endl;
};

void printHeader(HEADER head){
    std::cout << "Transaction ID: " << head.id << std::endl;
    std::cout << "Query/Response: " << std::bitset<1>(head.qr) << std::endl;
    std::cout << "Opcode: " << std::bitset<4>(head.opcode) << std::endl;
    std::cout << "Authoritative Answer: " << std::bitset<1>(head.aa) << std::endl;
    std::cout << "Truncated: " << std::bitset<1>(head.tc) << std::endl;
    std::cout << "Recursion Desired: " << std::bitset<1>(head.rd) << std::endl;
    std::cout << "Recursion Available: " << std::bitset<1>(head.ra) << std::endl;
    std::cout << "Z: " << std::bitset<1>(head.z) << std::endl;
    std::cout << "Authenticated Data: " << std::bitset<1>(head.ad) << std::endl;
    std::cout << "Checking Disabled: " << std::bitset<1>(head.cd) << std::endl;
    std::cout << "Rcode: " << std::bitset<4>(head.rcode) << std::endl;
    std::cout << "Question Count: " << head.qdcount << std::endl;
    std::cout << "Answer Resource Records: " << head.ancount << std::endl;
    std::cout << "Authority Resource Records: " << head.nscount << std::endl;
    std::cout << "Additional Resource Records: " << head.arcount << std::endl;

    
};

unsigned char *messageDecompression(unsigned char *buf, unsigned char *nameServerDomain, unsigned short length){
    //thought process: message compression flag occurs at the end of the char array
    //=>so that means it will then contain the pointer/offset to the actual data somewhere in the buffer
    //however, when I point to the previous buffer, it might also contain a message compression flag
    //basically, when I see a message compression flag, go to that offset and decompress
    //HOWEVER, is there a possibility that there are additional data after the message compression flag???
    std::list<unsigned char> uncompressedList;
    unsigned short index = 0;
    while(index < length){
        unsigned char currentChar = nameServerDomain[index];
        if(currentChar >> 6 == 0x3){
            unsigned short offSetValue;
            memcpy(&offSetValue, nameServerDomain+index, 2);
            offSetValue = ntohs(offSetValue);
            offSetValue = offSetValue << 2;
            offSetValue = offSetValue >> 2;
            while(buf[offSetValue] != '\0'){
                if(buf[offSetValue] >> 6 == 0x3){
                    int temp = offSetValue;
                    memcpy(&offSetValue, buf+temp, 2);
                    offSetValue = ntohs(offSetValue);
                    offSetValue = offSetValue << 2;
                    offSetValue = offSetValue >> 2;
                }
                uncompressedList.push_back(buf[offSetValue]);
                offSetValue++;
                
            }
            // index ++;
            // index ++;
            //sometimes, there are additional message compression pointer after another
            //Implementation already has taken care of it so no need to continue decompressing.
            //Otherwise, it'll be duplicates, index+=2 is not needed unless problems occur later.
            //Then refactor will be needed in this particular section.
            break;
        }else{
            uncompressedList.push_back(currentChar);
            index++;
        }
    }
    uncompressedList.push_back('\0');
    unsigned char *returnArray = new unsigned char[uncompressedList.size()]; 
    int returnArrayIndex = 0;
    for(unsigned char data:uncompressedList){
        returnArray[returnArrayIndex] = data;
        returnArrayIndex++;
    }
    return returnArray;
};


unsigned char *encodeDNSQuery(std::string domainName, HEADER dnsHeader, QUESTION dnsQuestion){

    unsigned char *convertedDomainName = convertHostNameToDNSField(domainName);

    int totalSize = 12
    + strlen((const char*)convertedDomainName) + 1 + sizeof(dnsQuestion.qclass) + sizeof(dnsQuestion.qtype);

    //encoded array
    unsigned char *encodedInfo = new unsigned char[totalSize];
    memset(encodedInfo, 0 ,totalSize);

    int pointerOffSet = 0;

    //encoding header struct
    memcpy(encodedInfo, &dnsHeader, sizeof(dnsHeader));
    pointerOffSet += 12;

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

DECODED_RESPONSE *decodeDNSRespond(unsigned char *buf){
    // std::cout << "Decoding function running" << std::endl;
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
        qname[index] = buf[pointerOffSet];
        pointerOffSet++;
    }

    QUESTION *decodedQuestion = new QUESTION();
    memcpy(decodedQuestion, buf + pointerOffSet, 4);

    decodedQuestion->qtype = ntohs(decodedQuestion->qtype);
    decodedQuestion->qclass = ntohs(decodedQuestion->qclass);
    pointerOffSet += 4;

    //RES_QUESTION used for putting it back into local server
    RES_QUESTION *resDecodedQuestion = new RES_QUESTION();
    resDecodedQuestion->qname = qname;
    resDecodedQuestion->qclass = decodedQuestion->qclass;
    resDecodedQuestion->qtype = decodedQuestion->qtype;
    delete decodedQuestion;
    //appending head and question before decoding RRs
    returnInformation->head = *decodedHeader;
    returnInformation->question = *resDecodedQuestion;

    std::list<RESOURCE_RECORD> *decodedAnswerList = new std::list<RESOURCE_RECORD>();
    std::list<RESOURCE_RECORD> *decodedAuthNameServerList = new std::list<RESOURCE_RECORD>();
    std::list<RESOURCE_RECORD> *decodedAdditionalList = new std::list<RESOURCE_RECORD>();

    int totalRRCount = decodedHeader->ancount + decodedHeader->nscount + decodedHeader->arcount;

    int answerCount = returnInformation->head.ancount;
    int nsCount = returnInformation->head.nscount;
    int additionalCount = returnInformation->head.arcount;


    for(int index = 0; index < totalRRCount; index++){
        //NOTE: THIS ONLY WORKS IF MESSAGE COMPRESSION OCCURS, I'M NOT SURE IF THIS WORKS ON OTHER FORMATS
        //NEED TO CHECK IF NAME != QANME IN OTHER ANSWERS
        //1)Get two octet (2bytes) from the buffer
        //2)only 14bits(ignore the firs two bits) and store it in var offset
        //3)Use offset to get the name
        //4)Continue decoding and store all the data in the RR structure

        unsigned short *compressMsgPointer = new unsigned short; //technically a offset, not really a pointer
        memcpy(compressMsgPointer, buf+pointerOffSet, sizeof(short));
        
        *compressMsgPointer = ntohs(*compressMsgPointer);
        
        unsigned char *name;


        if(*compressMsgPointer >> 14 == 0x3){
            unsigned short offSetValue;
            offSetValue = *compressMsgPointer;
            //removing the first two bits inside the pointer
            offSetValue = offSetValue << 2;
            offSetValue = offSetValue >> 2;
            *compressMsgPointer = offSetValue;

            //retrieving name field with compressMsgPointer
            int tempLength = 0;
            int tempOffSetTracker = *compressMsgPointer; //used for temp tracing
            int nameOffsetTracker = *compressMsgPointer;//used for iterating the name
            while(buf[tempOffSetTracker] != '\0'){
                tempLength++;
                tempOffSetTracker++;
            }
            tempLength++;

            unsigned char *tempName = new unsigned char[tempLength];
            for(int inner_index = 0; inner_index < tempLength ; inner_index++){
                tempName[inner_index] = buf[nameOffsetTracker];
                nameOffsetTracker++;
            }
            name = messageDecompression(buf, tempName, tempLength);
            pointerOffSet += sizeof(unsigned short);


        }else{
            std::cout << "Message compression does =========not======= occur!" << std::endl;
            //Note: Not sure if msg compression does not occur, leaving this condition
            //      in just in case if it is needed
            int tempIndex = pointerOffSet;
            int tempLength = 0;
            while(buf[tempIndex] != '\0'){
                std::cout << "@@@@@@@@@@@@@@@@@@@@@@@@@@@@UNCOMPRESS DEBUG: " << buf[tempIndex] <<" " << std::bitset<8>(buf[tempIndex])<<std::endl;
                tempIndex++;
                tempLength++;
            }
            unsigned char *tempName = new unsigned char[tempLength+1];
            int uncompressedIndex = 0;
            while(uncompressedIndex < tempLength){
                tempName[uncompressedIndex] = buf[pointerOffSet];
                pointerOffSet++;
                uncompressedIndex++;
            }
            tempName[uncompressedIndex] = '\0';
            name = tempName;
            std::cout << "TEMP LENGTH@@@@@@@@@@@@@@@@@@@@@@@: " << tempLength << " " <<pointerOffSet<< std::endl;
            pointerOffSet ++;
            
        }


        
  
        TEMP_RESOURCE_RECORD *tempRR = new TEMP_RESOURCE_RECORD();
        memcpy(tempRR, buf+pointerOffSet, sizeof(TEMP_RESOURCE_RECORD));
        tempRR->rrClass = ntohs(tempRR->rrClass);
        tempRR->rrType = ntohs(tempRR->rrType);
        tempRR->ttl = ntohl(tempRR->ttl);
        tempRR->rdlength = ntohs(tempRR->rdlength);
 
        pointerOffSet += 10;


        RESOURCE_RECORD decodedRR;
     
        decodedRR.rrName = name; 

        decodedRR.rrType = tempRR->rrType;
        decodedRR.rrClass = tempRR->rrClass;
        decodedRR.ttl = tempRR->ttl;
        decodedRR.rdlength = tempRR->rdlength;

        delete tempRR;

        //Note: Ignores all IPV6 address because this is a IPV4 implementation
        if(decodedRR.rrType == A){
            // std::cout << "RR type A" << std::endl;
            unsigned char *ipAddress = new unsigned char[4];
            memcpy(ipAddress, buf + pointerOffSet, decodedRR.rdlength);

            decodedRR.rdata = ipAddress;
        }else if(decodedRR.rrType == NS){
            unsigned char *nameServerDomain = new unsigned char [decodedRR.rdlength];
            memcpy(nameServerDomain, buf + pointerOffSet, decodedRR.rdlength);
            unsigned char* decompressedNameServerDomain = messageDecompression(buf, nameServerDomain, decodedRR.rdlength);
            decodedRR.rdata = decompressedNameServerDomain;
        }else if(decodedRR.rrType == CNAME){
            std::cout << "RR type CNAME" << std::endl;
            unsigned char *cName = new unsigned char[decodedRR.rdlength];
            memcpy(cName, buf+pointerOffSet, decodedRR.rdlength);
            unsigned char* decompressedCName = messageDecompression(buf, cName, decodedRR.rdlength);
            decodedRR.rdata = decompressedCName;
        }else if(decodedRR.rrType == SOA){
            std::cout << "RR type SOA" << std::endl;
        }else if(decodedRR.rrType == PTR){
            std::cout << "RR type PTR" << std::endl;
        }else{
            std::cout << "none of the RR type has been matched" << std::endl;
        }
        pointerOffSet += decodedRR.rdlength;


        if(answerCount > 0){
            decodedAnswerList->push_back(decodedRR);
            answerCount--;
        }else if(nsCount > 0){
            if(decodedRR.rrType != SOA){
                decodedAuthNameServerList->push_back(decodedRR);
            }else{
                returnInformation->head.nscount--;
            }
            nsCount--;
        }else if(additionalCount > 0){
            if(decodedRR.rrType != AAA){
                decodedAdditionalList->push_back(decodedRR);
            }else{
                returnInformation->head.arcount--;
            }
            additionalCount--;
        }
        delete compressMsgPointer;         
    }

    returnInformation->answer = *decodedAnswerList;
    returnInformation->authNameServer = *decodedAuthNameServerList;
    returnInformation->additional = *decodedAdditionalList;
    return returnInformation;

};

