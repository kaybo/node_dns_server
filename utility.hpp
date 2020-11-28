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

//converts string hostname into DNS format field 
//note: allocates memory on heap for the returned unsigned char array
//example use case:
//string contains "www.test.com" becomes "3www4test3com0" 
//also another note: 0 is same as \0
unsigned char *convertHostNameToDNSField(std::string inputString);