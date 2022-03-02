/**
 @file RPCImpl.cpp
 @authors Phuc T, Narissa T, Kristen K
 @date 3/10/22
 @version 2.0
 */

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <vector>
#include <iterator>

#include "RPCImpl.h"
#include "LocalContext.h"
#include "Auth.h"

using namespace std;

typedef struct _GlobalContext {
	int g_rpcCount;
	int g_mealCount;
	int g_breakfastCount;
	int g_lunchCount;
	int g_dinnerCount;
} GlobalContext;

GlobalContext globalObj; // We need to protect this, as we don't want bad data


RPCImpl::RPCImpl(int socket)
{
	m_socket = socket;
	m_rpcCount = 0;
	mg = new MealGenerator();
	authObj = new Auth();
};


RPCImpl::~RPCImpl() {};


void RPCImpl::ParseTokens(char* buffer, std::vector<std::string>& a)
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


bool RPCImpl::ProcessRPC()
{
	char buffer[1024] = { 0 };
	std::vector<std::string> arrayTokens;   // Holds tokenized form of buffer
	int valread = 0;
	bool bConnected = false;                // Server is connected to client
	bool bStatusOk = true;
	bool bContinue = true;
	const int RPCTOKEN = 0;                 // Index in RPC token array
	string aString;                         // Holds RPC token

	// Exit loop on bStatusOK and bContinue false. 
	while ((bContinue) && (bStatusOk))
	{
		// Read socket's buffer and parse tokens into arrayTokens. 
		if ((valread = read(this->m_socket, buffer, sizeof(buffer))) <= 0)
		{
			printf("errno is %d\n", errno);
			break;
		}
		printf("%s\n", buffer);
		arrayTokens.clear(); // Refresh array token list. 
		this->ParseTokens(buffer, arrayTokens); // Parse tokens from buffer into arrayTokens. 
		aString = arrayTokens[RPCTOKEN];
#if 0
		// Enumerate through the tokens. The first token is always the specific RPC
		for (vector<string>::iterator t = arrayTokens.begin(); t != arrayTokens.end(); ++t)
		{
			printf("Debugging our tokens\n");
			printf("token = %s\n", t);
		}
#endif
		// Signup protocol - call ProcessSignupRPC, followed by ProcessConnectRPC. 
		if ((bConnected == false) && (aString == "signup")) {
			bStatusOk = ProcessSignupRPC(arrayTokens);
			
			aString = "connect";
		}


		// Login protocol - call ProcessConnectRPC when not connected and RPCToken is "connect". 
		if ((bConnected == false) && (aString == "connect"))
		{
			bStatusOk = ProcessConnectRPC(arrayTokens);  // Connect RPC
			if (bStatusOk == true)
				bConnected = true;
		}
		
		// Disconnect protocol - call ProcessDisconnectRPC when connected and RPCToken is "disconnect". 
		else if ((bConnected == true) && (aString == "disconnect"))
		{
			bStatusOk = ProcessDisconnectRPC();
			printf("Server: Disconnected.");
			bContinue = false; 
		}

		// Status protocol - call ProcessStatusRPC on RPCToken "status". 
		else if ((bConnected == true) && (aString == "status"))
			bStatusOk = ProcessStatusRPC();   // Status RPC

		// Get Meal Protocol - call ProcessMealRPC when connected and RPCToken is "mean". 
		else if ((bConnected == true) && (aString == "meal"))
		{
			bStatusOk = ProcessMealRPC(arrayTokens);  // Connect RPC
			if (bStatusOk == true)
				bConnected = true;
		}

		else if ((bConnected == true) && (aString == "addMeal"))
		{
			bStatusOk = ProcessAddMealRPC(arrayTokens);  // Connect RPC
			if (bStatusOk == true)
				bConnected = true;
		}

		else
		{
			// Not in our list, perhaps, print out what was sent
		}

	}

	return true;
}


bool RPCImpl::ProcessSignupRPC(std::vector<std::string>& arrayTokens)
{
	const int USERNAMETOKEN = 1;
	const int PASSWORDTOKEN = 2;
	const int ADMINTOKEN = 3;

	// Strip out tokens. 
	string userNameString = arrayTokens[USERNAMETOKEN];
	string passwordString = arrayTokens[PASSWORDTOKEN];
	string adminString = arrayTokens[ADMINTOKEN];
	char szBuffer[80];

	// Call Auth's SignUp function. 
	// If account already exists, send failure error code back. 
	if (authObj->SignUp(userNameString, passwordString, adminString)) 
		strcpy(szBuffer, "successful;");
	else
		strcpy(szBuffer, "failure;");

	// Send response back.
	int nlen = strlen(szBuffer);
	szBuffer[nlen] = 0;
	send(this->m_socket, szBuffer, strlen(szBuffer) + 1, 0);
	return true;
}


bool RPCImpl::ProcessConnectRPC(std::vector<std::string>& arrayTokens)
{
	// TODO: Integrate with client. NT. 

	// Init new Auth object.
	Auth authObj = Auth(); 

	const int USERNAMETOKEN = 1;
	const int PASSWORDTOKEN = 2;

	// Strip out tokens 1 and 2 (username, password)
	string userNameString = arrayTokens[USERNAMETOKEN];
	string passwordString = arrayTokens[PASSWORDTOKEN];
	char szBuffer[80];

	// TODO: Remove preprocessor block at end of project
#if 0
	// TODO NT CALL AUTH CLASS HERE 
	// Our Authentication Logic. Looks like Mike/Mike is only valid combination
	if ((userNameString == "MIKE") && (passwordString == "MIKE"))
	{
		strcpy(szBuffer, "1;"); // Connected
	}
	else
	{
		strcpy(szBuffer, "0;"); // Not Connected
	}
#endif
	strcpy(szBuffer, "successful;");
	// send Response back on our socket
	int nlen = strlen(szBuffer);
	szBuffer[nlen] = 0;
	send(this->m_socket, szBuffer, strlen(szBuffer) + 1, 0);

	return true;
}


bool RPCImpl::ProcessStatusRPC()
{
	return true;
}


bool RPCImpl::ProcessDisconnectRPC()
{
	char szBuffer[16];
	strcpy(szBuffer, "disconnect;");
	// Send Response back on our socket
	int nlen = strlen(szBuffer);
	szBuffer[nlen] = 0;
	send(this->m_socket, szBuffer, strlen(szBuffer) + 1, 0);
	return true;
}



/* Returns a buffer containing the informationfor the meal that meets the client's submitted
   criteria. */
bool RPCImpl::ProcessMealRPC(std::vector<std::string>& arrayTokens)
{
    const int RPCTOKEN = 1;
    const int INFOTOKEN = 2;
	string output;                    // Holds output from Meal Generator object class function. 
	string error;                     // Holds error message to be copied to socket. 

    // Strip out tokens 1 and 2 (username, password).
    string RPC = arrayTokens[RPCTOKEN];
    string info = arrayTokens[INFOTOKEN];
    char szBuffer[80];

	// Gets random meal, meal by time, or meal by cuisine from Meal Generator object mg, copies to buffer. 
	// If no output, copies error message to buffer. 
	if (RPC == "Random") 
	{
		output = mg->getRandomMeal();
		if (output != "")
			strcpy(szBuffer, ("successful;" + output).c_str());
		else
			strcpy(szBuffer, "failed;Meal List is empty;");
	}
	else if (RPC == "ByTime")
	{
		output = mg->getRandomMealByTime(info);
		if (output != "")
			strcpy(szBuffer, ("successful;" + output).c_str());
		else
		{
			error = "failed;There is no meal in the list for " + info + ";";
			strcpy(szBuffer, error.c_str());
		}
	}
	else if (RPC == "ByCuisine")
	{
		output = mg->getRandomMealByCuisine(info);
		if (output != "")
			strcpy(szBuffer, ("successful;" + output).c_str());
		else
		{
			error = "failed;There is no meal in the list from " + info + ";";
			strcpy(szBuffer, error.c_str());
		}
	}
	else
		strcpy(szBuffer, "failed;Invalid Argument, or Operation Not Supported;");

    // Send response back on the socket.
    int nlen = strlen(szBuffer);
    szBuffer[nlen] = 0;
    send(this->m_socket, szBuffer, strlen(szBuffer) + 1, 0);

    return true;
}

bool RPCImpl::ProcessAddMealRPC(std::vector<std::string>& arrayTokens)
{
	const int NAMETOKEN = 1;
	const int TIMETOKEN = 2;
	const int CUISINETOKEN = 3;

	// Strip out tokens 1 and 2 (username, password).
	string name = arrayTokens[NAMETOKEN];
	string time = arrayTokens[TIMETOKEN];
	string cuisine = arrayTokens[CUISINETOKEN];
	char szBuffer[80];

	mg->addMeal(name, time, cuisine);  // Add new meal to Meal Generator class 
	strcpy(szBuffer, "successful");
	int nlen = strlen(szBuffer);
	szBuffer[nlen] = 0;
	send(this->m_socket, szBuffer, strlen(szBuffer) + 1, 0);
	return true;
}
