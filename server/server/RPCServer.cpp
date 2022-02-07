/**
 @file  Server side class file for MealTime socket programming project
 @authors Phuc T, Narissa T, Kristen K
 @date 2/8/22
 @version 1.0
 @reference https://www.geeksforgeeks.org/socket-programming-cc/
 @reference https://www.gnu.org/software/libc/manual/html_node/Sockets.html
  
 GNU C functions 
 setsockopt function: setsockopt(int socket, int level, int optname, int optval, socklen_t optlen)
 bind function: bind(int socket, struct sockaddr*address, socklen_t length)
 listen function: listen(int socket, int number of connection requests allowed in queue)
 read function: read(int socket, buffer, buffer size)
 */


#include "RPCServer.h"

#include <unistd.h>
#include <iostream>     // For C++ IO.
#include <stdio.h>
#include <sys/socket.h> // For UNIX socket structs.
#include <stdlib.h>
#include <netinet/in.h> // For onstants and structs for INTERNET domain addresses.
#include <string.h>
#include <vector>
#include <iterator>


//#define PORT 8081     // For local testing only 

using namespace std;

// Constructor.
RPCServer::RPCServer(const char *serverIP, int port)
{
	m_rpcCount = 0;
	m_serverIP = (char *)serverIP;
	m_port = port;
};

// Destructor.
RPCServer::~RPCServer() {};


// Creates server file descriptpr on port passed by user.
bool RPCServer::StartServer()
{
	// New socket constants 
	const int OPTVAL = 1;               // setsockopt option value
	const int BACKLOG = 10;             // Number of allowed connection requests before error thrown
	const int NAMESPACE = AF_INET;      // Internet namespace IP4 
	const int COMM_STYLE = SOCK_STREAM; // Communication style for TCP programming
	const int PROTOCOL = 1;             // 1 for TCP IPv4

	// setsockopt constants
	const int LEVEL = SOL_SOCKET;       
	const int OPTNAMES = SO_REUSEADDR | SO_REUSEADDR;  // Socket level options to reuse address and port

	// server socket address init
	// server socket address struct
	// parameter for accept fxn
	struct sockaddr_in m_address;                      // server socket address struct
	m_address.sin_family = NAMESPACE;                  // server socket's address format is INET for IP4
	m_address.sin_addr.s_addr = INADDR_ANY;            // Server socket is bound to LOCAL HOST
	m_address.sin_port = htons(m_port);                // Assign server's port and convert to network bytes

	// Create socket file descriptor, exit on error
	if ((m_server_fd = socket(NAMESPACE, COMM_STYLE, PROTOCOL)) == 0)
	{
		perror("\nServer: Socket creation error\n");
		exit(EXIT_FAILURE);
	}
	else
		cout << "\nServer: Socket creation successful\n";

	// Setup options to reuse address and port with setsockopt function, exit on error
	if (setsockopt(m_server_fd, LEVEL, OPTNAMES, &OPTVAL, sizeof(OPTVAL)))
	{
		perror("\nServer: Error setting up socket options\n");
		exit(EXIT_FAILURE);
	}

	// Forcefully attaching socket to the port, exit on error
	if (bind(m_server_fd, (struct sockaddr*)&m_address, sizeof(m_address)) < 0)
	{
		perror("\nServer: Error binding socket to address\n");
		exit(EXIT_FAILURE);
	}
	else
		cout << "\nServer: Socket binding successful\n";

	// Enable connection requests on server file descriptor, exit on error
	if (listen(m_server_fd, BACKLOG) < 0)
	{
		perror("\nServer: Error listenting\n");
		exit(EXIT_FAILURE);
	}
	else
		cout << "\nServer: Server file descriptor listening for connection...\n";

	return true;  // Server started successfully
}


// Server file descriptor accepts new connection requests by listening on it's IP address.
// Creates new socket to handle RPC. 
bool RPCServer::ListenForClient()
{
	int addrlen = sizeof(m_address);

	if ((m_socket = accept(m_server_fd, (struct sockaddr*)&m_address, (socklen_t*)&addrlen)) < 0)
	{
		perror("\nServer: Error creating new socket\n");
		exit(EXIT_FAILURE);
	}
	else
		cout << "\nServer: Connection request rec'd...\n";

	this->ProcessRPC();
	return true;
}

// String processing helper function to Process RPC.
void RPCServer::ParseTokens(char * buffer, vector<string> & a)
{
	char* token;
	char* rest = (char *)buffer;

	// Split string on delimiter until no buffer remains. 
	while ((token = strtok_r(rest, ";", &rest)))

	{
		printf("%s\n", token); // For debugging.
		a.push_back(token);    // Store tokens. 
	}

	return;
}


bool RPCServer::ProcessRPC()
{
	const char* rpcs[] = { "connect", "disconnect", "status" };

	char buffer[1024] = { 0 };
	vector<string> arrayTokens;
	int valread = 0;  // Bytes of buffer rec'd by server. 
	bool bConnected = false;
	bool bStatusOk = true;
	const int RPCTOKEN = 0;
	bool bContinue = true;

	while ((bContinue) && (bStatusOk))
	{
		// Should be blocked when a new RPC has not called us yet
		if ((valread = read(this->m_socket, buffer, sizeof(buffer))) <= 0)
		{
			printf("errno is %d\n", errno);
			break;
		}
		printf("%s\n", buffer);

		arrayTokens.clear();
		this->ParseTokens(buffer, arrayTokens);

		// Enumerate through the tokens. The first token is always the specific RPC
		for (vector<string>::iterator t = arrayTokens.begin(); t != arrayTokens.end(); ++t)
		{
			printf("Debugging our tokens\n");
			printf("token = %s\n", t);
		}

		// string statements are not supported with a switch, so using if/else logic to dispatch
		string aString = arrayTokens[RPCTOKEN];

		if ((bConnected == false) && (aString == "connect"))
		{
			bStatusOk = ProcessConnectRPC(arrayTokens);  // Connect RPC
			if (bStatusOk == true)
				bConnected = true;
		}

		else if ((bConnected == true) && (aString == "disconnect"))
		{
			bStatusOk = ProcessDisconnectRPC();
			printf("We are going to terminate this endless loop\n");
			bContinue = false; // We are going to leave this loop, as we are done
		}

		else if ((bConnected == true) && (aString == "status"))
			bStatusOk = ProcessStatusRPC();   // Status RPC

		else
		{
			// Not in our list, perhaps, print out what was sent
		}

	}

	return true;
}

bool RPCServer::ProcessConnectRPC(vector<string> & arrayTokens)
{
	const int USERNAMETOKEN = 1;
	const int PASSWORDTOKEN = 2;

	// Strip out tokens 1 and 2 (username, password)
	string userNameString = arrayTokens[USERNAMETOKEN];
	string passwordString = arrayTokens[PASSWORDTOKEN];
	char szBuffer[80];

	// Our Authentication Logic. Looks like Mike/Mike is only valid combination
	if ((userNameString == "MIKE") && (passwordString == "MIKE"))
	{
		strcpy(szBuffer, "1;"); // Connected
	}
	else
	{
		strcpy(szBuffer, "0;"); // Not Connected
	}

	// Send Response back on our socket
	int nlen = strlen(szBuffer);
	szBuffer[nlen] = 0;
	send(this->m_socket, szBuffer, strlen(szBuffer) + 1, 0);

	return true;
}

/* TDB
*/
bool RPCServer::ProcessStatusRPC()
{
	return true;
}

/*
*/
bool RPCServer::ProcessDisconnectRPC()
{
	char szBuffer[16];
	strcpy(szBuffer, "disconnect");
	// Send Response back on our socket
	int nlen = strlen(szBuffer);
	szBuffer[nlen] = 0;
	send(this->m_socket, szBuffer, strlen(szBuffer) + 1, 0);
	return true;
}