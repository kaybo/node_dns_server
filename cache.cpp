#include "cache.hpp"

network::LocalResourceRecordCache::LocalResourceRecordCache(int size){
    std::cout << "LocalResourceRecordCache class created" << std::endl;
    cacheSize = size;
   
};

void network::LocalResourceRecordCache::decrementTTLValues(int value){
    std::list<RESOURCE_RECORD>::iterator i = resourceRecordList.begin();
    while (i != resourceRecordList.end())
    {
        RESOURCE_RECORD &tempRR = *i;
        std::string tempRRName = convertSequenceLabelToHostName(tempRR.rrName);
        int convertedTTL= tempRR.ttl;
        if ((convertedTTL-value) <= 0)
        {
            resourceRecordList.erase(i++);  // alternatively, i = items.erase(i);
        }
        else
        {
            tempRR.ttl -= value;
            ++i;
        }
    }
    for(RESOURCE_RECORD rr : resourceRecordList){
   
        std::cout << "ttl: " << rr.ttl << std::endl;
    }
};

RESOURCE_RECORD network::LocalResourceRecordCache::getResourceRecordByName(std::string name){
    std::cout << "getResourceRecordByName ran0"  <<std::endl;
    RESOURCE_RECORD *return_record;
    std::cout << "getResourceRecordByName ran1"  <<std::endl;
    int size = resourceRecordList.size();
    std::cout << "getResourceRecordByName ran2"  <<std::endl;
    if(size > 0){
        std::cout << "getResourceRecordByName ran3"  <<std::endl;
        for(RESOURCE_RECORD rr : resourceRecordList){
            std::cout << "getResourceRecordByName ran4"  <<std::endl;
            std::string tempRRName = convertSequenceLabelToHostName(rr.rrName);
            if(name.compare(tempRRName) == 0){
                return rr;
            }
            std::cout << "debug: " << rr.ttl <<std::endl;
        }
    }
    std::cout << "getResourceRecordByName ran5"  <<std::endl;
    return_record = new RESOURCE_RECORD;
    return_record->ttl = 0;
    return *return_record;
};

int network::LocalResourceRecordCache::addResourceRecord(RESOURCE_RECORD rr){
    //if duplicate exist, update it
    std::cout<< "ADDReSOURECRECORD" << std::endl;
    std::string insertRRName = convertSequenceLabelToHostName(rr.rrName);
    std::list<RESOURCE_RECORD>::iterator i = resourceRecordList.begin();
    while (i != resourceRecordList.end())
    {
        RESOURCE_RECORD tempRR = *i;
        std::string tempRRName = convertSequenceLabelToHostName(tempRR.rrName);

        if (tempRRName.compare(insertRRName) == 0)
        {
            resourceRecordList.erase(i++);  // alternatively, i = items.erase(i);
        }
        else
        {
            // other_code_involving(*i);
            ++i;
        }
    }
    if(cacheSize <= resourceRecordList.size()){
        resourceRecordList.pop_front();
    }
    resourceRecordList.push_back(rr);


    for(RESOURCE_RECORD rr : resourceRecordList){
        std::string tempRRName = convertSequenceLabelToHostName(rr.rrName);
        std::cout << "name: " << tempRRName << std::endl;
    }
};