/**
 * @file Server side code for meal time socket programming project
 * @authors Phuc T, Narissa T, Kristen K
 * @date 2/6/22
 * @version 1.0
 * @reference https://www.geeksforgeeks.org/socket-programming-cc/

 */

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#define PORT 10328  // Per spec. 

int parseBuffer(char* buffer) {
	// This function will parse the buffer, and figure out what RPC it is (e
	// .g. connect, disconnect). Based on the type of rpc it is, it will try
	// to unravel the string that was passed and create paramters from it.

	// "RPC=CONNECT;USERNAME=mike;PASSWORD=mikepass"

	// Parse the first part of the buffer, the type of RPC.

	switch (RPC)
	{
	case CONNECT:
		connect(username, password)
			break;
	case DISCONNECT:
		break;
	default:
		break;
	}
}

int connect(char* username, char* password) {
	//Connect will be the fxn I process on the server onceI figure out what
	// type of rpc it is, it will return 0 for success or -1 failure

	// tell the client we successfully made the connection,
	// the client would unravel the buffer
	sendRPC(status);
}

int main(int argc, char const *argv[])
{



	return 0;
}