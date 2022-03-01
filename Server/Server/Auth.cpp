/**
 @file auth.cpp
 @authors Phuc T, Narissa T, Kristen K
 @date 3/10/22
 @version 1.0
 */

#include "Auth.h"
#include <unordered_map>

Auth::Auth()
{

}

Auth::~Auth()
{
}

bool Auth::UserLookup(const string username, const string password)
{
	string key = username + password;
	bool isAdmin = false; 

	// check if user already exists
	if (this->userLookupMap.find(key) == userLookupMap.end()) {
		cout << "New user, creating an account...\n";
		this->userLookupMap.insert({key, true});
		cout << "New user created. Logged in successfully\n";
	}
	else {
		this->userLookupMap[key] = true;
		cout << "User found. Logged in successfully\n";
	}
	return true;
}

