#include "client.hpp"

void Client::createSocket(){
    sock = socket(AF_INET, SOCK_STREAM, 0);
};

