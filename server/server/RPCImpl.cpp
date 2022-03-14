/**
 @file  RPCImpl.cpp Remote Procedure Call implementation file.
 @authors Phuc T, Narissa T
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
#include <semaphore.h>

#include "RPCImpl.h"
#include "Auth.h"
#include "GlobalContext.h"

using namespace std;

// Global error codes. 
const char* FAILCODE = "failure;"; 
const char* SUCCESSCODE = "successful;"; 
const string defaultID = "admin";
const string defaultPassword = "password";

RPCImpl::RPCImpl(int socket)
{
	m_socket = socket;         // Assign socket to RPCImpl socket field. 
	m_rpcCount = 0;
	mg = new MealGenerator();  // Init new MG object.
	authObj = new Auth();      // Init new Auth object. 
	authObj->signUp(defaultID, defaultPassword, "Y"); // Seed Auth with default user and password. 
}
RPCImpl::RPCImpl(int socket, MealGenerator* mg, Auth* accountDB, GlobalContext* gc, sem_t* updateMG, sem_t* updateDB, sem_t* updateGC)
{
	m_socket = socket;         // Assign socket to RPCImpl socket field. 
	m_rpcCount = 0;
	this->mg = mg;  // Assign the meal database.
	authObj = accountDB;      // Init new Auth object. 
	authObj->signUp(defaultID, defaultPassword, "Y"); // Seed Auth with default user and password. 
	this->gc = gc;
	this->updateMG = updateMG;
	this->updateDB = updateDB;
	this->updateGC = updateGC;
}
;

RPCImpl::~RPCImpl() {};

bool RPCImpl::ProcessRPC()
{
	// List of valid RPC codes that ProcessRPC will recognize. 
	const vector<string> RPCList = { "signup", "connect", "logout", "disconnect", "status", "meal", "addMeal"};
	const vector<string> RPCStatList = { "totalRPC", "mostRequested" };
	const string SIGNUP = RPCList[0];
	const string CONNECT = RPCList[1];
	const string LOGOUT = RPCList[2];
	const string DISCONNECT = RPCList[3];
	const string STATUS = RPCList[4];
	const string MEAL = RPCList[5];
	const string ADDMEAL = RPCList[6];
	const string TOTALRPC = RPCStatList[0];
	const string MOSTREQUESTED = RPCStatList[1];
	
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
			
			sem_wait(updateGC);
			gc->incRPC();
			sem_post(updateGC);

			aString = CONNECT;
		}

		// Connect RPC - call ProcessConnectRPC when not connected and RPCToken is "connect". 
		if ((bConnected == false) && (aString == CONNECT)) {
			bStatusOk = ProcessConnectRPC(arrayTokens, bConnected);  

			sem_wait(updateGC);
			gc->incRPC();
			sem_post(updateGC);
		}
		
		// Disconnect RPC - call ProcessDisconnectRPC when connected and RPCToken is "disconnect". 
		else if ((bConnected == true) && (aString == DISCONNECT)) {
			bStatusOk = ProcessDisconnectRPC();
			printf("RPCServer>Disconnected.");

			sem_wait(updateGC);
			gc->incRPC();
			sem_post(updateGC);

			bContinue = false; 
		}

		else if ((bConnected == true) && (aString == LOGOUT)) {
			bStatusOk = ProcessLogoutRPC();
			printf("RPCServer>Logged Out.");

			sem_wait(updateGC);
			gc->incRPC();
			sem_post(updateGC);

			bConnected = false;
		}

		// Status RPC - call ProcessStatusRPC on RPCToken "status". 
		else if (aString == STATUS) {
			bStatusOk = ProcessStatusRPC();   // Status RPC

			sem_wait(updateGC);
			gc->incRPC();
			sem_post(updateGC);
		}

		// Get Meal RPC - call ProcessMealRPC when connected and RPCToken is "meal". 
		else if ((bConnected == true) && (aString == MEAL)) {
			bStatusOk = ProcessMealRPC(arrayTokens);  

			sem_wait(updateGC);
			gc->incRPC();
			sem_post(updateGC);
		}

		// AddMeal RPC - calls ProcessAddMealRPC when connected and RPCToken is "addMeal". 
		else if ((bConnected == true) && (aString == ADDMEAL)) {
			bStatusOk = ProcessAddMealRPC(arrayTokens);  

			sem_wait(updateGC);
			gc->incRPC();
			sem_post(updateGC);
		}

		// TotalRPC RPC - calls ProcessTotalRPC when connected and RPCToken is "totalRPC". 
		else if ((bConnected == true) && (aString == TOTALRPC)) {
			bStatusOk = ProcessTotalRPC();

			sem_wait(updateGC);
			gc->incRPC();
			sem_post(updateGC);
		}

		// MostRequested RPC - calls ProcessMostRequestedRPC when connected and RPCToken is "mostRequested". 
		else if ((bConnected == true) && (aString == MOSTREQUESTED)) {
			bStatusOk = ProcessMostRequestedRPC();

			sem_wait(updateGC);
			gc->incRPC();
			sem_post(updateGC);
		}

		else {
			printf("RPCServer>ProcessRPC: Invalid tokens: ", aString, "\n");
		}
	}
	return true;
}

bool RPCImpl::ProcessConnectRPC(vector<string>& arrayTokens, bool& connected)
{
	const int USERNAMETOKEN = 1;
	const int PASSWORDTOKEN = 2;

	// Strip out tokens 1 and 2 (username, password).
	string userNameString = arrayTokens[USERNAMETOKEN];
	string passwordString = arrayTokens[PASSWORDTOKEN];
	char szBuffer[80];

	// Try to login.
	connected = authObj->login(userNameString, passwordString);
	if (connected)
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

	sem_wait(updateDB);

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

	sem_post(updateDB);

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
		sem_wait(updateGC);
		if (info == "breakfast")
			gc->incBreakfast();
		else if (info == "lunch")
			gc->incLunch();
		else if (info == "dinner")
			gc->incDinner();
		sem_post(updateGC);
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

	sem_wait(updateMG);

	// Attempt to add new meal to Meal Generator class 
	if(!(mg->addMeal(name, time, cuisine)))  // Meal already exists in MG. 
		strcpy(szBuffer, FAILCODE);
	else
		strcpy(szBuffer,SUCCESSCODE);        // New meal added. 
	int nlen = strlen(szBuffer);
	szBuffer[nlen] = 0;
	send(this->m_socket, szBuffer, strlen(szBuffer) + 1, 0);

	sem_post(updateMG);

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

bool RPCImpl::ProcessLogoutRPC()
{
	char szBuffer[16];  // Size of output buffer.
	strcpy(szBuffer, "loggedout;");
	// Send Response back to client.
	int nlen = strlen(szBuffer);
	szBuffer[nlen] = 0;
	send(this->m_socket, szBuffer, strlen(szBuffer) + 1, 0);
	return true; // RPC complete. 
}


bool RPCImpl::ProcessTotalRPC()
{
	char szBuffer[16]; // Size of output buffer. 
	string stat = SUCCESSCODE + to_string(this->gc->totalRequest()) + ";";
	strcpy(szBuffer, stat.c_str());
	// Send Response back on our socket
	int nlen = strlen(szBuffer);
	szBuffer[nlen] = 0;
	send(this->m_socket, szBuffer, strlen(szBuffer) + 1, 0);
	return true;  // RPC complete. 
}

bool RPCImpl::ProcessMostRequestedRPC()
{
	char szBuffer[16]; // Size of output buffer. 
	string stat = SUCCESSCODE + this->gc->mostRequested() + ";";
	strcpy(szBuffer, stat.c_str());
	// Send Response back on our socket
	int nlen = strlen(szBuffer);
	szBuffer[nlen] = 0;
	send(this->m_socket, szBuffer, strlen(szBuffer) + 1, 0);
	return true;  // RPC complete. 
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