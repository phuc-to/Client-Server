/**
 @file  RPCImpl.h
 @authors Phuc T, Narissa T, Kristen K
 @date 3/10/22
 @version 1.0
 @reference https://www.geeksforgeeks.org/socket-programming-cc/
 @reference https://www.gnu.org/software/libc/manual/html_node/Sockets.html
 */


#pragma once

#include <unistd.h>
#include <stdio.h>
#include <sys/socket.h>
#include <stdlib.h>
#include <netinet/in.h>
#include <string.h>
#include <vector>
#include <iterator>


class RPCImpl
{
public:
    RPCImpl(int socket);
    ~RPCImpl();

	/**
	* Function for processing all RPC calls.
	* @pre Buffer sent by the client.
	* @post The appropriate RPC call has been invoked.
	*/
	bool ProcessRPC();

private:
    int m_rpcCount;
    int m_socket;


    // First one in this function should be a connect, and it 
    // will continue try to process RPC's until a Disconnect happens
    bool ProcessConnectRPC(std::vector<std::string>& arrayTokens);
    bool ProcessStatusRPC();
    bool ProcessDisconnectRPC();
    void ParseTokens(char* buffer, std::vector<std::string>& a);

};
