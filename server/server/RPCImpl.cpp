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
	vault = new Auth();
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
#if 0
		// Enumerate through the tokens. The first token is always the specific RPC
		for (vector<string>::iterator t = arrayTokens.begin(); t != arrayTokens.end(); ++t)
		{
			printf("Debugging our tokens\n");
			printf("token = %s\n", t);
		}
#endif
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

// OUR CUSTOM PROCESSRPC
//* ProcessRPC will examine buffer and will essentially control which RPC calls
//* are made. The function invokes the ParseTokens method to read the message
//* sent by the client, and then invokes the appropriate RPC method call.
//*
//*/
//bool RPCServer::ProcessRPC()
//{
//	const char* rpcs[] = { "connect", "disconnect", "status" };
//	char buffer[1024] = { 0 };
//	std::vector<std::string> arrayTokens;
//	int valread = 0;
//	bool bConnected = false;
//	bool bStatusOk = true;
//	const int RPCTOKEN = 0;
//	bool bContinue = true;
//
//	while ((bContinue) && (bStatusOk))
//	{
//		// Should be blocked when a new RPC has not called us yet
//		if ((valread = read(this->m_socket, buffer, sizeof(buffer))) <= 0)
//		{
//			printf("errno is %d\n", errno);
//			break;
//		}
//		printf("%s\n", buffer);
//
//		arrayTokens.clear();
//		this->ParseTokens(buffer, arrayTokens);
//
//		// string statements are not supported with a switch, so using if/else logic to dispatch
//		string aString = arrayTokens[RPCTOKEN];
//
//		if ((bConnected == false) && (aString == "connect"))
//		{
//			bStatusOk = ProcessConnectRPC(arrayTokens);  // Connect RPC
//			if (bStatusOk == true)
//				bConnected = true;
//		}
//
//		else if ((bConnected == true) && (aString == "disconnect"))
//		{
//			bStatusOk = ProcessDisconnectRPC();
//			printf("We are going to terminate this endless loop\n");
//			bContinue = false; // We are going to leave this loop, as we are done
//		}
//
//		else if ((bConnected == true) && (aString == "status"))
//			bStatusOk = ProcessStatusRPC();   // Status RPC
//
//		else if ((bConnected == true) && (aString == "mealRandom"))
//		{
//			bStatusOk = ProcessMealRPC(arrayTokens);
//		}
//
//		else if ((bConnected == true) && (aString == "mealByTime"))
//		{
//			bStatusOk = ProcessMealRPC(arrayTokens);
//		}
//
//		else if ((bConnected == true) && (aString == "mealByCuisine"))
//		{
//			bStatusOk = ProcessMealRPC(arrayTokens);
//		}
//		else
//		{
//			// Not in our list, perhaps, print out what was sent
//		}
//
//	}
//
//	return true;
//}

bool RPCImpl::ProcessConnectRPC(std::vector<std::string>& arrayTokens)
{
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

bool RPCImpl::ProcessSignupRPC(std::vector<std::string>& arrayTokens)
{
	const int USERNAMETOKEN = 1;
	const int PASSWORDTOKEN = 2;

	// Strip out tokens 1 and 2 (username, password)
	string userNameString = arrayTokens[USERNAMETOKEN];
	string passwordString = arrayTokens[PASSWORDTOKEN];
	char szBuffer[80];

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


// old code come back to this

/* Returns a buffer containing the information
/* for the meal that meets the client's submitted
/* criteria. 
/*/
bool RPCImpl::ProcessMealRPC(std::vector<std::string>& arrayTokens)
{
    const int RPCTOKEN = 1;
    const int INFOTOKEN = 2;

    // Strip out tokens 1 and 2 (username, password)
    string RPC = arrayTokens[RPCTOKEN];
    string info = arrayTokens[INFOTOKEN];
    char szBuffer[80];
	if (RPC == "Random") 
	{
		string output = mg->getRandomMeal();
		if (output != "")
			strcpy(szBuffer, ("successful;" + output).c_str());
		else
			strcpy(szBuffer, "failed;Meal List is empty;");
	}
	else if (RPC == "ByTime")
	{
		string output = mg->getRandomMealByTime(info);
		if (output != "")
			strcpy(szBuffer, ("successful;" + output).c_str());
		else
		{
			string error = "failed;There is no meal in the list for " + info + ";";
			strcpy(szBuffer, error.c_str());
		}
	}
	else if (RPC == "ByCuisine")
	{
		string output = mg->getRandomMealByCuisine(info);
		if (output != "")
			strcpy(szBuffer, ("successful;" + output).c_str());
		else
		{
			string error = "failed;There is no meal in the list from " + info + ";";
			strcpy(szBuffer, error.c_str());
		}
	}
	else
		strcpy(szBuffer, "failed;Invalid Argument, or Not Supported Operations;");
    // Send Response back on our socket
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

	// Strip out tokens 1 and 2 (username, password)
	string name = arrayTokens[NAMETOKEN];
	string time = arrayTokens[TIMETOKEN];
	string cuisine = arrayTokens[CUISINETOKEN];
	char szBuffer[80];

	mg->addMeal(name, time, cuisine);
	strcpy(szBuffer, "successful");
	int nlen = strlen(szBuffer);
	szBuffer[nlen] = 0;
	send(this->m_socket, szBuffer, strlen(szBuffer) + 1, 0);
	return true;
}
