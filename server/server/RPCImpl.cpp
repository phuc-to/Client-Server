/**
 @file  RPCImpl.cpp Remote Procedure Call implementation file.
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

// Global error codes. 
const char* FAILCODE = "failure;"; 
const char* SUCCESSCODE = "successful;"; 
const string defaultID = "admin";
const string defaultPassword = "password";

typedef struct _GlobalContext {
	int g_rpcCount;
	int g_mealCount;
	int g_breakfastCount;
	int g_lunchCount;
	int g_dinnerCount;
} GlobalContext;

GlobalContext globalObj; 

RPCImpl::RPCImpl(int socket)
{
	m_socket = socket;
	m_rpcCount = 0;
	mg = new MealGenerator();
	authObj = new Auth();
	authObj->signUp(defaultID, defaultPassword, "Y");
};

RPCImpl::~RPCImpl() {};

bool RPCImpl::ProcessRPC()
{
	// List of valid RPC codes that ProcessRPC will recognize. 
	const vector<string> RPCList = { "signup", "connect", "disconnect", "status", "meal", "addMeal"};
	const string SIGNUP = RPCList[0];
	const string CONNECT = RPCList[1];
	const string DISCONNECT = RPCList[2];
	const string STATUS = RPCList[3];
	const string MEAL = RPCList[4];
	const string ADDMEAL = RPCList[5];
	
	// Holds tokenized form of buffer in the formats: 
	// Sign up: {"RPC code", "username", "password", "Y" or "N"} 
	// Login: {"RPC code", "username", "password"} 
	vector<string> arrayTokens;             


	char buffer[1024] = { 0 };              // Size of return buffer. 
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

		// Signup protocol - call ProcessSignupRPC, followed by ProcessConnectRPC. 
		if ((bConnected == false) && (aString == SIGNUP)) {
			bStatusOk = ProcessSignupRPC(arrayTokens);
			
			aString = CONNECT;
		}

		// Login protocol - call ProcessConnectRPC when not connected and RPCToken is "connect". 
		if ((bConnected == false) && (aString == CONNECT))
		{
			bStatusOk = ProcessConnectRPC(arrayTokens);  // Connect RPC
			if (bStatusOk == true)
				bConnected = true;
		}
		
		// Disconnect protocol - call ProcessDisconnectRPC when connected and RPCToken is "disconnect". 
		else if ((bConnected == true) && (aString == DISCONNECT))
		{
			bStatusOk = ProcessDisconnectRPC();
			printf("Server: Disconnected.");
			bContinue = false; 
		}

		// Status protocol - call ProcessStatusRPC on RPCToken "status". 
		else if ((bConnected == true) && (aString == STATUS))
			bStatusOk = ProcessStatusRPC();   // Status RPC

		// Get Meal Protocol - call ProcessMealRPC when connected and RPCToken is "mean". 
		else if ((bConnected == true) && (aString == MEAL))
		{
			bStatusOk = ProcessMealRPC(arrayTokens);  // Connect RPC
			if (bStatusOk == true)
				bConnected = true;
		}

		else if ((bConnected == true) && (aString == ADDMEAL))
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

bool RPCImpl::ProcessConnectRPC(std::vector<std::string>& arrayTokens)
{
	// Init new Auth object.
	//Auth authObj = Auth(); 

	const int USERNAMETOKEN = 1;
	const int PASSWORDTOKEN = 2;

	// Strip out tokens 1 and 2 (username, password).
	string userNameString = arrayTokens[USERNAMETOKEN];
	string passwordString = arrayTokens[PASSWORDTOKEN];
	char szBuffer[80];

	// Try to login.
	if (authObj->login(userNameString, passwordString))
		strcpy(szBuffer, SUCCESSCODE); // Login was successful. 
	else
		strcpy(szBuffer, FAILCODE);    // Login failed. 

	// Send response back.
	int nlen = strlen(szBuffer);
	szBuffer[nlen] = 0;
	send(this->m_socket, szBuffer, strlen(szBuffer) + 1, 0);

	return true;  // RPC complete. 
}

bool RPCImpl::ProcessSignupRPC(std::vector<std::string>& arrayTokens)
{
	const int USERNAMETOKEN = 1;
	const int PASSWORDTOKEN = 2;
	const int ADMINTOKEN = 3;

	// Strip out tokens from arrayTokens. 
	string userNameString = arrayTokens[USERNAMETOKEN];
	string passwordString = arrayTokens[PASSWORDTOKEN];
	string adminString = arrayTokens[ADMINTOKEN];
	char szBuffer[80];

	// Call Auth's SignUp function. 
	// If account already exists, send failure error code back. 
	if (authObj->signUp(userNameString, passwordString, adminString)) 
		strcpy(szBuffer, SUCCESSCODE); // New account created. 
	else
		strcpy(szBuffer, FAILCODE);  // Account is already signed up. 

	// Send response back.
	int nlen = strlen(szBuffer);
	szBuffer[nlen] = 0;
	send(this->m_socket, szBuffer, strlen(szBuffer) + 1, 0);
	return true;  // RPC complete. 
}

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
			strcpy(szBuffer, (SUCCESSCODE + output).c_str());
		else
			strcpy(szBuffer, "failed;Meal List is empty;");
	}
	else if (RPC == "ByTime")
	{
		// TODO: Input validation of times 

		output = mg->getRandomMealByTime(info);
		if (output != "")
			strcpy(szBuffer, (SUCCESSCODE + output).c_str());
		else
		{
			error = "failed;There is no meal in the list for " + info + ";";
			strcpy(szBuffer, error.c_str());
		}
	}
	else if (RPC == "ByCuisine")
	{
		// TODO: Input validation of cuisines 

		output = mg->getRandomMealByCuisine(info);
		if (output != "")
			strcpy(szBuffer, (SUCCESSCODE + output).c_str());
		else
		{
			error = "failed;No " + info + " meals stored yet;";
			strcpy(szBuffer, error.c_str());
		}
	}
	else
		strcpy(szBuffer, "failed;Invalid Argument, or Operation Not Supported;");

	// Send response back on the socket.
	int nlen = strlen(szBuffer);
	szBuffer[nlen] = 0;
	send(this->m_socket, szBuffer, strlen(szBuffer) + 1, 0);

	return true; // RPC complete. 
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

	// Attempt to add new meal to Meal Generator class 
	if(!(mg->addMeal(name, time, cuisine)))  // Meal already exists in MG. 
		strcpy(szBuffer, FAILCODE);
	else
		strcpy(szBuffer,SUCCESSCODE);        // New meal added. 
	int nlen = strlen(szBuffer);
	szBuffer[nlen] = 0;
	send(this->m_socket, szBuffer, strlen(szBuffer) + 1, 0);
	return true; // RPC complete. 
}

bool RPCImpl::ProcessStatusRPC()
{
	char szBuffer[16];
	strcpy(szBuffer, "active;");
	// Send Response back on our socket
	int nlen = strlen(szBuffer);
	szBuffer[nlen] = 0;
	send(this->m_socket, szBuffer, strlen(szBuffer) + 1, 0);
	return true;  // RPC complete. 
}

bool RPCImpl::ProcessDisconnectRPC()
{
	char szBuffer[16];
	strcpy(szBuffer, "disconnect;");
	// Send Response back on our socket
	int nlen = strlen(szBuffer);
	szBuffer[nlen] = 0;
	send(this->m_socket, szBuffer, strlen(szBuffer) + 1, 0);
	return true; // RPC complete. 
}

void RPCImpl::ParseTokens(char* buffer, std::vector<std::string>& a)
{
	char* token;
	char* rest = (char*)buffer;

	printf("Parsed tokens: ");
	while ((token = strtok_r(rest, ";", &rest)))
	{
		printf("%s ", token);
		a.push_back(token);
	}
	printf("\n");

	return;
}