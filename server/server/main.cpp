#include "RPCServer.h"
#include <cstdio>       // printf
#include <iostream>     // C++ IO

int main(int argc, char const* argv[])
{

	const char* serverIP = argv[1];
	int port = atoi(argv[2]);

	bool statusOk = true;
	RPCServer* serverObj = new RPCServer(serverIP, port);

	// Print it out

	statusOk = serverObj->StartServer();
	printf("Endless loop, as server is up always");
	while (statusOk)
	{
		statusOk = serverObj->ListenForClient();
		statusOk = serverObj->ProcessRPC(); // Launch thread
	}

	delete serverObj;
	return 0;
}