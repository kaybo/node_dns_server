#include "cache.hpp"

network::LocalResourceRecordCache::LocalResourceRecordCache(int size){
    std::cout << "LocalResourceRecordCache class created" << std::endl;
    cacheSize = size;
   
};

int network::LocalResourceRecordCache::addResourceRecord(RESOURCE_RECORD rr){
    //if duplicate exist, update it
    std::cout<< "ADDReSOURECRECORD" << std::endl;
    // std::string recordName = convertSequenceLabelToHostName(rr.rrName);
    // for (auto i = resourceRecordList.begin(); i != resourceRecordList.end();) {
    //     // std::string tempRecordName = convertSequenceLabelToHostName(i.rrName);
    //     if (recordName != i.){
    //         i = resourceRecordList.erase(i);
    //     }else{
    //         ++i;
    //     }    
    // }
};