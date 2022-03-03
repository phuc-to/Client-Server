/**
 @file  RPCImpl.h
 @authors Phuc T, Narissa T, Kristen K
 @date 3/10/22
 @version 2.0
 */


#pragma once

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string>
#include <vector>
#include <iterator>

#include "Auth.h"
#include "MealGenerator.h"


class RPCImpl
{
public:
	/**Creates instance of RPC Implemenation class for access to processing RPCs.*/
    RPCImpl(int socket);

	/** Destructor for instance of RPCImpl.*/
    ~RPCImpl();

	/** Function for processing all RPC calls.

	@pre Buffer sent by the client.
	@post The appropriate RPC call has been invoked.
	*/
	bool ProcessRPC();

private:
    int m_rpcCount;
    int m_socket;
	MealGenerator* mg;
	Auth* authObj;
	

	/**
	ProcessConnectRPC deconstructs client socket buffer and calls the approprite RPCImpl class function 
	to process the request. Accepts status tokens in predefined list within function. 
	@pre RPCImpl object seeded with socket exists. 
	@post RPCImpl class function will be employed. 
	@return true, upon completion of process. 
	*/
    bool ProcessConnectRPC(std::vector<std::string>& arrayTokens);

	bool ProcessSignupRPC(std::vector<std::string>& arrayTokens);

	bool ProcessMealRPC(std::vector<std::string>& arrayTokens);

	bool ProcessAddMealRPC(std::vector<std::string>& arrayTokens);

    bool ProcessStatusRPC();

	/** Disconnects server socket from the lcient socket, and sends client
		message stating disconnect was successful.
	*/
    bool ProcessDisconnectRPC();

	/** Populates String vector with tokens extracted from the string the client sent.
	The delimter will be a ; An example buffer could be "connect;mike;mike;"

	@param buffer, pointer to a char array
	@param a, vector to hold tokens parsed from buffer
	@post a holds tokens from buffer
	*/
    void ParseTokens(char* buffer, std::vector<std::string>& a);

};
