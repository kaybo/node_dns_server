#include "cache.hpp"

network::LocalResourceRecordCache::LocalResourceRecordCache(int size){
    std::cout << "LocalResourceRecordCache class created" << std::endl;
    cacheSize = size;
   
};

int network::LocalResourceRecordCache::addResourceRecord(RESOURCE_RECORD rr){
    //if duplicate exist, update it
    std::cout<< "ADDReSOURECRECORD" << std::endl;
    std::string insertRRName = convertSequenceLabelToHostName(rr.rrName);
    std::list<RESOURCE_RECORD>::iterator i = resourceRecordList.begin();
    while (i != resourceRecordList.end())
    {
        bool isActive = false;
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