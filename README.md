# CMPT 471 Final Project

## Requisites:

1) Must run on Ubuntu. I have not tested on other distros

2) The host cannot be connected to a shaw network/router. Please use a different network. For
my testing, I used Freedom mobile network.

3) Try not to use SFU csil computers, DNS queries will get blocked if you perform DNS queries
multiple times within a short time frame.

## Setup and Installation:

1) git clone the repo

2) go into the directory and run the command:

```
make
```

Two executables should be created called client and server.

## Running the executables

1) To run the server
```
./server
```
Afterwards, it will ask you to specify which port to connect it to. I would use port 1337 or
any ports that are free.

2) To run the client
```
./client
```
Afterwards,  it will ask you to specify the IP address, port and domain name.
For IP address, please use 127.0.0.1 and the same port as the server.

## Issues and Bugs

1) Server only accepts one client at a time. Do not send more than 1 request at a time.

2) Server may freeze because packets are sent over UDP when sending/receiving
DNS queries. This scenario is rare. Note that reliable transfer is not implementated. 

3) Try not to use SFU csil computers, DNS queries will get blocked if you perform DNS queries
multiple times within a short time frame.

4) Do not use Shaw network/Routers. DNS queries made by the server gets intercepted and will not received the correct information.

5) Server and client must run on the same host. Server is default configured to run on localhost

6) Executing too many DNS lookup may result in TLD, authoritative DNS servers blocking your IP which results in freezing on the local DNS server.

7) After exiting the server, the server does not shut down successfully so the port may still be
in used. Need to use a different port number to solve this issue.

8) Possibility of memory leaks.

9) DNS header transaction ID is kept as 1337 for simplicity.

## Testing

### Test case 1: Non-existing domain name

Domain name: 

```
thisdomainnamedoesnotexist.com
```

Client input log
```
Enter the DNS server IP address that you want to connect to:
127.0.0.1
Enter the DNS server port number that you want to connect to:
1335
Enter the domain name that you want to resolve: 
thisdomainnamedoesnotexist.com
```

Client output result
```
createSocket() succeeded
connectToServer() succeeded
thisdomainnamedoesnotexist.com
Domain name not found
```

Server input log
```
Enter the DNS server port number that you want to connect to:
1335
LocalResourceRecordCache class created
createSocket() succeeded
Cache thread running...
```

Server output result
```
Cache thread running...
><><>< Looking domain name inside cache...><><><
><><>< Unable to find domain name inside cache, executing iterative DNS query... ><><><

=========Print Decoded Response=========
*******Header Section********
Transaction ID: 1337
Query/Response: 1
Opcode: 0000
Authoritative Answer: 0
Truncated: 1
Recursion Desired: 0
Recursion Available: 0
Z: 0
Authenticated Data: 0
Checking Disabled: 0
Rcode: 0000
Question Count: 1
Answer Resource Records: 0
Authority Resource Records: 13
Additional Resource Records: 6
****************************
******Question Section*******
Question Name: thisdomainnamedoesnotexist.com
Query Type: 1
Query Class: 1
****************************

...

=========Print Decoded Response=========
*******Header Section********
Transaction ID: 1337
Query/Response: 1
Opcode: 0000
Authoritative Answer: 1
Truncated: 0
Recursion Desired: 0
Recursion Available: 0
Z: 0
Authenticated Data: 0
Checking Disabled: 0
Rcode: 0011
Question Count: 1
Answer Resource Records: 0
Authority Resource Records: 0
Additional Resource Records: 0
****************************
******Question Section*******
Question Name: thisdomainnamedoesnotexist.com
Query Type: 1
Query Class: 1
****************************
*******Answer Section*******
*********************************
********Authority Section*******
***********************************
*******Additional Section*********
**********************************
=========================================

```


### Test case 2: existing domain name
** Note ** : The IP addresses in the log may be different from the ones that you will retrieve.

domain name: 

```
google.com
```

Client input log
```
Enter the DNS server IP address that you want to connect to:
127.0.0.1
Enter the DNS server port number that you want to connect to:
1336
Enter the domain name that you want to resolve: 
thisdomainnamedoesnotexist.com
```

Client output result
```
createSocket() succeeded
connectToServer() succeeded
google.com
ip address: 172.217.3.174
```

Server input log
```
Enter the DNS server port number that you want to connect to:
1336
LocalResourceRecordCache class created
createSocket() succeeded
Cache thread running...
```

Server output result
```
Cache thread running...
><><>< Looking domain name inside cache...><><><
><><>< Unable to find domain name inside cache, executing iterative DNS query... ><><><


=========Print Decoded Response=========
*******Header Section********
Transaction ID: 1337
Query/Response: 1
Opcode: 0000
Authoritative Answer: 0
Truncated: 0
Recursion Desired: 0
Recursion Available: 0
Z: 0
Authenticated Data: 0
Checking Disabled: 0
Rcode: 0000
Question Count: 1
Answer Resource Records: 0
Authority Resource Records: 13
Additional Resource Records: 13
****************************
******Question Section*******
Question Name: google.com
Query Type: 1
Query Class: 1
****************************
*******Answer Section*******
*********************************

...

=========Print Decoded Response=========
*******Header Section********
Transaction ID: 1337
Query/Response: 1
Opcode: 0000
Authoritative Answer: 1
Truncated: 0
Recursion Desired: 0
Recursion Available: 0
Z: 0
Authenticated Data: 0
Checking Disabled: 0
Rcode: 0000
Question Count: 1
Answer Resource Records: 1
Authority Resource Records: 0
Additional Resource Records: 0
****************************
******Question Section*******
Question Name: google.com
Query Type: 1
Query Class: 1
****************************
*******Answer Section*******
.......Resource Record.......
Name: google.com
Type: 1
Class: 1
Time to live: 300 (seconds)
Data Length: 4
IP Adress: 172.217.3.174
..............................
*********************************
********Authority Section*******
***********************************
*******Additional Section*********
**********************************
=========================================

Cache thread running...
**Inside Cache** Domain name: google.com ttl: 295
Cache thread running...
**Inside Cache** Domain name: google.com ttl: 290


```

### Test case 3: Retrieving Resource Record from cache
This test case will build on top of test case 2. Do not exit the server. If you do,
restart test case 2 with a different port number.

domain name: 

```
google.com
```

Client input log
```
Enter the DNS server IP address that you want to connect to:
127.0.0.1
Enter the DNS server port number that you want to connect to:
1336
Enter the domain name that you want to resolve: 
thisdomainnamedoesnotexist.com
```

Client output result
```
createSocket() succeeded
connectToServer() succeeded
google.com
ip address: 172.217.3.174
```

Server output result
```
Cache thread running...
**Inside Cache** Domain name: google.com ttl: 295
Cache thread running...
**Inside Cache** Domain name: google.com ttl: 290
><><>< Looking domain name inside cache...><><><
><><>< Found Resource Record with the domain name inside cache! ><><><
Cache thread running...
**Inside Cache** Domain name: google.com ttl: 285
Cache thread running...
**Inside Cache** Domain name: google.com ttl: 280
```