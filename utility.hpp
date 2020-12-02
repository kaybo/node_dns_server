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
std::string convertDNSFieldToHostName(unsigned char field);

//Encodes the dnsquery and returns an encoded array ready to be sent over udp to other servers
unsigned char *encodeDNSQuery(std::string domainName, HEADER dnsHeader, QUESTION dnsQuestion);

//decodes the data from the buffer
//note: refer to RFC section 5 for message compression
void decodeDNSRespond(unsigned char *buf);