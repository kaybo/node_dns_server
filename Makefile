
dns_client: dns_client.o client_handler.o
	g++ dns_client.o client_handler.o -o client

dns_client.o: dns_client.cpp
	g++ -c dns_client.cpp

client_handler.o:
	g++ -c client_handler.cpp

clean:
	rm *.o client 