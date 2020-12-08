//author: Feng Wu
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
            resourceRecordList.erase(i++);  
        }
        else
        {
            tempRR.ttl -= value;
            ++i;
        }
    }
    for(RESOURCE_RECORD rr : resourceRecordList){
        std::string tempRRName = convertSequenceLabelToHostName(rr.rrName);
        std::cout << "**Inside Cache** " << "Domain name: "<< tempRRName << " ttl: "<<rr.ttl <<std::endl;
    }
};

RESOURCE_RECORD network::LocalResourceRecordCache::getResourceRecordByName(std::string name){
    RESOURCE_RECORD *return_record;
    int size = resourceRecordList.size();
    if(size > 0){
        for(RESOURCE_RECORD rr : resourceRecordList){
            std::string tempRRName = convertSequenceLabelToHostName(rr.rrName);
            if(name.compare(tempRRName) == 0){
                return rr;
            }
        }
    }
    return_record = new RESOURCE_RECORD;
    return_record->ttl = 0;
    return *return_record;
};

int network::LocalResourceRecordCache::addResourceRecord(RESOURCE_RECORD rr){
    //if duplicate exist, update it
    std::string insertRRName = convertSequenceLabelToHostName(rr.rrName);
    std::list<RESOURCE_RECORD>::iterator i = resourceRecordList.begin();
    while (i != resourceRecordList.end())
    {
        RESOURCE_RECORD tempRR = *i;
        std::string tempRRName = convertSequenceLabelToHostName(tempRR.rrName);

        if (tempRRName.compare(insertRRName) == 0)
        {
            resourceRecordList.erase(i++);  
        }
        else
        {
            ++i;
        }
    }
    if(cacheSize <= resourceRecordList.size()){
        resourceRecordList.pop_front();
    }
    resourceRecordList.push_back(rr);
    return 0;

};