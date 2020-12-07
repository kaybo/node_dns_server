//Author: Feng Wu

#include "server.hpp"
#include "root_servers.hpp"
#include <errno.h>
#include "utility.hpp"
#include <bitset>
#include "dns.hpp"


network::Server::Server(int inputPort){
    answerRRList = new LocalResourceRecordCache(5);
    // performAction();//remove later
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

    socklen_t clientSize = sizeof(client);
    unsigned char buf[512];
    unsigned char sendBuf[512];
    std::string domainName;
    while(true){
        clientSock = accept(listener, (sockaddr*)&client, &clientSize);
        int readingFromClient = read( clientSock , buf, 512);
        int index = 0;
        while(buf[index] != '\0'){
            std::cout << "server debug: " << buf[index] <<std::endl;
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

    std::string searchingDomainName = domainName;
    unsigned char *returnArray = new unsigned char [5];

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
        std::cout << std::endl << "<><><><><>New Query<><><><><>" << std::endl << std::endl;
        if(query->authNameServer.size() > 0 && query->additional.size() == 0){
            // std::cout << std::endl << "<><><><><>if statement<><><><><>" << std::endl << std::endl;
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
                    std::cout << std::endl << "<><><><><>ayayayyaayay<><><><><>" << std::endl << std::endl;
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
                            // std::cout << std::endl << "<><><><><>lala<><><><><>" << std::endl << std::endl;
                            std::cout << tempName << " " << tempNameServer << std::endl;
                            if(tempName.compare(tempNameServer) == 0){
                                tempAdditionalRR = additionalRR;
                                // std::cout << "BREAKING SON!" << std::endl;
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
                // std::cout << std::endl << "<><><><><>else, else if<><><><><>" << std::endl << std::endl;
                RESOURCE_RECORD tempAdditionalRR;
                for(RESOURCE_RECORD authRR: query->authNameServer){
                    
                    // RESOURCE_RECORD tempAuthRR = authRR;
                    std::string tempNameServer;
                    tempNameServer = convertSequenceLabelToHostName(authRR.rdata);
                    
                    for(RESOURCE_RECORD additionalRR: query->additional){
                        std::string tempName = convertSequenceLabelToHostName(authRR.rdata);
                        // std::cout << std::endl << "<><><><><>lala<><><><><>" << std::endl << std::endl;
                        std::cout << tempName << " " << tempNameServer << std::endl;
                        if(tempName.compare(tempNameServer) == 0){
                            tempAdditionalRR = additionalRR;
                            // std::cout << "BREAKING SON!" << std::endl;
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
        std::cout << "**********$@_$*#@()%*#@)(%*)@#%@#%#@%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%%" <<std::endl;
        
        query = dnsSendQuery(selectedDomainName, destAddress);
        printDecodedResponse(*query);
        RESOURCE_RECORD tempAnswerRR;
        for(RESOURCE_RECORD answerRR: query->answer){
            tempAnswerRR = answerRR;
            break;
        }
        if(tempAnswerRR.rrType == CNAME){
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
        memcpy(returnArray+1, ansRR.rdata, 4);
        memset(returnArray, 0, 1);
    }
    return returnArray;
    
};