/**
 @file  main.cpp - Driver file which runs the server side of the application. 
 @authors Phuc T, Narissa T, Kristen K
 @date 3/10/22
 @version 3.0
 */

#pragma once

#include <cstdio>       // printf
#include <iostream>     // C++ IO

#include "RPCServer.h"

/*
 * Server startup. Requires manual shutdown. 
 */
int main(int argc, char const* argv[])
{
	cout << "main: Starting in main...\n";
	// New server object init. 
	const char* serverIP = argv[1]; // Holds server's IP address. 
	// int port = atoi(argv[2]);       // holds port number. 
	const int port = 10327;

	bool statusOk = true;           // Holds server status, true when server is running. 
	RPCServer* serverObj = new RPCServer(serverIP, port);

	statusOk = serverObj->StartServer();
	while (statusOk) {
		statusOk = serverObj->ListenForClient();
		//statusOk = serverObj->ProcessRPC(); // Launch thread
	}

	delete serverObj;
	return 0;
}

