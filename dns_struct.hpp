//Author: Feng Wu

//Specifies the structure used for DNS queries
//Refer to RFC 1034, 1035 for all the structures

//Refer to section 4.1.1 RFC 1035
struct HEADER{

    //unique id for the specific dns header
    unsigned short id;

    //WARNING: super important to rearrange the bits field in opposite order so
    //the network knows how to identify it. htons does not work for this specific
    //scenario so each field must be manually adjusted. One of the reason to
    //why packet malform keeps occuring

    //Re-arranged these 8 bits first

    //recursion desired
    unsigned char rd: 1;

    //truncation
    unsigned char tc: 1;

    //Authoritative answer
    unsigned char aa: 1;

    //specifies kind of query in message. I believe it should be set
    //to 0 as default
    unsigned char opcode: 4;

    //indicates type of message, 0: query, 1: response
    unsigned char qr :1;

    //Re-arranged these 8 bits second

    //response code(values from 0 to 5, 6-15 is reserved)
    unsigned char rcode: 4;

    //checking disabled(new stuff that's not in RFC1035)
    unsigned char cd: 1;

    //authenticated data(new stuff that's not in RFC1035)
    unsigned char ad: 1;

    //best not to touch this
    unsigned char z: 1;

    //recursion available
    unsigned char ra: 1;

    //WARNING: MUST USE HTON ON THESE FOUR FIELDS

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
//Note: Use RES_QUESTION if it is a dns response query.
//      Request query use this only!!!
struct QUESTION{
    //query type
    unsigned short qtype;

    //query class
    unsigned short qclass;

};

//Refer to section 4.1.2 RFC 1035
//used only for decoding, formatting and putting it on local server
//WARNING: Use QUESTION struct for dns request queries
struct RES_QUESTION{
    //query name
    unsigned char *qname;

    //query type
    unsigned short qtype;

    //query class
    unsigned short qclass;

};



//Refer to section 4.1.3 RFC 1035
//WARNING: Used only for decoding, use other one for actual RR 
struct TEMP_RESOURCE_RECORD{
    
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