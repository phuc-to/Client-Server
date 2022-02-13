/**
 @file  Server side class file for MealTime socket programming project
 @authors Phuc T, Narissa T, Kristen K
 @date 3/10/22
 @version 1.0
 @reference https://www.geeksforgeeks.org/socket-programming-cc/
 @reference https://www.gnu.org/software/libc/manual/html_node/Sockets.html
 */


#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <vector>
#include <iterator>

#include "RPCServer.h"

 //#define PORT 8081

using namespace std;

RPCServer::RPCServer(const char* serverIP, int port)
{
    m_rpcCount = 0;
    m_serverIP = (char*)serverIP;
    m_port = port;
};

RPCServer::~RPCServer() {};

/*
* StartServer will create a server on a Port that was passed in, and create a socket
*/

bool RPCServer::StartServer()
{
    int opt = 1;
    const int BACKLOG = 10;


    // Creating socket file descriptor
    if ((m_server_fd = socket(AF_INET, SOCK_STREAM, 0)) == 0)
    {
        perror("socket failed");
        exit(EXIT_FAILURE);
    }

    // Forcefully attaching socket to the port 8080
    if (setsockopt(m_server_fd, SOL_SOCKET, SO_REUSEADDR | SO_REUSEPORT,
        &opt, sizeof(opt)))
    {
        perror("setsockopt");
        exit(EXIT_FAILURE);
    }

    m_address.sin_family = AF_INET;
    m_address.sin_addr.s_addr = INADDR_ANY;
    m_address.sin_port = htons(m_port);

    // Forcefully attaching socket to the port 8080
    if (bind(m_server_fd, (struct sockaddr*)&m_address,
        sizeof(m_address)) < 0)
    {
        perror("bind failed");
        exit(EXIT_FAILURE);
    }
    if (listen(m_server_fd, BACKLOG) < 0)
    {
        perror("listen");
        exit(EXIT_FAILURE);
    }

    return true;
}

/*
* Will accept a new connection by listening on its address.
*
*/

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
void RPCServer::ParseTokens(char* buffer, std::vector<std::string>& a)
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
    const int INFOTOKEN = 1;

    // Strip out tokens 1 and 2 (username, password)
    string info = arrayTokens[INFOTOKEN];
    char szBuffer[80];
    strcpy(szBuffer, "Instant Noodle");
    // Send Response back on our socket
    int nlen = strlen(szBuffer);
    szBuffer[nlen] = 0;
    send(this->m_socket, szBuffer, strlen(szBuffer) + 1, 0);

    return true;
}