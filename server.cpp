//Author: Feng Wu

#include "server.hpp"
#include "root_servers.hpp"
#include <errno.h>
#include "utility.hpp"
#include <bitset>
#include "dns.hpp"
#include <mutex>
#include "cache.hpp"
#include <unistd.h>
#include <thread>


network::LocalResourceRecordCache ipCache = network::LocalResourceRecordCache(5);
std::mutex ipCacheMutex;

void runTimer(){
    while(1){
        ipCacheMutex.lock();
        std::cout << "running timer thread, inside critical section" << std::endl;
        ipCache.decrementTTLValues(5);
        ipCacheMutex.unlock();
        sleep(5);
    }
};

network::Server::Server(int inputPort){
    answerRRList = new LocalResourceRecordCache(3);
    servPort = inputPort;
    createSocket();
    initializeListener();

    
};

void network::Server::createSocket(){
    listener = socket(AF_INET, SOCK_STREAM, 0);
    if(listener == -1){
        std::cout << "unable to initialize listener socket" << std::endl;
    }else{
        hint.sin_family = AF_INET;
        hint.sin_port = htons(servPort);
        //Todo: Need to change this to support local and other IP addresses
        inet_pton(AF_INET, "127.0.0.1", &hint.sin_addr); 

        //binding socket
        if(bind(listener, (sockaddr*)&hint, sizeof(hint)) == -1){
            std::cout << "Bind failed" << std::endl;
        }else{
            //creating socket listener
            if(listen(listener, SOMAXCONN) == -1){
                std::cout << "listen failed" << std::endl;
            }else{
                std::cout << "createSocket() succeeded" << std::endl;
            }
        }
    }
};

void network::Server::initializeListener(){
    std::thread loopThread(runTimer);
    // loopThread.join();

    socklen_t clientSize = sizeof(client);
    unsigned char buf[512];
    unsigned char sendBuf[512];
    std::string domainName;
    while(true){
        clientSock = accept(listener, (sockaddr*)&client, &clientSize);
        int readingFromClient = read( clientSock , buf, 512);
        int index = 0;
        while(buf[index] != '\0'){
            domainName += buf[index];
            index++;
        }
        unsigned char *res = performAction(domainName);
        send(clientSock, res, 5, 0);
        domainName = "";
        close(clientSock);
    }
    
};

unsigned char* network::Server::performAction(std::string domainName){
    unsigned char *returnArray = new unsigned char [5];

    //search in cache first
    ipCacheMutex.lock();
    RESOURCE_RECORD cacheRR = ipCache.getResourceRecordByName(domainName);
    ipCacheMutex.unlock();
    
    std::cout << "><><>< Looking domain name inside cache...><><><" << std::endl;
    if(cacheRR.ttl != 0){
        std::cout << "><><>< Found Resource Record with the domain name inside cache! ><><><" << std::endl;
        //need to perform mutex lock
        memset(returnArray, 0, 1);
        ipCacheMutex.lock();
        ipCache.addResourceRecord(cacheRR);
        ipCacheMutex.unlock();
        memcpy(returnArray+1, cacheRR.rdata, 4);
        return returnArray;
    }else{
        std::cout << "><><>< Unable to find domain name inside cache, executing iterative DNS query... ><><><" << std::endl;
    }
    

    std::string searchingDomainName = domainName;
    

cNameLabel:
    
    DECODED_RESPONSE *query;
    // printDecodedResponse(*query);

    //replicating do while loop
    std::string destAddress = A_ROOT;
    std::string selectedDomainName = searchingDomainName;
    query = dnsSendQuery(selectedDomainName, destAddress);
    printDecodedResponse(*query);
    std::string resolvedAdress;
    int flag = 0;
    int matchingTempNameAndNameServer = 0;

    if(query->answer.size() == 0 && query->additional.size() == 0 && query->authNameServer.size() == 0
        && flag == 0){
        memset(returnArray, 0, 5);
        return returnArray;
    }

    while(query->answer.size() == 0 || flag == 1){
        if(query->answer.size() == 0 && query->additional.size() == 0 && query->authNameServer.size() == 0){
            memset(returnArray, 0, 5);
            return returnArray;
        }
        if(query->authNameServer.size() > 0 && query->additional.size() == 0){
            if(resolvedAdress.empty()){
                flag = 1;
                RESOURCE_RECORD tempRR;
                for(RESOURCE_RECORD authRR: query->authNameServer){
                    tempRR = authRR;
                    break;
                }
                selectedDomainName = convertSequenceLabelToHostName(tempRR.rdata);
                destAddress = A_ROOT;
            }else{
                destAddress = resolvedAdress;
                selectedDomainName = searchingDomainName;
            }
            
        }else{
            
            if(flag == 1){
                if(query->answer.size() > 0){
                    flag = 0;
                    
                    for(RESOURCE_RECORD answerRR: query->answer){
                        resolvedAdress = convertUnsignedCharToIPAdress(answerRR.rdata);
                        break;
                    }
                    selectedDomainName = searchingDomainName;
                }else{
                    RESOURCE_RECORD tempAdditionalRR;
                    for(RESOURCE_RECORD authRR: query->authNameServer){
                        
                        // RESOURCE_RECORD tempAuthRR = authRR;
                        std::string tempNameServer;
                        tempNameServer = convertSequenceLabelToHostName(authRR.rdata);
                        for(RESOURCE_RECORD additionalRR: query->additional){
                            std::string tempName = convertSequenceLabelToHostName(authRR.rdata);
                            if(tempName.compare(tempNameServer) == 0){
                                tempAdditionalRR = additionalRR;
                                matchingTempNameAndNameServer = 1;
                                break;
                            }
                        }
                        if(matchingTempNameAndNameServer == 1){
                            break;
                        }

                    }
                    destAddress = convertUnsignedCharToIPAdress(tempAdditionalRR.rdata);
                }
            }else if(flag == 0){
                RESOURCE_RECORD tempAdditionalRR;
                for(RESOURCE_RECORD authRR: query->authNameServer){
                    std::string tempNameServer;
                    tempNameServer = convertSequenceLabelToHostName(authRR.rdata);
                    for(RESOURCE_RECORD additionalRR: query->additional){
                        std::string tempName = convertSequenceLabelToHostName(authRR.rdata);
                        if(tempName.compare(tempNameServer) == 0){
                            tempAdditionalRR = additionalRR;
                            matchingTempNameAndNameServer = 1;
                            break;
                        }
                    }
                    if(matchingTempNameAndNameServer == 1){
                        break;
                    }

                }
                destAddress = convertUnsignedCharToIPAdress(tempAdditionalRR.rdata);
            }
            matchingTempNameAndNameServer == 0;
        }
        
        query = dnsSendQuery(selectedDomainName, destAddress);
        printDecodedResponse(*query);
        RESOURCE_RECORD tempAnswerRR;
        for(RESOURCE_RECORD answerRR: query->answer){
            tempAnswerRR = answerRR;
            break;
        }
        if(tempAnswerRR.rrType == CNAME){
            std::string rrCName = convertSequenceLabelToHostName(tempAnswerRR.rdata);
            //search in cache first
            ipCacheMutex.lock();
            RESOURCE_RECORD cacheCNAMERR = ipCache.getResourceRecordByName(rrCName);
            ipCacheMutex.unlock();
            
            std::cout << "><><>< Looking CNAME inside cache...><><><" << std::endl;
            if(cacheCNAMERR.ttl != 0){
                std::cout << "><><>< Found Resource Record with the CNAME inside cache! ><><><" << std::endl;
                //need to perform mutex lock
                memset(returnArray, 0, 1);
                ipCacheMutex.lock();
                ipCache.addResourceRecord(cacheCNAMERR);
                ipCacheMutex.unlock();
                memcpy(returnArray+1, cacheCNAMERR.rdata, 4);
                return returnArray;
            }else{
                std::cout << "><><>< Unable to find CNAME inside cache, executing iterative DNS query... ><><><" << std::endl;
            }

            searchingDomainName = convertSequenceLabelToHostName(tempAnswerRR.rdata);
            goto cNameLabel;
        }
        
    }

    

    printDecodedResponse(*query);
    if(query->answer.size() == 0){
        memset(returnArray, 0, 5);
    }else if(query->answer.size() > 0){
        //todo: add cache byte, 0 indicates not retrieve from cache
        RESOURCE_RECORD ansRR = query->answer.front();
        //need to perform mutex lock
        memset(returnArray, 0, 1);
        ipCacheMutex.lock();
        ipCache.addResourceRecord(ansRR);
        ipCacheMutex.unlock();
        memcpy(returnArray+1, ansRR.rdata, 4);
    }
    return returnArray;
    
};