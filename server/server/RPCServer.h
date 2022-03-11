/**
 @file  RPCServer.h
 @authors Phuc T, Narissa T
 @date 3/10/22
 @version 2.0
 */

#pragma once
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h> // UNIX socket structs
#include <stdlib.h>
#include <netinet/in.h> // For constants and structs for INTERNET domain addresses  
#include <string.h>
#include <vector>
#include <iterator>
#include <semaphore.h>



#include "MealGenerator.h"
#include "GlobalContext.h"
#include "RPCImpl.h" 

using namespace std; 

class RPCServer
{

	public:
		/** Initalizes instance of RPCServer.
		@param serverIP Server's IP address
		@param port Given port for RCPServer to connect to 
		*/
		RPCServer(const char *serverIP, int port);

		/** Deallocates dyanically allocated memory of the serverIP member. 
		@pre An initialized RPCServer object. 
		@post RPCServer object is destroyed. 
		*/
		~RPCServer();

		/** Starts server file descriptor socket, opening it to listen for connection requests. 
		@pre An initialized RPCServer object. 
		@post An RPCServer listening for connection requests. 
		@returns True Upon successful start up. 
		*/
		bool StartServer();


		/** Server file descriptor accepts new connection requests by listening on it's IP address.
		@pre A server file descriptor in listening mode (successful call to StartServer).
		@post Upon receipt of connection request, creates new socket to process RPC. Calls ProcessRPC. 
		@returns True Upon successful acceptance of connection request and creation of new socket. 

		*/
		bool ListenForClient();

	private:
		int m_rpcCount;                // for metrics
		int m_server_fd;               // server socket file descriptor (listening only)
		int m_socket;                  // server socket (read and write only)
		char* m_serverIP;              // server's IP address, from main
		int m_port;                    // server's port, from main
		struct sockaddr_in m_address;  // server socket address struct

		MealGenerator* mg;              // private MG object
		Auth* accountDB;
		GlobalContext* gc;
		sem_t* updateMG;
		sem_t* updateDB;
		sem_t* updateGC;
};

