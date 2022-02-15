/**
 @file  Server side class file for MealTime socket programming project
 @authors Phuc T, Narissa T, Kristen K
 @date 3/10/22
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
#include <netinet/in.h> // For constants and structs for INTERNET domain addresses.
#include <string.h>
#include <vector>
#include <iterator>


using namespace std;

// Constructor
RPCServer::RPCServer(const char* serverIP, int port)
{
    m_rpcCount = 0;
    m_serverIP = (char*)serverIP;
    m_port = port;
    mg = new MealGenerator();
};

// Destructor
RPCServer::~RPCServer() {};

/*
* StartServer will create a server on a Port that was passed in, and create a server side socket
*/

bool RPCServer::StartServer()
{

	// New socket constants 
	const int OPTVAL = 1;               // setsockopt option value
	const int BACKLOG = 10;             // Number of allowed connection requests before error thrown
	const int NAMESPACE = AF_INET;      // Internet namespace IP4 
	const int STYLE = SOCK_STREAM;      // Communication style for TCP programming
	const int PROTOCOL = 0;             

	// setsockopt constants
	const int LEVEL = SOL_SOCKET;
	const int OPTNAMES = SO_REUSEADDR | SO_REUSEPORT;  // Socket level options to reuse address and port


	// Create socket file descriptor, exit on error or display success msg
	if ((m_server_fd = socket(NAMESPACE, STYLE, PROTOCOL)) == 0)
    {
		perror("\nServer: Socket creation error\n");
		exit(EXIT_FAILURE);
    }
	else
		cout << "\nServer: Socket creation successful\n";


	// TODO: NT Adjust the options when we switch to multithreading
    // forcefully attaching socket to the port
    if (setsockopt(m_server_fd, LEVEL, OPTNAMES, &OPTVAL, sizeof(OPTVAL)))
    {
		perror("\nServer: Error setting up socket options\n");
		exit(EXIT_FAILURE);
    }

	// init server socket address 
	struct sockaddr_in m_address;                      // server socket address struct
	m_address.sin_family = NAMESPACE;                  // server socket's address format is INET for IP4
	m_address.sin_addr.s_addr = INADDR_ANY;            // Server socket is bound to LOCAL HOST
	m_address.sin_port = htons(m_port);                // Assign server's port and convert to network bytes

    // Bind socket to port
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
		perror("\nServer: Error listening\n");
		exit(EXIT_FAILURE);
    }
	else
		cout << "\nServer: Server file descriptor listening for connection...\n";

    return true; // server started successfully
}


// Server file descriptor accepts new connection requests by listening on it's IP address.
// Creates new socket to handle RPC.
bool RPCServer::ListenForClient()
{

    int addrlen = sizeof(m_address);

    if ((m_socket = accept(m_server_fd, (struct sockaddr*)&m_address,
        (socklen_t*)&addrlen)) < 0)
    {
        perror("accept");
        exit(EXIT_FAILURE);
    }

    this->ProcessRPC();
    return true;
}

/*
* Going to populate a String vector with tokens extracted from the string the client sent.
* The delimter will be a ;
* An example buffer could be "connect;mike;mike;"
*/
void RPCServer::ParseTokens(char* buffer, vector<string>& a)
{
    char* token;
    char* rest = (char*)buffer;

    while ((token = strtok_r(rest, ";", &rest)))
    {
        printf("%s\n", token);
        a.push_back(token);
    }

    return;
}

/*
* ProcessRPC will examine buffer and will essentially control which RPC calls
* are made. The function invokes the ParseTokens method to read the message
* sent by the client, and then invokes the appropriate RPC method call.
* 
*/
bool RPCServer::ProcessRPC()
{
    const char* rpcs[] = { "connect", "disconnect", "status" };
    char buffer[1024] = { 0 };
    std::vector<std::string> arrayTokens;
    int valread = 0;
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
        
        else if ((bConnected == true) && (aString == "mealRandom"))
        {
            bStatusOk = ProcessMealRPC(arrayTokens);
        }

        else if ((bConnected == true) && (aString == "mealByTime"))
        {
            bStatusOk = ProcessMealRPC(arrayTokens);
        }

        else if ((bConnected == true) && (aString == "mealByCuisine"))
        {
            bStatusOk = ProcessMealRPC(arrayTokens);
        }
        else
        {
            // Not in our list, perhaps, print out what was sent
        }

    }

    return true;
}

/*
* Connects the 
*/
bool RPCServer::ProcessConnectRPC(std::vector<std::string>& arrayTokens)
{
    const int USERNAMETOKEN = 1;
    const int PASSWORDTOKEN = 2;

    // Strip out tokens 1 and 2 (username, password)
    string userNameString = arrayTokens[USERNAMETOKEN];
    string passwordString = arrayTokens[PASSWORDTOKEN];
    char szBuffer[80];
    strcpy(szBuffer, "successful");
    // Send Response back on our socket
    int nlen = strlen(szBuffer);
    szBuffer[nlen] = 0;
    send(this->m_socket, szBuffer, strlen(szBuffer) + 1, 0);

    return true;
}

/* 
* Returns the status of the RPC server.
*/
bool RPCServer::ProcessStatusRPC()
{
    return true;
}

/*
* Disconnects server socket from the lcient socket, and sends client 
* message stating disconnect was successful. 
*/
bool RPCServer::ProcessDisconnectRPC()
{
    char szBuffer[16];
    strcpy(szBuffer, "successful");
    // Send Response back on our socket
    int nlen = strlen(szBuffer);
    szBuffer[nlen] = 0;
    send(this->m_socket, szBuffer, strlen(szBuffer) + 1, 0);
    return true;
}

/*
* Returns a buffer containing the information
* for the meal that meets the client's submitted
* criteria. 
*/
bool RPCServer::ProcessMealRPC(std::vector<std::string>& arrayTokens)
{
    const int RPCTOKEN = 0;
    const int INFOTOKEN = 1;

    // Strip out tokens 1 and 2 (username, password)
    string RPC = arrayTokens[RPCTOKEN];
    string info = arrayTokens[INFOTOKEN];
    char szBuffer[80];
    if (RPC == "mealRandom")
        strcpy(szBuffer, (mg->getRandomMeal()).c_str());
    else if (RPC == "mealByTime")
        strcpy(szBuffer, (mg->getRandomMealByTime(info)).c_str());
    else
        strcpy(szBuffer, (mg->getRandomMealByCuisine(info)).c_str());
    // Send Response back on our socket
    int nlen = strlen(szBuffer);
    szBuffer[nlen] = 0;
    send(this->m_socket, szBuffer, strlen(szBuffer) + 1, 0);

    return true;
}