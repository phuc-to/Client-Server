
 // Server side C/C++ program to demonstrate Socket programming
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>  // data types for socket addresses
#include <string.h>
#define PORT 8080


// Functions
// bind
// getsockname
int main(int argc, char const *argv[])
{
	// server_fd = socket
	int server_fd, new_socket, valread;

	// sockaddr_in is the data type to represent INET socket addresses, a
	// pointer to a socket address
	struct sockaddr_in address;  // init socket address var

	int opt = 1;
	int addrlen = sizeof(address);
	char buffer[1024] = { 0 };
	char *hello = "Hello from server";

	// Creating socket file descriptor
	// AF_INET = address format for IPv4 internet namespace
	// SOCK_STREAM = socket communication style that operatoes over a
	// connection with one remote socket and transmits data as a stream of bytes
	if ((server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
	{
		perror("socket failed");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port 8080
	if (setsockopt(server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
		&opt, sizeof(opt)))
	{
		perror("setsockopt");
		exit(EXIT_FAILURE);
	}
	address.sin_family = AF_INET;

	// Construct a socket address
	// sin_addr = IPv4 address as an uint32_t
	// INADDR_ANY = constant stands for "any incoming address" when binding
	// to an address. This is the normal address to give to sin_addr in order
	// to accept Internet connections
	address.sin_addr.s_addr = INADDR_ANY;
	address.sin_port = htons(PORT);

	// cast a pointer to struct sockaddr * when calling bind, attaching
	// socket to port
	if (bind(server_fd, (struct sockaddr *)&address,
		sizeof(address)) < 0)
	{
		perror("bind failed");
		exit(EXIT_FAILURE);
	}
	// listen enables the socket server_fd to accept connections, n = length
	// of queue for pending connections, when full new client attempts to
	// connect fail
	if (listen(server_fd, 3) < 0)  // if listen returns -1
	{
		perror("listen");
		exit(EXIT_FAILURE);
	}
	// accept completes the connection request. The server's original socket,
	// server_fd does not become part of the connection, instead accept makes
	// a new socket to complete the request while server_fd continues to
	// listen for other requests
	if ((new_socket = accept(server_fd, (struct sockaddr *)&address,
		(socklen_t*)&addrlen)) < 0)
	{
		perror("accept");
		exit(EXIT_FAILURE);
	}


	valread = read(new_socket, buffer, 1024);
	printf("%s\n", buffer);

	// send
	// inputs: new socket, buffer, length of the buffer, flags
	send(new_socket, hello, strlen(hello), 0);
	printf("Hello message sent\n");
	return 0;
}
