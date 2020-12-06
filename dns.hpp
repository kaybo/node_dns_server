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
#include "dns_struct.hpp"
#include "utility.hpp"


DECODED_RESPONSE *dnsSendQuery(std::string domainName, std::string destinationIPAddress);

void dnsLookUp();