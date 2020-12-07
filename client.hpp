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
#include "utility.hpp"

namespace network{
    class Client
    {
        public:

            //Constructor
            Client(std::string inputServerIpAddr ,int inputPort, std::string userInputDomainName);
            
            //Creates socket and setups any configurations related to the socket
            void createSocket();

            //establishes and connects to the server
            void connectToServer();

            //closes connection
            void shutDown();
            

        private:
            int port;
            int sock;
            std::string serverIpAddr;
            std::string domainName;
            sockaddr_in hint;

            void setup();

    };
}
