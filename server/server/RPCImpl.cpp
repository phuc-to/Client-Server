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
	m_socket = socket;         // Assign socket to RPCImpl socket field. 
	m_rpcCount = 0;
	mg = new MealGenerator();  // Init new MG object.
	authObj = new Auth();      // Init new Auth object. 
	authObj->signUp(defaultID, defaultPassword, "Y"); // Seed Auth with default user and password. 
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
	
	/* 
	Holds tokenized form of buffer in the formats: 
	Sign up: {"RPC code", "username", "password", "Y" or "N"}
	Login: {"RPC code", "username", "password"}
	*/
	vector<string> arrayTokens;             

	char buffer[1024] = { 0 };              // Size of return buffer. 
	int valread = 0;                        // Bytes read from client. 
	bool bConnected = false;                // Server is connected to client.
	bool bStatusOk = true;
	bool bContinue = true;
	const int RPCTOKEN = 0;                 // Index in RPC token array.
	string aString;                         // Holds RPC token.

	// Exit loop on bStatusOK and bContinue false. 
	while ((bContinue) && (bStatusOk)) {
		// Read socket's buffer and parse tokens into arrayTokens. 
		if ((valread = read(this->m_socket, buffer, sizeof(buffer))) <= 0) {
			printf("errno is %d\n", errno);
			break;
		}
		printf("%s\n", buffer);

		// Refresh array token list. 
		arrayTokens.clear(); 

		// Parse tokens from buffer into arrayTokens. 
		this->ParseTokens(buffer, arrayTokens); 
		aString = arrayTokens[RPCTOKEN];

		// Signup RPC - call ProcessSignupRPC, followed by ProcessConnectRPC. 
		if ((bConnected == false) && (aString == SIGNUP)) {
			bStatusOk = ProcessSignupRPC(arrayTokens);
			
			aString = CONNECT;
		}

		// Connect RPC - call ProcessConnectRPC when not connected and RPCToken is "connect". 
		if ((bConnected == false) && (aString == CONNECT)) {
			bStatusOk = ProcessConnectRPC(arrayTokens);  
			if (bStatusOk == true)
				bConnected = true;
		}
		
		// Disconnect RPC - call ProcessDisconnectRPC when connected and RPCToken is "disconnect". 
		else if ((bConnected == true) && (aString == DISCONNECT)) {
			bStatusOk = ProcessDisconnectRPC();
			printf("RPCServer>Disconnected.");
			bContinue = false; 
		}

		// Status RPC - call ProcessStatusRPC on RPCToken "status". 
		else if ((bConnected == true) && (aString == STATUS))
			bStatusOk = ProcessStatusRPC();   // Status RPC

		// Get Meal RPC - call ProcessMealRPC when connected and RPCToken is "meal". 
		else if ((bConnected == true) && (aString == MEAL)) {
			bStatusOk = ProcessMealRPC(arrayTokens);  
			if (bStatusOk == true)
				bConnected = true;
		}

		// AddMeal RPC - calls ProcessAddMealRPC when connected and RPCToken is "addMeal". 
		else if ((bConnected == true) && (aString == ADDMEAL)) {
			bStatusOk = ProcessAddMealRPC(arrayTokens);  
			if (bStatusOk == true)
				bConnected = true;
		}
		else {
			printf("RPCServer>ProcessRPC: Invalid tokens: ", aString, "\n");
		}
	}
	return true;
}

bool RPCImpl::ProcessConnectRPC(vector<string>& arrayTokens)
{
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

bool RPCImpl::ProcessSignupRPC(vector<string>& arrayTokens)
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

	// Send response to client.
	int nlen = strlen(szBuffer);
	szBuffer[nlen] = 0;
	send(this->m_socket, szBuffer, strlen(szBuffer) + 1, 0);
	return true;  // RPC complete. 
}

bool RPCImpl::ProcessMealRPC(vector<string>& arrayTokens)
{
	const int RPCTOKEN = 1;           // "Random", "ByTime", "ByCuisine". 
	const int INFOTOKEN = 2;          
	string output;                    // Holds output from Meal Generator object class function. 
	string error;                     // Holds error message to be copied to socket. 
	char szBuffer[80];                // Size of output buffer.

	// Strip out tokens 1 and 2 (username, password).
	string RPC = arrayTokens[RPCTOKEN];
	string info = arrayTokens[INFOTOKEN];

	// Gets random meal, meal by time, or meal by cuisine from Meal Generator object mg, copies to buffer. 
	// If no output, copies error message to buffer. 
	if (RPC == "Random") {
		output = mg->getRandomMeal();
		if (output != "")
			strcpy(szBuffer, (SUCCESSCODE + output).c_str());
		else
			strcpy(szBuffer, "failed;Meal List is empty;");
	}
	else if (RPC == "ByTime") {
		output = mg->getRandomMealByTime(info);
		if (output != "")
			strcpy(szBuffer, (SUCCESSCODE + output).c_str());
		else {
			error = "failed;There is no meal in the list for " + info + ";";
			strcpy(szBuffer, error.c_str());
		}
	}
	else if (RPC == "ByCuisine") {
		output = mg->getRandomMealByCuisine(info);
		if (output != "")
			strcpy(szBuffer, (SUCCESSCODE + output).c_str());
		else {
			error = "failed;No " + info + " meals stored yet;";
			strcpy(szBuffer, error.c_str());
		}
	}
	else
		strcpy(szBuffer, "failed;Invalid Argument, or Operation Not Supported;");

	// Send response to client.
	int nlen = strlen(szBuffer);
	szBuffer[nlen] = 0;
	send(this->m_socket, szBuffer, strlen(szBuffer) + 1, 0);

	return true; // RPC complete. 
}

bool RPCImpl::ProcessAddMealRPC(vector<string>& arrayTokens)
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
	char szBuffer[16]; // Size of output buffer. 
	strcpy(szBuffer, "active;");
	// Send Response back on our socket
	int nlen = strlen(szBuffer);
	szBuffer[nlen] = 0;
	send(this->m_socket, szBuffer, strlen(szBuffer) + 1, 0);
	return true;  // RPC complete. 
}

bool RPCImpl::ProcessDisconnectRPC()
{
	char szBuffer[16];  // Size of output buffer.
	strcpy(szBuffer, "disconnect;");
	// Send Response back to client.
	int nlen = strlen(szBuffer);
	szBuffer[nlen] = 0;
	send(this->m_socket, szBuffer, strlen(szBuffer) + 1, 0);
	return true; // RPC complete. 
}

void RPCImpl::ParseTokens(char* buffer, vector<string>& a)
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