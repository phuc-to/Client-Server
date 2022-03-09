/**
 @file  Client side header file for MealTime socket programming project
 @authors Phuc T, Narissa T
 @date 3/10/22
 @version 2.0
 */

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h> // UNIX socket structs
#include <stdlib.h>
#include <netinet/in.h> // For constants and structs for INTERNET domain addresses  
#include <string.h>
#include <vector>
#include <iterator>

using namespace std; 

/** Static client class responsible for connecting to server. */
class client
{
	public:


		/** Displays menu selections for user to choose from through console input*/
		static void optionList();

		/** Prompts user to login, signup or exit. Builds and send corresponding buffer to server. 
		
		@param cliSocket, int representing client socket.
		@param buffer, char* to hold buffer contents.
		@param valsend, number of bytes to be sent to server.
		@param valread, number of bytes read back from server.
		@param arrayTokens vector holding tokens parsed from buffer
		@param STATUSTOKEN, index of RPC status ("connect", "disconnect", etc).
		@param INFOTOKEN, index of related info from user. 
		@param msg,  message to be sent to server in buffer. 
		*/
		static bool loginValidation(int cliSocket, char* buffer, int valsend, int valread, vector<string> arrayTokens, 
									int STATUSTOKEN, int INFOTOKEN, string msg);

		/**

		@param cliSocket, int representing client socket.
		@param buffer, char* to hold buffer contents.
		@param valsend, number of bytes to be sent to server.
		@param valread, number of bytes read back from server.
		@param arrayTokens vector holding tokens parsed from buffer
		@param STATUSTOKEN, index of RPC status ("connect", "disconnect", etc).
		@param INFOTOKEN, index of related info from user.
		@param msg,  message to be sent to server in buffer.
		*/
		static bool mealProcessing(int cliSocket, char* buffer, int valsend, int valread, vector<string> arrayTokens, int STATUSTOKEN, 
								   int INFOTOKEN, string msg);
		
		/** Displays menu selections for desired meal for user to choose from through console input*/
		static void mealOptions();

		/** Given a char pointer, splits the buffer into tokens based on a ';' delimiter and stores in vector.

		@param buffer, char* holding buffer contents.
		@param &arrayTokens, reference to vector to fill with tokens parsed from buffer. 
		*/
		static void parseTokens(char * buffer, std::vector<std::string>& a);

		/** Given serverIP address, port and reference to socket, creates client socket and attempts
		to connect to server.
		
		@param serverAddress, holds IP address. 
		@param port, holds port number.
		@param sock, holds reference to client socket var. 
		@post client is connected to server. 
		*/
		static bool connectToServer(const char * serverAddress, int port, int & sock);

	private:


};
