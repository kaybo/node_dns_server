//Author: Feng Wu

//Specifies the structure used for DNS queries
//Refer to RFC 1034, 1035 for all the structures

//Refer to section 4.1.1 RFC 1035
struct HEADER{

    //unique id for the specific dns header
    unsigned short id;

    //indicates type of message, 0: query, 1: response
    unsigned char qr :1;

    //specifies kind of query in message. I believe it should be set
    //to 0 as default
    unsigned char opcode: 4;

    //Authoritative answer
    unsigned char aa: 1;

    //truncation
    unsigned char tc: 1;

    //recursion desired
    unsigned char rd: 1;

    //recursion available
    unsigned char ra: 1;

    //best not to touch this
    unsigned char z: 3;

    //response code(values from 0 to 5, 6-15 is reserved)
    unsigned char rcode: 4;

    //# of entries in question section
    unsigned short qdcount;

    //#of rr in answer section
    unsigned short ancount;

    //# of ns rr in authority records section
    unsigned short nscount;

    //# of rr in additional records section
    unsigned short arcount;

};

//Refer to section 4.1.2 RFC 1035
struct QUESTION{
    //NOTE: not sure if qname is in correct format
    //query name
    unsigned char *qname;

    //query type
    unsigned short qtype;

    //query class
    unsigned short qclass;

};

//Refer to section 4.1.3 RFC 1035
struct RESOURCE_RECORD{
    
    //domain name of the resource record
    unsigned char *rrName;

    //resource record type
    unsigned short rrType;

    //specifies data in rdata field
    unsigned short rrClass;

    //time to live in seconds
    unsigned int ttl;

    //length of rdata field
    unsigned short rdlength;

    //NOTE: not sure if this is correct either
    unsigned char *rdata;

};
