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

namespace network{
    class Server
    {
        public:

            //Constructor
            Server(int inputPort);
            
            //Creates listener socket and setups any configurations related to the listener socket
            void createSocket();

            //makes server listen to incoming request from clients
            void initializeListener();
     
            //performs sequence of actions related to DNS before sending response
            //back to the client
            void performAction();
            

        private:
            int servPort;
            sockaddr_in hint;
            int listener;

            sockaddr_in client;
            int clientSock;


    };
}
