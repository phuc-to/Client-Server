/**
 @file  RPCImpl.h Remote Procedure Call header file. 
 @authors Phuc T, Narissa T
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
#include <semaphore.h>

#include "Auth.h"
#include "MealGenerator.h"
#include "GlobalContext.h"

using namespace std;

/*
 * Implementation of Remote Procedure Calls. 
 */
class RPCImpl {
public:
	/** Creates instance of RPC Implemenation class for access to processing RPCs.*/
    RPCImpl(int socket);
	
	/** Creates instance of RPC Implemenation class for access to processing RPCs.*/
	RPCImpl(int socket, MealGenerator* mg, Auth* accountDB, GlobalContext* gc, sem_t* updateMG, sem_t* updateDB, sem_t* updateGC);

	/** Destructor for instance of RPCImpl.*/
    ~RPCImpl();

	/** Routes to appropriate RPC based on remaining tokens in buffer.

	@pre Buffer sent by the client.
	@post The appropriate RPC call has been invoked.
	*/
	bool ProcessRPC();

private:
    int m_rpcCount;  
    int m_socket;
	bool adminPrivilege;
	MealGenerator* mg;  // Meal Generator object. 
	Auth* authObj;      // User authorization object. 
	GlobalContext* gc;  
	sem_t* updateMG;    // Protects global Meal Generator object. 
	sem_t* updateDB;    // Protects global Auth object. 
	sem_t* updateGC;    // Protects global context struct. 
	

	/** Parses client socket buffer and calls the approprite RPCImpl class function 
	to process the request. Accepts status tokens in predefined list within function. 

	@pre RPCImpl object seeded and socket connection is live. 
	@post RPCImpl class function will be employed. 
	@return true, RPC completed. 
	*/
    bool ProcessConnectRPC(vector<string>& arrayTokens, bool& connected);

	/** Attempts to sign user in. If user already exists, sends fail code in buffer. 

	@pre RPCImpl object seeded and socket connection is live.
	@post User signed in.
	@return true, RPC completed.
	*/
	bool ProcessSignupRPC(vector<string>& arrayTokens);

	/** Returns a buffer containing the information for the meal meeting the client's 
	input criteria. 

	@pre RPCImpl object seeded and socket connection is live.
	@post Buffer contains meal. 
	@return true, RPC completed.
	*/
	bool ProcessMealRPC(vector<string>& arrayTokens);

	/** Adds a new Meal to the Meal Generator. Sends fail code in buffer if meal already exists. 

	@pre RPCImpl object seeded and socket connection is live.
	@post SUCCESSCODE sent via buffer. 
	@return true, RPC completed.
	*/
	bool ProcessAddMealRPC(vector<string>& arrayTokens);

	/**  Confirms that RPCImpl is running. 

	@pre RPCImpl object seeded and socket connection is live.
	return true, RPCImpl is running.
	*/
    bool ProcessStatusRPC();

	/** Disconnects server socket from the lcient socket, and sends client
	message stating disconnect was successful.

	@pre RPCImpl object seeded and socket connection is live.
	@post Connection is terminated.
	*/
    bool ProcessDisconnectRPC();

	/** Log the user out from the current account to a not-logged-in state, and sends client
	message stating disconnect was successful.

	@pre RPCImpl object seeded and socket connection is live.
	@post Connected status changed to false.
	*/
	bool ProcessLogoutRPC();

	bool ProcessTotalRPC();

	bool ProcessMostRequestedRPC();

	/** Populates String vector with tokens extracted from the string the client sent.
	The delimter will be a ; An example buffer could be "connect;mike;mike;"

	@param buffer, pointer to a char array
	@param a, vector to hold tokens parsed from buffer
	@post a holds tokens from buffer
	*/
    void ParseTokens(char* buffer, vector<string>& a);

};
