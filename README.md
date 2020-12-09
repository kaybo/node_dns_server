# CMPT 471 Final Project

## Requisites:

1)Must run on Ubuntu. I have not tested on other distros

2)The host cannot be connected to a shaw network/router. Please use a different network. For
my testing, I used Freedom mobile network.

3)Try not to use SFU csil computers, DNS queries will get blocked if you perform DNS queries
multiple times within a short time frame.

##Setup and Installation:

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
./client
Afterwards,  it will ask you to specify the IP address, port and domain name.
For IP address, please use 127.0.0.1 and the same port as the server.

## Issues, Bugs

1) Server only accepts one client at a time. Do not send more than 1 request at a time.

2) Server may go into a deadlock because packets are sent over UDP when sending/receiving
DNS queries. This scenario is rare.

3)Try not to use SFU csil computers, DNS queries will get blocked if you perform DNS queries
multiple times within a short time frame.

4)Do not use Shaw network/Routers. DNS queries made by the server gets intercepted and will not received the correct information.

5) Server and client must run on the same host. Server is default configured to run on localhost