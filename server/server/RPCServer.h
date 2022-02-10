/**
 @file  Server side class header  for MealTime socket programming project
 @authors Phuc T, Narissa T, Kristen K
 @date 2/8/22
 @version 1.0
 @reference https://www.geeksforgeeks.org/socket-programming-cc/
 @reference https://www.gnu.org/software/libc/manual/html_node/Sockets.html
 */

#pragma once
#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h> // UNIX socket structs
#include <stdlib.h>
#include <netinet/in.h> // For onstants and structs for INTERNET domain addresses  
#include <string.h>
#include <vector>
#include <iterator>

using namespace std; 

/**
 * TODO: Class description 
*/
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
		@post An RPCServer listening for connection requestions. 
		@returns True Upon successful start up. 
		*/
		bool StartServer();

		/** Server file descriptor accepts new connection requests by listening on it's IP address.
		@pre A server file descriptor in listening mode (successful call to StartServer).
		@post Upon receipt of connection request, creates new socket to process RPC. Calls ProcessRPC. 
		@returns True Upon successful acceptance of connection request and creation of new socket. 

		*/
		bool ListenForClient();

		/** String processing helper function to Process RPC. Delimits and tokenizes buffer. Stores tokens
			in vector. 
		@param buffer Charcter array. 
		@param a String vector holds tokens. 
		@pre Buffer containing 0 to many tokens in the form 'abc;def;jkl'. 
		@post a Vector contains tokenized portions of original buffer. 
		*/
		void ParseTokens(char* buffer, vector<string>& a);

		/** 
		*/
		bool ProcessRPC();

	private:
		int m_rpcCount;                // for metrics
		int m_server_fd;               // server socket file descriptor (listening only)
		int m_socket;                  // server socket (read and write only)
		char* m_serverIP;              // server's IP address, from main
		int m_port;                    // server's port, from main
		struct sockaddr_in m_address;  // server socket address struct

		// First one in this function should be a connect, and it 
		// will continue try to process RPC's until a Disconnect happens
		bool ProcessConnectRPC(vector<string>& arrayTokens);
		bool ProcessMealRPC(vector<string>& arrayTokens);
		bool ProcessStatusRPC();
		bool ProcessDisconnectRPC();


};

