//author: Feng Wu
#ifndef CACHE_H
#define CACHE_H

#include <iostream>
#include <list>
#include "dns_struct.hpp"
#include "utility.hpp"


namespace network{
    class LocalResourceRecordCache
    {
        public:
            LocalResourceRecordCache(int size);

            //decrements the ttl values in the whole list by value
            void decrementTTLValues(int value);

            //gets resource record by domain name, if ttl is less than 1, does not return the RR
            RESOURCE_RECORD *getResourceRecordByName(std::string name);
            
            //returns 0 if successful, 1 if error, also checks for duplicates
            int addResourceRecord(RESOURCE_RECORD rr);

            //removes resource record by domain name and returns it
            RESOURCE_RECORD removeResourceRecord(std::string name);

        private:
        // sem_t *readWriteMutex;
        int cacheSize;
        std::list<RESOURCE_RECORD> resourceRecordList;
    };
}

#endif