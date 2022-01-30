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
//		connect(username, password)
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
	// server socket init
	int server_fd;    // server's listening socket
	int new_socket;   // server's accept socket for client interaction
	int valread;      // value of new_socket after read

	// socket address init
	struct sockaddr_in address;            // server socket address struct
	address.sin_family = AF_INET;          // server socket's address format is INET for IP4
	address.sin_addr.s_addr = INADDR_ANY;  // server socket is bound to LOCAL HOST
	address.sin_port = htons(PORT);        // assign server's port and convert to network bytes



	return 0;
}