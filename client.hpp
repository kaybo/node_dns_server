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

class Client
{
    public:
        Client(const char* serverAddr, int port, void* data);

        void createSocket();
        void shutDown();

    private:
        int sock;
}