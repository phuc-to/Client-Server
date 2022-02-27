/**
 @file Auth.h
 @authors Phuc T, Narissa T, Kristen K
 @date 3/10/22
 @version 1.0
 */

#pragma once

#include <string>

using namespace std;

class Auth
{
public:
	/** Auth class constructor*/
	Auth();

	/** Auth class destructor*/
	~Auth();

private: 
	string username;
	string password;
	bool isAdmin; 
	bool isConnected;
};

