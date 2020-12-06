//Author: Feng Wu

#include <cstdio>
#include <cstdlib>
#include <cstring>
#include <string>
#include <unistd.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <netinet/in.h> //sockaddr, socklen_t
#include <arpa/inet.h>
#include <netdb.h>
#include <iostream>
#include <bitset>
#include "dns_struct.hpp"


//converts string hostname into DNS format field 
//note: allocates memory on heap for the returned unsigned char array
//example use case:
//string contains "www.test.com" becomes "3www4test3com0" 
//also another note: 0 is same as \0
unsigned char *convertHostNameToDNSField(std::string inputString);

//converts DNS format field back to string hostname
//example use case:
//string contains "3www4test3com0"  becomes "www.test.com"
std::string convertSequenceLabelToHostName(unsigned char *field);

//converts char array to readable ip address in string format
std::string convertUnsignedCharToIPAdress(unsigned char *data);

//prints all the content inside decoded response struct
void printDecodedResponse(DECODED_RESPONSE res);

//prints all content inside resource record
void printResourceRecord(RESOURCE_RECORD rr);

//print the header
void printHeader(HEADER head);

//print res_question struct
void printResQuestion(RES_QUESTION resQ);

//Special case usage for uncompressing domain names inside decodeDNSResponse()
//Must not use in else where
unsigned char *messageDecompression(unsigned char *buf, unsigned char *nameServerDomain, unsigned short length);

//Encodes the dnsquery and returns an encoded array ready to be sent over udp to other servers
unsigned char *encodeDNSQuery(std::string domainName, HEADER dnsHeader, QUESTION dnsQuestion);

//decodes the data from the buffer
//note: refer to RFC section 5 for message compression
DECODED_RESPONSE *decodeDNSRespond(unsigned char *buf);