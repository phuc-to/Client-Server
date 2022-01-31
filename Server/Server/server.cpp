/**
 * @file Server side code for meal time socket programming project
 * @authors Phuc T, Narissa T, Kristen K
 * @date 2/6/22
 * @version 1.0
 * @reference https://www.geeksforgeeks.org/socket-programming-cc/

 */

#include <unistd.h>
#include <iostream>       // C++ IO
#include <stdio.h>        // C IO
#include <sys/socket.h>   // UNIX socket structs
#include <stdlib.h>
#include <netinet/in.h>   // Constants and structs for INTERNET domain addresses  
#include <string.h>       
#define PORT 10328        // CS1 port per program specs. 

using namespace std;

// TODO: Implement parseBuffer to delimit the string buffer into RPC and inputs.
//int parseBuffer(char* buffer) {
//	// This function will parse the buffer, and figure out what RPC it is (e
//	// .g. connect, disconnect). Based on the type of rpc it is, it will try
//	// to unravel the string that was passed and create paramters from it.
//
//	// "RPC=CONNECT;USERNAME=mike;PASSWORD=mikepass"
//
//	// Parse the first part of the buffer, the type of RPC.
//
//	switch (RPC)
//	{
//	case CONNECT:
//		loginRPC(username, password)
//			break;
//	case DISCONNECT:
//		break;
//	default:
//		break;
//	}
//}

// TODO: Implement log in functionility for connect
//int connect(char* username, char* password) {
//
//	sendRPC(status);
//}



int main(int argc, char const *argv[])
{
	// START SERVER SOCKET INIT //////////////////////////////////////////////////////////////

	// server socket and buffer init 
	const int NAMESPACE = AF_INET;    // internet namespace IP4 
	const int STYLE = SOCK_STREAM;    // comm style for TCP programming
	const int PROTOCOL = 0;           // 0 for internet 
	const int BUFF_SIZE = 1024;       // default buffer size
	const int QUEUE_SIZE = 3;  // number of connection requests in queue before error is thrown

	char buffer[BUFF_SIZE] = { 0 };  // init empty character buffer 
	int serv_socket;    // server's listening socket
	int new_socket;   // server's accept socket for client interaction
	int valread;      

	// server socket address init
	struct sockaddr_in address;             // server socket address struct
	address.sin_family = NAMESPACE;         // server socket's address format is INET for IP4
	address.sin_addr.s_addr = INADDR_ANY;  // server socket is bound to LOCAL HOST
	address.sin_port = htons(PORT);        // assign server's port and convert to network bytes

	// create server socket, exit if error creating socket, else print success msg
	if ((serv_socket = socket(NAMESPACE, STYLE, PROTOCOL)) < 0) {
		perror("\nServer: Socket creation error\n");
		exit(EXIT_FAILURE);
	}
	else
		cout << "\nServer: Socket creation successful\n";

	// setup options to reuse address and port with setsockopt function
	// setsockopt function: setsockopt(int socket, int level, int optname, int optval, socklen_t optlen)
	const int LEVEL = SOL_SOCKET; // holds socket level options to reuse address and port
	const int OPTNAMES = SO_REUSEADDR | SO_REUSEADDR; 
	const int OPTVAL = 1; 

	if (setsockopt(serv_socket, LEVEL, OPTNAMES, &OPTVAL, sizeof(OPTVAL))) {
		perror("\nServer: Error setting up socket options\n");
		exit(EXIT_FAILURE);
	}
	else
		cout << "\nServer: Socket options set\n";

	// use bind to assign address to server socket
	// bind function: bind(int socket, struct sockaddr*address, soscklen_t length)
	if (bind(serv_socket, (struct sockaddr *)&address, sizeof(address)) < 0) {
		perror("\nServer: Error binding socket to address\n");
		exit(EXIT_FAILURE);
	}
	else
		cout << "\nServer: Socket binding successful\n";

	// enable connection requests on the server socket
	// listen function: listen(int socket, int number of connection requests allowed in queue)
	if (listen(serv_socket, QUEUE_SIZE) < 0) {
		perror("\nServer: Error listenting\n");
		exit(EXIT_FAILURE);
	}
	else
		cout << "\nServer: Server socket listening...\n";

	// accept incoming connection request by creating a new socket to host the connection
	// accept function: accept(int socket, struct sockakddr* address, socklen_t*length of address)
	if ((new_socket = accept(new_socket, (struct sockaddr *)&address,(socklen_t*)&(sizeof(address)))) < 0) {
		perror("\nServer: Error creating new socket\n");
		exit(EXIT_FAILURE);
	}
	else
		cout << "\nServer: Connection request rec'd...\n";

	// read msg from client and print
	// read function: read(int socket, buffer, buffer size)
	valread = read(new_socket, buffer, BUFF_SIZE);
	cout << "Server: " << buffer << "\n";

	// send msg to client
	const char *testMsg = "Testing from the server!";
	send(new_socket, testMsg, strlen(testMsg), 0);


	// END SERVER SOCKET INIT //////////////////////////////////////////////////////////////


	// RPC stuff....
	////....
	//....




	return 0;
}