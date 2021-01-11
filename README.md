# DNS Server Implementation

## Note:
This project has been forked from my previous repo https://github.com/kaybo/cmpt471_project.
The server and algorithm has been rewritten in Javascript to resolve many of the bugs and issues
in the previous version.
The dns query is still written in C++ and has not been modified.

## Requisites:

1) Must run on Ubuntu. I have not tested on other distros

2) The host cannot be connected to a shaw network/router. Please use a different network. For
my testing, I used Freedom mobile network.

## Setup and Installation:

1) git clone the repo

2) go into the directory and run the command:

```
npm install
```
3) run this command after installing all the required modules

```
node-gyp build
```

4) after building the C++ binaries, this command will start up the server
```
node index.js
```
The server should now be up and running.

## Usage

Replace InserDomainNameHere with the specify domain. Note that this server was ran locally.

```
localhost:8081/finddomain/InsertDomainNameHere
```

An IP will be returned if domain name has been found. Otherwise, it will return the value 'unknown'.


## Issues and Bugs


1) Server may freeze because packets are sent over UDP when sending/receiving
DNS queries. This scenario is rare. Note that reliable transfer is not implementated. 

2) Do not use Shaw network/Routers. DNS queries made by the server gets intercepted and will not received the correct information.

3) Executing too many DNS lookup may result in TLD, authoritative DNS servers blocking your IP which results in freezing on the local DNS server.

4) DNS header transaction ID is kept as 1337 for simplicity.

